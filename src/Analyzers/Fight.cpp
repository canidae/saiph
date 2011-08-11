#include "Analyzers/Fight.h"

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Monster.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Fight.h"
#include "Actions/Move.h"
#include "Actions/Throw.h"
#include "Actions/Search.h"
#include "Data/Attack.h"
#include "Data/Dagger.h"
#include "Data/Dart.h"
#include "Data/Monster.h"
#include "Data/Spear.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"

/*
#include <sstream>
#include <iomanip>
*/

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Fight::Fight() : Analyzer("Fight") {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);

	for (vector<const data::Monster*>::const_iterator mi = data::Monster::monsters().begin(); mi != data::Monster::monsters().end(); ++mi) {
		const data::Monster* m = *mi;
		bool nokite = false;
		if (m->m1() & M1_TPORT_CNTRL) nokite = true;
		if (m->m2() & (M2_WERE | M2_WANDER)) nokite = true;
		if (m->m3() & (M3_COVETOUS | M3_WAITMASK)) nokite = true;
		if (m->rangedAttack()) nokite = true;
		if (nokite) {
			//Debug::custom(name()) << "Disallow hit and run of " << m->name() << endl;
			_no_hitandrun.insert(m->name());
		}
	}
}

/* methods */
void Fight::analyze() {
	/* if engulfed try to fight our way out */

	/*
	Level& l = World::level();
	for (int y = MAP_ROW_BEGIN; y <= MAP_ROW_END; ++y) {
		string mobs;
		ostringstream costs;
		for (int x = MAP_COL_BEGIN; x <= MAP_COL_END; ++x) {
			Tile& t = l.tile(Point(y,x));
			mobs.push_back(t.symbol() > 126 ? '?' : t.symbol());
			mobs.push_back(' ');
			mobs.push_back(t.monster() == ILLEGAL_MONSTER ? '_' : t.monster());
			mobs.push_back(' ');
			costs << setw(4) << (t.cost() > 999 ? 999 : t.cost());
		}
		Debug::info() << mobs << endl;
		Debug::info() << costs.str() << endl;
	}
	*/

	if (Saiph::engulfed()) {
		World::setAction(new action::Fight(this, NW, PRIORITY_FIGHT_MELEE_MAX));
		return;
	}

	bool bosses_adjacent = false;

	if (Saiph::position().level() != _boss_waiting_level) {
		_boss_waiting_level = Saiph::position().level();
		_boss_waiting_since = World::turn();
	}

	for (map<Point, Monster*>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
		if (Point::gridDistance(m->first, Saiph::position()) > 1)
			continue;
		if (m->second->data() == NULL)
			continue;
		if ((m->second->data()->m3() & M3_COVETOUS) == 0)
			continue;

		Debug::custom(name()) << "Moving " << m->second->data()->name() << " to " << World::level().name() << endl;
		_boss_last_seen[m->second->data()->name()] = Saiph::position().level();
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
			if (tn.cost() < best_tile.cost())
				best_tile = tn;
		}
		if (best_tile.cost() < UNPASSABLE) {
			if (best_tile.direction() != NOWHERE) {
				Debug::custom(name()) << "Going to fight a boss at " << best_tile.coordinate() << endl;
				World::setAction(new action::Move(this, best_tile, PRIORITY_FIGHT_POSITION_BOSS));
			} else if (best_tile.symbol() == STAIRS_UP) {
				// Hah! We're on the <, you're done for.  Just wait for you to come...
				Debug::custom(name()) << "Waiting to kill a boss at " << best_tile.coordinate() << endl;
				World::setAction(new action::Search(this, PRIORITY_FIGHT_POSITION_BOSS));
			} else {
				// We want to fight the boss on the stairs on the next level down
				if (bosses_adjacent) {
					Debug::custom(name()) << "Luring a boss downstairs at " << best_tile.coordinate() << endl;
					best_tile.direction(DOWN);
					World::setAction(new action::Move(this, best_tile, PRIORITY_FIGHT_POSITION_BOSS_LURE));
				} else {
					Debug::custom(name()) << "Waiting to lure a boss downstairs at " << best_tile.coordinate() << endl;
					World::setAction(new action::Search(this, PRIORITY_FIGHT_POSITION_BOSS));
				}
			}
		}
	}

	map<Point,int> mon_range;
	for (map<Point, Monster*>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
		if (World::subTurn() >= 0 && World::subTurn() + 1 < Saiph::minMovesThisTurn()) {
			mon_range[m->first] = (m->second->maxMovesThisTurn() > World::subTurn()) ? 1 : 0;
		} else {
			mon_range[m->first] = std::max(0, m->second->maxMovesThisTurn() - std::max(0,World::subTurn()));
		}
	}

	set<Point> safe_backup;
	for (Point::adjacent_iterator ai(Saiph::position(), true); ai; ++ai) {
		Point p = *ai;
		if (World::level().tile(p).cost() >= UNPASSABLE)
			continue;
		bool ok = true;
		for (map<Point,int>::const_iterator m = mon_range.begin(); m != mon_range.end(); ++m) {
			if (Point::gridDistance(p, m->first) <= m->second) {
				ok = false;
				break;
			}
		}
		if (ok) {
			//Debug::custom(name()) << "Found safe backup square " << p << endl;
			safe_backup.insert(p);
		}
	}

	/* fight monsters */
	int attack_score = INT_MIN;
	for (map<Point, Monster*>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
		if (m->second->symbol() == PET)
			continue; // we're not fighting pets :)
		else if (m->second->attitude() == FRIENDLY && m->second->symbol() != S_HUMANOID)
			continue; // don't attack friendlies except dwarves
			/* I'm not sure about how good this is for her in the mines.
			 * This was added to prevent her sticking in levels that were
			 * being dug out - but I still sometimes see this happening
			 * and it may have killed her. -rawrmage */
		else if (m->second->symbol() == 'm' && m->second->color() == BLUE && Saiph::experience() < 3)
			continue; // below level three, let strange objects be
		else if (m->second->symbol() == 'n' && (m->second->lastSeen() - m->second->lastMoved()) >= 3)
			continue; // let sleeping nymphs lie
		else if (m->second->symbol() == S_UNICORN && ((m->second->color() == BOLD_WHITE && Saiph::alignment() == LAWFUL) || (m->second->color() == WHITE && Saiph::alignment() == NEUTRAL) || (m->second->color() == BLUE && Saiph::alignment() == CHAOTIC)))
			continue; // don't attack unicorns of same alignment
		else if (m->second->data() == NULL) {
			/* seems like MonsterInfo haven't had the chance to farlook monster. set attack_score to max */
			Debug::custom(name()) << "Found monster we don't know data about '" << m->second->symbol() << "'. Hmm" << endl;
			attack_score = data::Monster::saiphDifficultyMax();
		} else {
			/* figure out the attack score */
			/* FIXME
			 * this needs to be improved later.
			 * currently we use "saiph_difficulty", whether we can melee/throw at it and how far away it is.
			 * this is a too simple solution, we need to consider stuff as how hard they hit, if they steal
			 * stuff, if they use wands, etc */
			attack_score = m->second->data()->saiphDifficulty();
			Debug::custom(name()) << "Attack score for monster '" << m->second->data()->name() << ": " << attack_score << " (" << data::Monster::saiphDifficultyMin() << '-' << data::Monster::saiphDifficultyMax() << ")" << endl;
		}
		int distance = Point::gridDistance(m->first, Saiph::position());
		bool floating_eye = (m->second->symbol() == S_EYE && m->second->color() == BLUE);
		bool can_shoot = _projectile_slots.size() > 0 && Saiph::encumbrance() < OVERTAXED && !Saiph::hallucinating();
		if (m->second->visible() && (distance > 1 || floating_eye) && can_shoot) {
			/* got projectiles and monster is not next to us or it's a floating eye.
			 * should check if we can throw projectile at the monster */
			unsigned char in_line = World::directLine(m->first, false, true, Saiph::strength() / 2, Saiph::strength() / 2);
			if (in_line != ILLEGAL_DIRECTION) {
				/* we can throw at monster */
				attack_score -= distance;
				int priority = (attack_score - data::Monster::saiphDifficultyMin()) * (PRIORITY_FIGHT_THROW_MAX - PRIORITY_FIGHT_THROW_MIN) / (data::Monster::saiphDifficultyMax() - data::Monster::saiphDifficultyMin()) + PRIORITY_FIGHT_THROW_MIN;
				World::setAction(new action::Throw(this, *_projectile_slots.begin(), in_line, priority));
				Debug::custom(name()) << "Setting action to throw at '" << m->second->symbol() << "' which is " << distance << " squares away with priority " << priority << endl;
				continue;
			}
		}
		if (m->second->symbol() == 'c' && (m->second->color() == YELLOW || m->second->color() == BOLD_YELLOW) && (Inventory::keyForSlot(SLOT_WEAPON) == ILLEGAL_ITEM) && (Inventory::keyForSlot(SLOT_GLOVES) == ILLEGAL_ITEM))
			continue; // Don't even think about attacking cockatrices with our bare hands
		// TODO: can't kite if mob has motion-affecting statuses
		int speed = m->second->data() ? m->second->data()->moveRate() : 36;
		bool kitable = m->second->data() && Saiph::maxSpeed() > speed && (can_shoot || Saiph::minSpeed() > speed);
		bool infravisible = Saiph::infravision() && m->second->data() && (m->second->data()->m3() & M3_INFRAVISIBLE) != 0;
		if (!infravisible && (World::level().tile(Saiph::position()).lit() <= 0 || World::level().tile(m->first).lit() <= 0))
			kitable = false; // no hit and run in the dark
		if (m->second->attitude() == FRIENDLY || (m->second->lastSeen() - m->second->lastMoved()) > 10)
			kitable = false; // it doesn't seem willing to follow us.
		if (m->second->data() && _no_hitandrun.find(m->second->data()->name()) != _no_hitandrun.end())
			kitable = false; // ranged attacks or otherwise problematic
		if (m->second->ranged())
			kitable = false; // eep! monster won't just chase us
		Debug::custom(name()) << "kitable=" << kitable << " speed=" << speed << " range=" << mon_range[m->first] << " distance=" << distance << endl;
		if (kitable && mon_range[m->first] > 0 && distance <= 2) {
			// mob will strike back if we attack it or move towards it, and we can avoid that
			Point good;
			int min_dist = 100;
			// we prioritize horizontal directions because NetHack rooms are generally much wider than tall
			for (Point::adjacent_iterator ai(Saiph::position(), "hlyubnjk"); ai; ++ai) {
				Point p = *ai;
				if (safe_backup.find(p) != safe_backup.end() && Point::gridDistance(m->first, p) < min_dist) {
					good = p;
					min_dist = Point::gridDistance(m->first, p);
					break;
				}
			}
			if (good.insideMap()) {
				int priority = (floating_eye ? 10 : (attack_score - data::Monster::saiphDifficultyMin()) * (PRIORITY_FIGHT_MELEE_MAX - PRIORITY_FIGHT_MELEE_MIN) / (data::Monster::saiphDifficultyMax() - data::Monster::saiphDifficultyMin()) + PRIORITY_FIGHT_MELEE_MIN);
				World::setAction(new action::Move(this, World::level().tile(good), priority, false));
				Debug::custom(name()) << "Setting action to kite '" << m->second->symbol() << "' moving to " << good << " at priority " << priority << endl;
				continue;
			}
		}

		if (distance == 1) {
			/* next to monster, and it's not a floating eye. melee */
			int priority = (floating_eye ? 10 : (attack_score - data::Monster::saiphDifficultyMin()) * (PRIORITY_FIGHT_MELEE_MAX - PRIORITY_FIGHT_MELEE_MIN) / (data::Monster::saiphDifficultyMax() - data::Monster::saiphDifficultyMin()) + PRIORITY_FIGHT_MELEE_MIN);
			World::setAction(new action::Fight(this, World::shortestPath(m->first).direction(), priority));
			Debug::custom(name()) << "Setting action to melee '" << m->second->symbol() << "' with priority " << priority << endl;
			continue;
		}
		if (Saiph::hallucinating() || Saiph::blind() || Saiph::confused() || Saiph::stunned())
			continue; // don't move while blind/confused/stunned
		/* we can neither melee nor throw at the monster, move towards it */
		Tile* tile = 0;
		for (Point::adjacent_iterator ai(m->first); ai; ++ai) {
			Tile& cand = World::shortestPath(*ai);
			if (!tile || cand.cost() < tile->cost())
				tile = &cand;
		}
		if (!tile || tile->cost() >= UNPASSABLE)
			continue; // can't move to monster
		int priority = (floating_eye ? 10 : (attack_score - data::Monster::saiphDifficultyMin()) * (PRIORITY_FIGHT_MOVE_MAX - PRIORITY_FIGHT_MOVE_MIN) / (data::Monster::saiphDifficultyMax() - data::Monster::saiphDifficultyMin()) + PRIORITY_FIGHT_MOVE_MIN);
		priority = action::Move::calculatePriority(priority, tile->cost());
		World::setAction(new action::Move(this, *tile, priority, false));
		Debug::custom(name()) << "Setting action to move towards '" << m->second->symbol() << "' which is " << tile->cost() << " cost away with priority " << priority << endl;
	}
}

