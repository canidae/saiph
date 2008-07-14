#include "MonsterTracker.h"

/* constructors */
MonsterTracker::MonsterTracker(Saiph *saiph) : saiph(saiph) {
}

/* methods */
void MonsterTracker::removeMonsters() {
	/* when we're standing next to where a monster used to be
	 * we'll remove the monster if we can't see it */
	int b = saiph->current_branch;
	int l = saiph->current_level;
	for (list<Monster>::iterator m = monsters[b][l].begin(); m != monsters[b][l].end(); ) {
		if (abs(saiph->world->player.row - m->row) > 1 || abs(saiph->world->player.col - m->col) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		unsigned char symbol = saiph->world->view[m->row][m->col];
		if (saiph->world->color[m->row][m->col] == INVERSE)
			symbol = PET; // pets are a bit unique
		if (symbol == m->symbol && saiph->world->color[m->row][m->col] == m->color) {
			/* we can still see the monster */
			++m;
			continue;
		}
		/* remove monster from monstermap */
		saiph->map[b][l].monster[m->row][m->col] = ILLEGAL_MONSTER;
		/* remove monster from list */
		m = monsters[b][l].erase(m);
	}
}

void MonsterTracker::updateMonster(const Point &point) {
	/* add a monster, or update position of an existing monster */
	int b = saiph->current_branch;
	int l = saiph->current_level;
	unsigned char symbol = saiph->world->view[point.row][point.col];
	int color = saiph->world->color[point.row][point.col];
	if (color == INVERSE)
		symbol = PET; // pets are a bit unique
	/* set monster on monstermap */
	saiph->map[b][l].monster[point.row][point.col] = symbol;
	/* find nearest monster and update position */
	int best_distance = INT_MAX;
	Monster *best_monster = NULL;
	for (list<Monster>::iterator m = monsters[b][l].begin(); m != monsters[b][l].end(); ++m) {
		if (m->symbol != symbol || m->color != color)
			continue; // not the same monster
		if (saiph->world->view[m->row][m->col] == m->symbol && saiph->world->color[m->row][m->col] == m->color)
			continue; // this monster already is on its square
		/* see if this monster is closer than the last found monster */
		int distance = max(abs(m->row - point.row), abs(m->col - point.col));
		if (distance >= best_distance)
			continue;
		/* it is */
		best_distance = distance;
		best_monster = &(*m);
	}
	if (best_monster != NULL) {
		/* remove monster from monstermap */
		saiph->map[b][l].monster[best_monster->row][best_monster->col] = ILLEGAL_MONSTER;
		/* update monster */
		best_monster->row = point.row;
		best_monster->col = point.col;
		return;
	}
	/* add monster */
	monsters[b][l].push_back(Monster(point, symbol, color));
}
