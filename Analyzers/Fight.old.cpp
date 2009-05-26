#include <stdlib.h>
#include "Fight.h"
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Data/MonsterData.h"

using namespace action;
using namespace analyzer;
using namespace data;
using namespace event;
using namespace std;

/* constructors/destructor */
Fight::Fight(Saiph *saiph) : Analyzer("Fight"), saiph(saiph) {
}

/* methods */
void Fight::analyze() {
	/* if engulfed try to fight our way out */
	if (World::engulfed) {
		World::setAction(new Fight(NW, PRIORITY_FIGHT_ENGULFED));
		return;
	}
	/* fight monsters */
	int most_dangerous = INT_MIN;
	map<Point, Monster>::iterator best_monster = World::levels[Saiph::position.level].monsters.end();
	for (map<Point, Monster>::iterator m = World::levels[Saiph::position.level].monsters.begin(); m != World::levels[Saiph::position.level].monsters.end(); ++m) {
		if (m->second.symbol == PET)
			continue; // we're not fighting pets :)
		else if (m->second.attitude == FRIENDLY)
			continue; // don't attack friendlies
		else if (m->second.symbol == 'u' && ((m->second.color == BOLD_WHITE && Saiph::alignment == LAWFUL) || (m->second.color == WHITE && Saiph::alignment == NEUTRAL) || (m->second.color == BLUE && Saiph::alignment == CHAOTIC)))
			continue; // don't attack unicorns of same alignment
		if (m->second.data == NULL) {
			/* this shouldn't happen, MonsterInfo should make sure we got monster data */
			most_dangerous = INT_MAX;
			best_monster = m;
			break;
		}
	}

	int min_distance = INT_MAX;
	unsigned int min_moves = UNREACHABLE;
	Monster *target = NULL;
	map<Point, Monster>::iterator best_monster = saiph->levels[saiph->position.level].monsters.end();
	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (m->second.symbol == PET)
			continue; // we're not fighting pets :)
		else if (m->second.attitude == FRIENDLY)
			continue; // don't attack friendlies
		else if (m->second.symbol == 'u' && ((m->second.color == BOLD_WHITE && saiph->world->player.alignment == LAWFUL) || (m->second.color == WHITE && saiph->world->player.alignment == NEUTRAL) || (m->second.color == BLUE && saiph->world->player.alignment == CHAOTIC)))
			continue; // don't attack unicorns of same alignment
		int distance = max(abs(m->first.row - saiph->position.row), abs(m->first.col - saiph->position.col));
		bool blue_e = (m->second.symbol == 'e' && m->second.color == BLUE);
		if (((!blue_e && distance > 1) || (blue_e && distance == 1)) && m->second.visible && distance <= saiph->world->player.strength / 2) {
			/* monster is within throw distance, or it's a blue 'e' and distance is 1 */
			unsigned char in_line = saiph->directLine(m->first, false, true);
			if (in_line != ILLEGAL_DIRECTION) {
				/* we got a direct line to the monster */
				if (got_thrown == FIGHT_NOT_CHECKED_THROWN_WEAPONS)
					got_thrown = gotThrown();
				if (got_thrown != FIGHT_NO_THROWN_WEAPONS && saiph->world->player.encumbrance <= BURDENED) {
					/* got thrown weapons */
					if (priority == PRIORITY_FIGHT_ATTACK && distance >= min_distance && m->second.symbol != '@' && m->second.symbol != 'A' && !moreDangerousThan(&m->second, target))
						continue; // already got a target
					priority = PRIORITY_FIGHT_ATTACK;
					target = &m->second;
					min_distance = distance;
					command = THROW;
					command2 = got_thrown;
					command3 = in_line;
					continue;
				}
			}
		}
		/* we couldn't throw something at the monster, try moving to or melee it */
		const PathNode &node = saiph->shortestPath(m->first);
		if (node.cost == UNREACHABLE)
			continue; // unable to path to monster
		else if (node.moves > 1 && (priority > PRIORITY_FIGHT_MOVE || saiph->world->player.blind))
			continue; // we must move to monster, but we got something else with higher priority or are blind
		else if (node.moves > min_moves)
			continue; // we know of a monster closer than this one
		else if (node.moves == 1 && distance == min_distance && priority == PRIORITY_FIGHT_ATTACK && m->second.symbol != '@' && m->second.symbol != 'A' && !moreDangerousThan(&m->second, target))
			continue; // already got a target
		else if (blue_e)
			priority = PRIORITY_FIGHT_MELEE_BLUE_E;
		else
			priority = (node.moves == 1) ? PRIORITY_FIGHT_ATTACK : PRIORITY_FIGHT_MOVE;
		target = &m->second;
		min_distance = distance;
		min_moves = node.moves;
		command = (node.moves == 1 ? FIGHT : ""); // always fight using F when distance is 1
		command.push_back(node.dir);
		Debug::info(saiph->last_turn) << FIGHT_DEBUG_NAME << "Fighting " << m->second.symbol << " " << m->first << ": " << "dist: " << distance << ", command: " << command << ", pri: " << priority << ", attitude: " << m->second.attitude << endl;
	}
}

void Fight::onEvent(Event *const event) {
	if (event->getID() == ChangedInventoryItems::id) {
		ChangedInventoryItems *e = (ChangedInventoryItems *) event;
		for (set<unsigned char>::iterator k = event.keys.begin(); k != event.keys.end(); ++k) {
			map<unsigned char, Item>::iterator i = Inventory::items.find(*k);
			if (i == Inventory::items.end()) {
				/* we lost this item, remove it from thrown */
				thrown.erase(*k);
			} else {
				/* this item is new or changed.
				 * if we intend to throw it, add it to thrown.
				 * otherwise remove it from thrown */
				/* FIXME: figure out what to throw depending on class */
				/* FIXME 2: looking up the lists each time is expencive, create a single map instead */
				if (Dagger::daggers.find(i->second.name) != Dagger::daggers.end() || Spear::spears.find(i->second.name) != Spear::spears.end() || Dart::darts.find(i->second.name) != Dart::darts.end()) {
					/* add to thrown */
					thrown.insert(*k);
				} else {
					/* remove from thrown */
					thrown.erase(*k);
				}
			}
		}
	}
}
