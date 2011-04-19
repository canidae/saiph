#include "Analyzers/Fight.h"

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Fight.h"
#include "Actions/Move.h"
#include "Actions/Throw.h"
#include "Actions/Search.h"
#include "Data/Dagger.h"
#include "Data/Dart.h"
#include "Data/Monster.h"
#include "Data/Spear.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Fight::Fight() : Analyzer("Fight") {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
}

/* methods */
void Fight::analyze() {
	/* if engulfed try to fight our way out */
	if (Saiph::engulfed()) {
		World::setAction(static_cast<action::Action*> (new action::Fight(this, NW, PRIORITY_FIGHT_MELEE_MAX)));
		return;
	}

	bool bosses_adjacent = false;

	if (Saiph::position().level() != _boss_waiting_level) {
		_boss_waiting_level = Saiph::position().level();
		_boss_waiting_since = World::turn();
	}

	for (map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
		if (Point::gridDistance(m->first, Saiph::position()) > 1)
			continue;
		if (m->second.data() == NULL)
			continue;
		if ((m->second.data()->m3() & M3_COVETOUS) == 0)
			continue;

		Debug::custom(name()) << "Moving " << m->second.data()->name() << " to " << World::level().name() << endl;
		_boss_last_seen[m->second.data()->name()] = Saiph::position().level();
		_boss_waiting_since = World::turn();
		bosses_adjacent = true;
	}

	if ((World::turn() - _boss_waiting_since) > BOSS_PATIENCE) {
		for (map<string, int>::iterator m = _boss_last_seen.begin(); m != _boss_last_seen.end(); ) {
			if (m->second != Saiph::position().level()) {
				++m;
				continue;
			}
			const vector<Level>& levels = World::levels();
			int tgt = UNKNOWN_SYMBOL_VALUE;
			Debug::custom(name()) << "Current level = " << m->second << " = " << World::level().depth() << "," << World::level().branch() << endl;
			for (unsigned i = 0; i < levels.size(); ++i) {
				Debug::custom(name()) << "Level " << i << " = " << levels[i].depth() << "," << levels[i].branch() << endl;
				if (levels[i].branch() == World::level().branch() && levels[i].depth() == (World::level().depth() - 1)) {
					tgt = i;
					break;
				}
			}
			Debug::custom(name()) << m->first << " has not been seen in " << BOSS_PATIENCE << " turns; moving it to " << tgt << endl;
			if (tgt == UNKNOWN_SYMBOL_VALUE)
				_boss_last_seen.erase(m++);
			else {
				m->second = tgt;
				++m;
			}
		}
	}

	vector<Coordinate> interesting_boss_stairs;
	for (map<string, int>::iterator b = _boss_last_seen.begin(); b != _boss_last_seen.end(); ++b) {
		Level& lv = World::level(b->second);
		for (map<Point,int>::const_iterator m = lv.symbols(STAIRS_DOWN).begin(); m != lv.symbols(STAIRS_DOWN).end(); ++m)
			interesting_boss_stairs.push_back(Coordinate(b->second, m->first));
		for (map<Point,int>::const_iterator m = lv.symbols(STAIRS_UP).begin(); m != lv.symbols(STAIRS_UP).end(); ++m)
			interesting_boss_stairs.push_back(Coordinate(b->second, m->first));
	}

	if (interesting_boss_stairs.size()) {
		Tile best_tile;
		for (vector<Coordinate>::iterator it = interesting_boss_stairs.begin(); it != interesting_boss_stairs.end(); ++it) {
			Tile tn = World::shortestPath(*it);
			if (tn.cost() < best_tile.cost()) best_tile = tn;
		}
		if (best_tile.cost() < UNPASSABLE) {
			if (best_tile.direction() != NOWHERE) {
				Debug::custom(name()) << "Going to fight a boss at " << best_tile.coordinate() << endl;
				World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, PRIORITY_FIGHT_POSITION_BOSS)));
			} else if (best_tile.symbol() == STAIRS_UP) {
				// Hah! We're on the <, you're done for.  Just wait for you to come...
				Debug::custom(name()) << "Waiting to kill a boss at " << best_tile.coordinate() << endl;
				World::setAction(static_cast<action::Action*> (new action::Search(this, PRIORITY_FIGHT_POSITION_BOSS)));
			} else {
				// We want to fight the boss on the stairs on the next level down
				if (bosses_adjacent) {
					Debug::custom(name()) << "Luring a boss downstairs at " << best_tile.coordinate() << endl;
					best_tile.direction(DOWN);
					World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, PRIORITY_FIGHT_POSITION_BOSS_LURE)));
				} else {
					Debug::custom(name()) << "Waiting to lure a boss downstairs at " << best_tile.coordinate() << endl;
					World::setAction(static_cast<action::Action*> (new action::Search(this, PRIORITY_FIGHT_POSITION_BOSS)));
				}
			}
		}
	}

	/* fight monsters */
	int attack_score = INT_MIN;
	for (map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
		if (m->second.symbol() == PET)
			continue; // we're not fighting pets :)
		else if (m->second.attitude() == FRIENDLY && m->second.symbol() != S_HUMANOID)
			continue; // don't attack friendlies except dwarfs
		else if (m->second.symbol() == S_UNICORN && ((m->second.color() == BOLD_WHITE && Saiph::alignment() == LAWFUL) || (m->second.color() == WHITE && Saiph::alignment() == NEUTRAL) || (m->second.color() == BLUE && Saiph::alignment() == CHAOTIC)))
			continue; // don't attack unicorns of same alignment
		else if (m->second.data() == NULL) {
			/* seems like MonsterInfo haven't had the chance to farlook monster. set attack_score to max */
			Debug::custom(name()) << "Found monster we don't know data about '" << m->second.symbol() << "'. Hmm" << endl;
			attack_score = data::Monster::saiphDifficultyMax();
		} else {
			/* figure out the attack score */
			/* FIXME
			 * this needs to be improved later.
			 * currently we use "saiph_difficulty", whether we can melee/throw at it and how far away it is.
			 * this is a too simple solution, we need to consider stuff as how hard they hit, if they steal
			 * stuff, if they use wands, etc */
			attack_score = m->second.data()->saiphDifficulty();
		}
		int distance = Point::gridDistance(m->first, Saiph::position());
		bool floating_eye = (m->second.symbol() == S_EYE && m->second.color() == BLUE);
		if (m->second.visible() && (distance > 1 || floating_eye) && _projectile_slots.size() > 0 && distance <= Saiph::strength() / 2 && Saiph::encumbrance() < OVERTAXED) {
			/* got projectiles and monster is not next to us or it's a floating eye.
			 * should check if we can throw projectile at the monster */
			unsigned char in_line = World::directLine(m->first, false, true);
			if (in_line != ILLEGAL_DIRECTION) {
				/* we can throw at monster */
				attack_score -= distance;
				int priority = (attack_score - data::Monster::saiphDifficultyMin()) * (PRIORITY_FIGHT_THROW_MAX - PRIORITY_FIGHT_THROW_MIN) / (data::Monster::saiphDifficultyMax() - data::Monster::saiphDifficultyMin()) + PRIORITY_FIGHT_THROW_MIN;
				World::setAction(static_cast<action::Action*> (new action::Throw(this, *_projectile_slots.begin(), in_line, priority)));
				Debug::custom(name()) << "Setting action to throw at '" << m->second.symbol() << "' which is " << distance << " squares away with priority " << priority << endl;
				continue;
			}
		}
		if (m->second.symbol() == 'c' && (m->second.color() == YELLOW || m->second.color() == BOLD_YELLOW) && (Inventory::keyForSlot(SLOT_WEAPON) == ILLEGAL_ITEM) && (Inventory::keyForSlot(SLOT_GLOVES) == ILLEGAL_ITEM))
			continue; // Don't even think about attacking cockatrices with our bare hands
		if (distance == 1) {
			/* next to monster, and it's not a floating eye. melee */
			int priority = (floating_eye ? 10 : (attack_score - data::Monster::saiphDifficultyMin()) * (PRIORITY_FIGHT_MELEE_MAX - PRIORITY_FIGHT_MELEE_MIN) / (data::Monster::saiphDifficultyMax() - data::Monster::saiphDifficultyMin()) + PRIORITY_FIGHT_MELEE_MIN);
			World::setAction(static_cast<action::Action*> (new action::Fight(this, World::shortestPath(m->first).direction(), priority)));
			Debug::custom(name()) << "Setting action to melee '" << m->second.symbol() << "' with priority " << priority << endl;
			continue;
		}
		if (Saiph::blind() || Saiph::confused() || Saiph::stunned())
			continue; // don't move while blind/confused/stunned
		/* we can neither melee nor throw at the monster, move towards it */
		Tile& tile = World::shortestPath(m->first);
		if (tile.cost() >= UNPASSABLE)
			continue; // can't move to monster
		int priority = (floating_eye ? 10 : (attack_score - data::Monster::saiphDifficultyMin()) * (PRIORITY_FIGHT_MOVE_MAX - PRIORITY_FIGHT_MOVE_MIN) / (data::Monster::saiphDifficultyMax() - data::Monster::saiphDifficultyMin()) + PRIORITY_FIGHT_MOVE_MIN);
		priority = action::Move::calculatePriority(priority, tile.cost());
		World::setAction(static_cast<action::Action*> (new action::Move(this, tile, priority, false)));
		Debug::custom(name()) << "Setting action to move towards '" << m->second.symbol() << "' which is " << distance << " squares away with priority " << priority << endl;
	}
}

