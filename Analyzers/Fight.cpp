#include "Fight.h"

/* constructors */
Fight::Fight(Saiph *saiph) : Analyzer("Fight"), saiph(saiph) {
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
	thrown.push_back("dart");
	thrown.push_back("darts");
}

/* methods */
void Fight::complete() {
}

void Fight::finish() {
	command = "";
	/* if engulfed try to fight our way out */
	if (saiph->engulfed) {
		command = MOVE_NW; // doesn't matter which direction
		priority = FIGHT_ATTACK_PRIORITY;
		return;
	}
	/* look for thrown weapons on ground */
	if (saiph->on_ground != NULL) {
		/* there are items here, we should look for weapons */
		req.request = REQUEST_LOOT_STASH;
		req.priority = FIGHT_LOOT_PRIORITY;
		req.coordinate = saiph->position;
		bool die = false;
		for (list<Item>::iterator i = saiph->on_ground->items.begin(); !die && i != saiph->on_ground->items.end(); ++i) {
			for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
				if (i->name == *t) {
					/* request that someone loot this stash */
					saiph->request(req);
					die = true;
					break;
				}
			}
		}
	}
	/* fight nearest monster */
	int best_distance = INT_MAX;
	unsigned char best_move = ILLEGAL_MOVE;
	map<Point, Monster>::iterator best_monster = saiph->monsters[saiph->position.branch][saiph->position.level].end();
	bool enemy_in_line = false;
	unsigned char got_thrown = 0;
	req.request = REQUEST_UPDATED_INVENTORY;
	req.priority = PRIORITY_LOOK;
	if (saiph->request(req)) {
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); got_thrown == 0 && i != saiph->inventory.end(); ++i) {
			for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
				if (i->second.name == *t) {
					got_thrown = i->first;
					break;
				}
			}
		}
	}
	for (map<Point, Monster>::iterator m = saiph->monsters[saiph->position.branch][saiph->position.level].begin(); m != saiph->monsters[saiph->position.branch][saiph->position.level].end(); ++m) {
		if (m->second.symbol == PET)
			continue; // we're not fighting pets :)
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(m->first, true, &distance, &straight_line);
		if (move == ILLEGAL_MOVE)
			continue; // can't path to this monster
		int cur_priority;
		if (got_thrown == 0 && m->second.symbol == 'e' && m->second.color == BLUE)
			cur_priority = FIGHT_BLUE_E_PRIORITY;
		else if (m->second.symbol == '@' && m->second.color == BLUE)
			cur_priority = FIGHT_BLUE_AT_PRIORITY;
		else if (m->second.symbol == '@' && m->second.color == WHITE)
			cur_priority = FIGHT_WHITE_AT_PRIORITY;
		else
			cur_priority = (distance == 1 ? FIGHT_ATTACK_PRIORITY : FIGHT_MOVE_PRIORITY);
		if (cur_priority < priority)
			continue; // we've already found another monster with higher priority
		if (got_thrown > 0 && enemy_in_line && !straight_line)
			continue; // got thrown and an enemy in line already, this enemy isn't or we got no thrown
		if (distance > best_distance)
			continue; // we know of a monster closer to us
		if (distance == best_distance && m->second.symbol != '@' && m->second.symbol != 'A')
			continue; // equally close, and it's not '@' or 'A' (we attack those first as they won't respect elbereth)
		enemy_in_line = straight_line;
		priority = cur_priority;
		best_distance = distance;
		best_move = move;
		best_monster = m;
	}
	if (best_monster == saiph->monsters[saiph->position.branch][saiph->position.level].end())
		return;
	saiph->debugfile << "[Fight      ] " << got_thrown << ", " << enemy_in_line << ", " << best_monster->second.visible << ", " << best_distance << ", " << best_monster->second.symbol << ", " << best_monster->second.color << endl;
	if (got_thrown != 0 && enemy_in_line && best_monster->second.visible && (best_distance > 1 || (best_monster->second.symbol == 'e' && best_monster->second.color == BLUE))) {
		/* throw */
		command = THROW;
		command2 = got_thrown;
		command3 = best_move;
	} else {
		/* move or melee */
		command = best_move;
	}
}

void Fight::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && command3 != "" && messages.find(MESSAGE_WHAT_TO_THROW, 0) != string::npos) {
		command = command2;
		command2 = command3;
		command3 = "";
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && command2 != "" && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		command = command2;
		command2 = "";
		priority = PRIORITY_CONTINUE_ACTION;
		/* make inventory dirty, we just threw something */
		req.request = REQUEST_DIRTY_INVENTORY;                                                                                                 
		saiph->request(req);
	} else if (saiph->world->menu && messages.find(MESSAGE_PICK_UP_WHAT, 0) != string::npos) {
		/* pick up thrown weapons if any */
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
				if (p->second.name == *t) {
					/* pick it up :) */
					command = p->first;
					priority = PRIORITY_PICKUP_ITEM;
					continue;
				}
			}
		}
	}
}
