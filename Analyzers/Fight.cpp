#include "Fight.h"

/* constructors */
Fight::Fight(Saiph *saiph) : Analyzer("Fight"), saiph(saiph), action("") {
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
	/* fight nearest monster */
	int b = saiph->current_branch;
	int l = saiph->current_level;
	int best_distance = INT_MAX;
	unsigned char best_move = ILLEGAL_MOVE;
	unsigned char best_symbol = ILLEGAL_MONSTER;
	int best_color = NO_COLOR;
	for (list<Monster>::iterator m = saiph->monstertracker->monsters[b][l].begin(); m!= saiph->monstertracker->monsters[b][l].end(); ++m) {
		if (m->symbol == PET)
			continue; // we're not fighting pets :)
		int cur_priority;
		if (m->symbol == 'e' && m->color == BLUE)
			cur_priority = FIGHT_ATTACK_BLUE_E;
		else if (m->symbol == '@' && m->color == WHITE)
			cur_priority = FIGHT_ATTACK_WHITE_AT;
		else if (m->symbol == '@' && m->color == BLUE)
			cur_priority = FIGHT_ATTACK_BLUE_AT;
		else
			cur_priority = FIGHT_ATTACK_MONSTER;
		if (cur_priority < priority)
			continue; // we've already found another monster with higher priority
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(*m, true, &distance, &straight_line);
		if (move == ILLEGAL_MOVE)
			continue; // can't path to this monster
		if (distance >= best_distance)
			continue; // we know of a monster closer to us
		priority = cur_priority;
		best_distance = distance;
		best_move = move;
		best_symbol = m->symbol;
		best_color = m->color;
	}
	action = best_move;
}

void Fight::parseMessages(const string &messages) {
	if (messages.find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		action = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}
