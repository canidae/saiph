#include "Fight.h"

/* constructors */
Fight::Fight(Saiph *saiph) : Analyzer("Fight"), saiph(saiph) {
}

/* methods */
void Fight::analyze() {
	if (FIGHT_ATTACK_PRIORITY < saiph->best_priority)
		return;
	/* if engulfed try to fight our way out */
	if (saiph->engulfed) {
		command = MOVE_NW; // doesn't matter which direction
		priority = FIGHT_ATTACK_PRIORITY;
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
					if (priority == FIGHT_ATTACK_PRIORITY && distance >= min_distance && m->second.symbol != '@' && m->second.symbol != 'A')
						continue; // already got a target
					priority = FIGHT_ATTACK_PRIORITY;
					min_distance = distance;
					command = THROW;
					command2 = got_thrown;
					command3 = in_line;
					continue;
				}
			}
		}
		/* we couldn't throw something at the monster, try moving to or melee it */
		int moves = 0;
		unsigned char move = saiph->shortestPath(m->first, true, &moves);
		if (move == ILLEGAL_MOVE)
			continue; // unable to path to monster
		if (moves > 1 && priority > FIGHT_MOVE_PRIORITY)
			continue; // we must move to monster, but we got something else with higher priority
		if (moves > min_moves)
			continue; // we know of a monster closer than this one
		if (moves == 1 && distance == min_distance && priority == FIGHT_ATTACK_PRIORITY && m->second.symbol != '@' && m->second.symbol != 'A')
			continue; // already got a target
		if (blue_e)
			priority = FIGHT_BLUE_E_PRIORITY;
		else
			priority = (moves == 1) ? FIGHT_ATTACK_PRIORITY : FIGHT_MOVE_PRIORITY;
		min_distance = distance;
		min_moves = moves;
		command = move;
	}
}

void Fight::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && !command3.empty() && messages.find(MESSAGE_WHAT_TO_THROW, 0) != string::npos) {
		command = command2;
		command2 = command3;
		command3 = THROW;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && command3 == THROW && !command2.empty() && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		command = command2;
		command2.clear();
		command3.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* make inventory dirty, we just threw something */
		req.request = REQUEST_DIRTY_INVENTORY;                                                                                                 
		saiph->request(req);
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
