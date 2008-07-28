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
	thrown.push_back("poisoned dart");
	thrown.push_back("poisoned darts");
	thrown.push_back("dart");
	thrown.push_back("darts");
}

/* methods */
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
		req.priority = FIGHT_PICKUP_PRIORITY;
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
	int fewest_moves = INT_MAX;
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
		int distance = max(abs(m->first.row - saiph->position.row), abs(m->first.col - saiph->position.col));
		if (distance <= saiph->world->player.strength / 2) {
			/* we're within throwing distance */
		}
		int moves = -1;
		unsigned char move = saiph->shortestPath(m->first, true, &moves);
		if (move == ILLEGAL_MOVE)
			continue; // can't path to this monster
		int cur_priority;
		bool direct_line = false;
		if (got_thrown == 0 && m->second.symbol == 'e' && m->second.color == BLUE) {
			/* only fight blue e when we can throw stuff at it or when we're cornered */
			cur_priority = FIGHT_BLUE_E_PRIORITY;
		} else if (m->second.symbol == '@' && m->second.color == BLUE) {
			/* don't attack blue @ for now */
			cur_priority = FIGHT_BLUE_AT_PRIORITY;
		} else if (m->second.symbol == '@' && m->second.color == WHITE) {
			/* don't attack white @ for now */
			cur_priority = FIGHT_WHITE_AT_PRIORITY;
		} else if (got_thrown && distance <= saiph->world->player.strength / 2) {
			unsigned char direct_move = saiph->directLine(m->first, false);
			if (direct_move != ILLEGAL_MOVE) {
				/* seemingly we can throw stuff at the enemy */
				direct_line = true;
				cur_priority = FIGHT_ATTACK_PRIORITY;
				move = direct_move; // or we'll throw in wild direction while standing in doorways, aiming diagonally
				moves = distance; // or we might try to pass somewhere we cannot
			} else {
				cur_priority = (moves == 1 ? FIGHT_ATTACK_PRIORITY : FIGHT_MOVE_PRIORITY);
			}
		} else {
			cur_priority = (moves == 1 ? FIGHT_ATTACK_PRIORITY : FIGHT_MOVE_PRIORITY);
		}
		saiph->debugfile << "[Fight      ] Monster: " << got_thrown << ", " << enemy_in_line << ", " << direct_line << ", " << m->second.visible << ", " << moves << ", " << m->second.symbol << ", " << m->second.color << ", " << cur_priority << endl;
		if (cur_priority < priority) {
			continue; // we've already found another monster with higher priority
		} else if (enemy_in_line && !direct_line) {
			continue; // we got a monster we can throw at, and we can't throw at this monster
		} else if ((!enemy_in_line && !direct_line) || (enemy_in_line && direct_line)) {
			/* either we can't throw at neither monster, or we can throw at them both */
			if (moves > fewest_moves)
				continue; // best_monster is closer
			else if (moves == fewest_moves && m->second.symbol != '@' && m->second.symbol != 'A')
				continue; // monster is just as far away, and it's not a '@' or 'A'
		}
		enemy_in_line = direct_line;
		priority = cur_priority;
		fewest_moves = moves;
		best_move = move;
		best_monster = m;
	}
	if (best_monster == saiph->monsters[saiph->position.branch][saiph->position.level].end())
		return;
	saiph->debugfile << "[Fight      ] " << got_thrown << ", " << enemy_in_line << ", " << best_monster->second.visible << ", " << fewest_moves << ", " << best_monster->second.symbol << ", " << best_monster->second.color << endl;
	if (got_thrown != 0 && enemy_in_line && best_monster->second.visible && fewest_moves <= saiph->world->player.strength / 2 && (fewest_moves > 1 || (best_monster->second.symbol == 'e' && best_monster->second.color == BLUE))) {
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