void Fight::onEvent(Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		for (set<unsigned char>::iterator k = e->keys().begin(); k != e->keys().end(); ++k) {
			map<unsigned char, Item>::iterator i = Inventory::items().find(*k);
			if (i == Inventory::items().end()) {
				/* we lost this item, remove it from projectile_slots */
				_projectile_slots.erase(*k);
			} else {
				/* this item is new or changed.
				 * if we intend to throw it, add it to projectile_slots.
				 * otherwise remove it from projectile_slots */
				map<const string, const data::Weapon*>::const_iterator w = data::Weapon::weapons().find(i->second.name());
				if (w == data::Weapon::weapons().end())
					continue; // not a weapon
				if (w->second->type() == WEAPON_DAGGER || w->second->type() == WEAPON_DART || w->second->type() == WEAPON_JAVELIN || w->second->type() == WEAPON_KNIFE || w->second->type() == WEAPON_SHURIKEN || w->second->type() == WEAPON_SPEAR)
					_projectile_slots.insert(*k);
				else
					_projectile_slots.erase(*k);
			}
		}
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			map<const string, const data::Weapon*>::const_iterator w = data::Weapon::weapons().find(i->second.name());
			if (w == data::Weapon::weapons().end())
				continue; // not a weapon
			if (w->second->type() == WEAPON_DAGGER || w->second->type() == WEAPON_DART || w->second->type() == WEAPON_JAVELIN || w->second->type() == WEAPON_KNIFE || w->second->type() == WEAPON_SHURIKEN || w->second->type() == WEAPON_SPEAR)
				_projectile_slots.insert(i->first);
		}
	}
}