void Fight::onEvent(Event* const event) {
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
				if (w->second->type() == P_DAGGER || w->second->type() == -P_DART || w->second->type() == P_JAVELIN || w->second->type() == P_KNIFE || w->second->type() == -P_SHURIKEN || w->second->type() == P_SPEAR)
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
			if (w->second->type() == P_DAGGER || w->second->type() == -P_DART || w->second->type() == P_JAVELIN || w->second->type() == P_KNIFE || w->second->type() == -P_SHURIKEN || w->second->type() == P_SPEAR)
				_projectile_slots.insert(i->first);
		}
	}
}

void Fight::parseMessages(const string& messages) {
	vector<string> deaths;
	string::size_type p, p2;
	// Monnam is [killed/destroyed][ by the %s]!
	// You [destroy/kill] mon_nam!
	for (p = 0; ; ) {
		if ((p2 = messages.find("  You kill ", p)) == string::npos)
			break;
		p = messages.find("!  ", p2 + 11);
		deaths.push_back(messages.substr(p2 + 11, p - (p2 + 11)));
	}
	for (p = 0; ; ) {
		if ((p2 = messages.find("  You destroy ", p)) == string::npos)
			break;
		p = messages.find("!  ", p2 + 14);
		deaths.push_back(messages.substr(p2 + 14, p - (p2 + 14)));
	}
	for (p = 0; ; ) {
		if ((p = messages.find(" is killed", p)) == string::npos)
			break;
		p2 = messages.rfind("  ", p);
		deaths.push_back(messages.substr(p2 + 2, p - (p2 + 2)));
		p += 10;
	}
	for (p = 0; ; ) {
		if ((p = messages.find(" is destroyed", p)) == string::npos)
			break;
		p2 = messages.rfind("  ", p);
		deaths.push_back(messages.substr(p2 + 2, p - (p2 + 2)));
		p += 13;
	}

	for (vector<string>::iterator di = deaths.begin(); di != deaths.end(); ++di) {
		map<string,int>::iterator bossit = _boss_last_seen.begin();
		// needs to be a substring match for now because I don't feel like stripping off all the adjectives nethack puts on
		while (bossit != _boss_last_seen.end() && di->find(bossit->first) == string::npos) ++bossit;
		if (bossit != _boss_last_seen.end()) {
			_boss_last_seen.erase(bossit);
			Debug::custom(name()) << "We seem to have killed <" << *di << ">" << endl;
			Debug::custom(name()) << "Removing it from the boss list." << endl;
		}
	}
}

