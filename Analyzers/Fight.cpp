#include "Fight.h"

/* constructors */
Fight::Fight(Saiph *saiph) : saiph(saiph), action("") {
}

/* methods */
void Fight::command(string *command) {
	*command = action;
}

int Fight::finish() {
	action = "";
	/* if engulfed try to fight our way out */
	if (saiph->engulfed) {
		action = MOVE_NW; // doesn't matter which direction
		return FIGHT_ATTACK_MONSTER;
	}
	/* fight nearest monster */
	int b = saiph->current_branch;
	int l = saiph->current_level;
	int best_distance = INT_MAX;
	int best_priority = -1;
	unsigned char best_move = ILLEGAL_MOVE;
	unsigned char best_symbol = ILLEGAL_MONSTER;
	int best_color = NO_COLOR;
	for (list<Monster>::iterator m = saiph->monstertracker->monsters[b][l].begin(); m!= saiph->monstertracker->monsters[b][l].end(); ++m) {
		if (m->symbol == PET)
			continue; // we're not fighting pets :)
		int priority;
		if (best_symbol == 'e' && best_color == BLUE)
			priority = FIGHT_ATTACK_BLUE_E;
		else if (best_symbol == '@' && best_color == WHITE)
			priority = FIGHT_ATTACK_WHITE_AT;
		else
			priority = FIGHT_ATTACK_MONSTER;
		if (priority < best_priority)
			continue; // we've already found another monster with higher priority
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(*m, true, &distance, &straight_line);
		if (move == ILLEGAL_MOVE)
			continue; // can't path to this monster
		if (distance >= best_distance)
			continue; // we know of a monster closer to us
		best_priority = priority;
		best_distance = distance;
		best_move = move;
		best_symbol = m->symbol;
		best_color = m->color;
	}
	action = best_move;
	return best_priority;
}

int Fight::parseMessages(string *messages) {
	if (messages->find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		action = YES;
		return FIGHT_CONTINUE_ACTION;
	}
	return 0;
}
