#include "Fight.h"

/* constructors */
Fight::Fight(Saiph *saiph) : Analyzer("Fight"), saiph(saiph) {
	/* thrown weapons */
	thrown.push_back("dwarvish spear");
	thrown.push_back("dwarvish spears");
	thrown.push_back("silver spear");
	thrown.push_back("silver spears");
	thrown.push_back("elven spear");
	thrown.push_back("elven spears");
	thrown.push_back("spear");
	thrown.push_back("spears");
	thrown.push_back("orcish spear");
	thrown.push_back("orcish spears");
	thrown.push_back("javelin");
	thrown.push_back("javelins");
	thrown.push_back("silver dagger");
	thrown.push_back("silver daggers");
	thrown.push_back("elven dagger");
	thrown.push_back("elven daggers");
	thrown.push_back("dagger");
	thrown.push_back("daggers");
	thrown.push_back("orcish dagger");
	thrown.push_back("orcish daggers");
	//thrown.push_back("poisoned dart"); // damages our alignment
	//thrown.push_back("poisoned darts"); // ^^
	thrown.push_back("dart");
	thrown.push_back("darts");
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
		else if (m->second.symbol == '@' && m->second.color == BLUE)
			continue; // don't attack blue @ for now
		else if (m->second.symbol == '@' && m->second.color == WHITE)
			continue; // don't attack white @ for now
		int distance = max(abs(m->first.row - saiph->position.row), abs(m->first.col - saiph->position.col));
		if (distance > min_distance)
			continue; // we'll always attack nearest monster
		bool blue_e = (m->second.symbol == 'e' && m->second.color == BLUE);
		if ((distance > 1 || blue_e) && m->second.visible && distance <= saiph->world->player.strength / 2) {
			/* monster is within throw distance, or it's a blue 'e' */
			unsigned char in_line = saiph->directLine(m->first, false);
			if (in_line != ILLEGAL_MOVE) {
				/* we got a direct line to the monster */
				if (got_thrown == FIGHT_NOT_CHECKED_THROWN_WEAPONS)
					got_thrown = gotThrown();
				if (got_thrown != FIGHT_NO_THROWN_WEAPONS) {
					/* got thrown weapons */
					if (priority == FIGHT_ATTACK_PRIORITY && m->second.symbol != '@' && m->second.symbol != 'A')
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
		if (moves == 1 && priority == FIGHT_ATTACK_PRIORITY && m->second.symbol != '@' && m->second.symbol != 'A')
			continue; // already got a target
		if (blue_e)
			priority = FIGHT_BLUE_E_PRIORITY;
		else
			priority = (moves == 1) ? FIGHT_ATTACK_PRIORITY : FIGHT_MOVE_PRIORITY;
		min_distance = distance;
		min_moves = moves;
		command = move;
	}

	/* look for thrown weapons on ground */
	if (FIGHT_PICKUP_PRIORITY < saiph->best_priority)
		return;
	if (saiph->on_ground != NULL) {
		/* there are items here, we should look for weapons */
		req.request = REQUEST_LOOT_STASH;
		req.priority = FIGHT_PICKUP_PRIORITY;
		for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
			for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
				if (i->name == *t && i->beatitude != CURSED) {
					/* request that someone loot this stash */
					saiph->request(req);
					return;
				}
			}
		}
	}
}

void Fight::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && command3 != "" && messages.find(MESSAGE_WHAT_TO_THROW, 0) != string::npos) {
		command = command2;
		command2 = command3;
		command3.clear();
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && command2 != "" && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* make inventory dirty, we just threw something */
		req.request = REQUEST_DIRTY_INVENTORY;                                                                                                 
		saiph->request(req);
	} else if (saiph->pickup.size() > 0) {
		/* pick up thrown weapons if any */
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
				if (p->second.name == *t && p->second.beatitude != CURSED) {
					/* pick it up :) */
					command = p->first;
					priority = PRIORITY_SELECT_ITEM;
					return;
				}
			}
		}
	}
}

/* private methods */
unsigned char Fight::gotThrown() {
	/* return best weapon we can throw, if any */
	req.request = REQUEST_UPDATED_INVENTORY;
	req.priority = PRIORITY_LOOK;
	if (saiph->request(req)) {
		for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
			for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
				if (i->second.name == *t)
					return i->first;
			}
		}
	}
	return FIGHT_NO_THROWN_WEAPONS;
}