// The return value from this is intended to represent the per-1000 damage that we'll get standing here and fighting.  It is of course extremely approximate.
int Fight::pointVulnerability(const Point& pt, int elbereth_factor, bool isCrowd, bool isStalk, bool isFast) {
	Tile& t = World::level().tile(pt);
	int vuln = 1000;

	if (t.symbol() == FOUNTAIN || t.symbol() == GRAVE || t.symbol() == ALTAR || t.symbol() == WATER || t.symbol() == LAVA)
		vuln += 9 * elbereth_factor; /* can't E here */
	if (!isStalk && (t.symbol() == STAIRS_UP || t.symbol() == STAIRS_DOWN))
		vuln /= 3; /* easy escape, very desirable */

	int open_neighbors = 0;
	for (int row = pt.row() - 1; row <= pt.row() + 1; ++row) {
		for (int col = pt.col() - 1; col <= pt.col() + 1; ++col) {
			if (Level::isPassable(World::level().tile(Point(row,col)).symbol()))
				++open_neighbors;
		}
	}

	/* TODO: Weight open neighbors 4/2/1 by direction relative to centroid of threat
	   This will improve behavior in cases like this:
	   .|..aa.
	   ..@.aaa
	   .|..aa.
	 */

	if (isCrowd)
		vuln = (vuln * (2 + open_neighbors)) / 10;

	vuln += t.distance() * (isFast ? 100 : 10);

	return vuln;
}
/*
void Fight::checkPanic() {
	int threat = 0;
	int threat_stalkers = 0;
	int threat_eignorers = 0;
	int threat_single = 0;
	int threat_fast = 0;

	// loop over mobs

	// generate flags

	int max_vulnerability = saiph_power * 1000 / threat;

	if (pointVulnerability(Saiph::position(), ...) < max_vulnerability)
		return;

	// we're scared, find the closest safe enough point and go there
}
*/
