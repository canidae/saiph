#include "Fight.h"

/* constructors */
Fight::Fight(Saiph *saiph) : Analyzer("Fight"), saiph(saiph), action("") {
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
void Fight::command(string *command) {
	*command = action;
}

void Fight::finish() {
	action = "";
	/* if engulfed try to fight our way out */
	if (saiph->engulfed) {
		action = MOVE_NW; // doesn't matter which direction
		priority = FIGHT_ATTACK_MONSTER;
		return;
	}
	/* look for thrown weapons on ground */
	if (saiph->itemtracker->on_ground != NULL) {
		/* there are items here, we should look for weapons */
		req.request = REQUEST_LOOT_STASH;
		req.priority = FIGHT_LOOT_PRIORITY;
		req.coordinate = saiph->position;
		bool die = false;
		for (list<Item>::iterator i = saiph->itemtracker->on_ground->items.begin(); !die && i != saiph->itemtracker->on_ground->items.end(); ++i) {
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
	unsigned char best_symbol = ILLEGAL_MONSTER;
	bool enemy_in_line = false;
	int best_color = NO_COLOR;
	unsigned char got_thrown = 0;
	for (map<unsigned char, Item>::iterator i = saiph->itemtracker->inventory.begin(); got_thrown == 0 && i != saiph->itemtracker->inventory.end(); ++i) {
		for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
			if (i->second.name == *t) {
				got_thrown = i->first;
				break;
			}
		}
	}
	for (map<Point, Monster>::iterator m = saiph->monstertracker->monsters[saiph->position.branch][saiph->position.level].begin(); m!= saiph->monstertracker->monsters[saiph->position.branch][saiph->position.level].end(); ++m) {
		if (m->second.symbol == PET)
			continue; // we're not fighting pets :)
		int cur_priority;
		if (got_thrown == 0 && m->second.symbol == 'e' && m->second.color == BLUE)
			cur_priority = FIGHT_ATTACK_BLUE_E;
		else if (m->second.symbol == '@' && m->second.color == WHITE)
			cur_priority = FIGHT_ATTACK_WHITE_AT;
		else if (m->second.symbol == '@' && m->second.color == BLUE)
			cur_priority = FIGHT_ATTACK_BLUE_AT;
		else
			cur_priority = FIGHT_ATTACK_MONSTER;
		if (cur_priority < priority)
			continue; // we've already found another monster with higher priority
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(m->first, true, &distance, &straight_line);
		if (move == ILLEGAL_MOVE)
			continue; // can't path to this monster
		if (got_thrown == 0 || enemy_in_line && !straight_line)
			continue; // got another enemy in line already, this enemy isn't
		if (distance > best_distance)
			continue; // we know of a monster closer to us
		if (distance == best_distance && m->second.symbol != '@' && m->second.symbol != 'A')
			continue; // equally close, and it's not '@' or 'A' (we attack those first as they won't respect elbereth)
		enemy_in_line = straight_line;
		priority = cur_priority;
		best_distance = distance;
		best_move = move;
		best_symbol = m->second.symbol;
		best_color = m->second.color;
	}
	if (got_thrown != 0 && enemy_in_line && (best_distance > 1 || (best_symbol == 'e' && best_color == BLUE))) {
		/* throw */
		action = THROW;
		action2 = got_thrown;
		action3 = best_move;
	} else {
		/* move or melee */
		action = best_move;
	}
}

void Fight::parseMessages(const string &messages) {
	if (messages.find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		action = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->menu && messages.find(MESSAGE_PICK_UP_WHAT, 0) != string::npos) {
		/* pick up thrown weapons if any */
		for (map<unsigned char, Item>::iterator p = saiph->itemtracker->pickup.begin(); p != saiph->itemtracker->pickup.end(); ++p) {
			for (list<string>::iterator t = thrown.begin(); t != thrown.end(); ++t) {
				if (p->second.name == *t) {
					/* pick it up :) */
					action = p->first;
					priority = PRIORITY_PICKUP_ITEM;
					continue;
				}
			}
		}
	}
}
