#include <stdlib.h>
#include "Fight.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Fight::Fight(Saiph *saiph) : Analyzer("Fight"), saiph(saiph) {
}

/* methods */
void Fight::analyze() {
	/* if engulfed try to fight our way out */
	if (saiph->engulfed) {
		clearCommands();
		setCommand(0, FIGHT_ATTACK_PRIORITY, string(1, MOVE_NW));
		sequence = 0;
		return;
	}
	/* fight monsters */
	unsigned char got_thrown = FIGHT_NOT_CHECKED_THROWN_WEAPONS;
	int min_distance = INT_MAX;
	int min_moves = INT_MAX;
	map<Point, Monster>::iterator best_monster = saiph->levels[saiph->position.level].monsters.end();
	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (m->second.symbol == PET)
			continue; // we're not fighting pets :)
		if (m->second.attitude == FRIENDLY)
			continue; // don't attack friendlies
		int distance = max(abs(m->first.row - saiph->position.row), abs(m->first.col - saiph->position.col));
		if (distance > min_distance)
			continue; // we'll always attack nearest monster
		bool blue_e = (m->second.symbol == 'e' && m->second.color == BLUE);
		if (((!blue_e && distance > 1) || (blue_e && distance == 1)) && m->second.visible && distance <= saiph->world->player.strength / 2) {
			/* monster is within throw distance, or it's a blue 'e' and distance is 1 */
			unsigned char in_line = saiph->directLine(m->first, false, true);
			if (in_line != ILLEGAL_MOVE) {
				/* we got a direct line to the monster */
				if (got_thrown == FIGHT_NOT_CHECKED_THROWN_WEAPONS)
					got_thrown = gotThrown();
				if (got_thrown != FIGHT_NO_THROWN_WEAPONS) {
					/* got thrown weapons */
					if (commands[0].priority == FIGHT_ATTACK_PRIORITY && distance >= min_distance && m->second.symbol != '@' && m->second.symbol != 'A')
						continue; // already got a target
					clearCommands();
					setCommand(0, FIGHT_ATTACK_PRIORITY, THROW);
					setCommand(1, PRIORITY_CONTINUE_ACTION, string(1, got_thrown));
					setCommand(2, PRIORITY_CONTINUE_ACTION, string(1, in_line));
					sequence = 0;
					min_distance = distance;
					continue;
				}
			}
		}
		/* we couldn't throw something at the monster, try moving to or melee it */
		int moves = 0;
		unsigned char move = saiph->shortestPath(m->first, true, &moves);
		if (move == ILLEGAL_MOVE)
			continue; // unable to path to monster
		if (moves > 1 && sequence >= 0 && commands[sequence].priority > FIGHT_MOVE_PRIORITY)
			continue; // we must move to monster, but we got something else with higher priority
		if (moves > min_moves)
			continue; // we know of a monster closer than this one
		if (moves == 1 && distance == min_distance && sequence >= 0 && commands[sequence].priority == FIGHT_ATTACK_PRIORITY && m->second.symbol != '@' && m->second.symbol != 'A')
			continue; // already got a target
		if (blue_e) {
			clearCommands();
			setCommand(0, FIGHT_BLUE_E_PRIORITY, string(1, move));
		} else {
			clearCommands();
			setCommand(0, (moves == 1) ? FIGHT_ATTACK_PRIORITY : FIGHT_MOVE_PRIORITY, string(1, move));
		}
		sequence = 0;
		min_distance = distance;
		min_moves = moves;
	}
}

void Fight::parseMessages(const string &messages) {
	if (sequence == 0 && saiph->world->question && messages.find(MESSAGE_WHAT_TO_THROW, 0) != string::npos) {
		++sequence;
	} else if (sequence == 1 && saiph->world->question && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		++sequence;
		/* make inventory dirty, we just threw something */
		req.request = REQUEST_DIRTY_INVENTORY;                                                                                                 
		saiph->request(req);
	} else if (saiph->world->question && messages.find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		/* this may happen after we've said which direction to attack.
		 * answer YES for now */
		setCommand(0, PRIORITY_CONTINUE_ACTION, YES);
		sequence = 0;
	}
}

bool Fight::request(const Request &request) {
	if (request.request == REQUEST_ADD_THROWN_WEAPON) {
		thrown.push_back(request.data);
		return true;
	}
	return false;
}

/* private methods */
unsigned char Fight::gotThrown() {
	/* return best weapon we can throw, if any */
	for (vector<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name == *t)
				return i->first;
		}
	}
	return FIGHT_NO_THROWN_WEAPONS;
}
