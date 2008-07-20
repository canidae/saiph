#include "MonsterTracker.h"

/* constructors */
MonsterTracker::MonsterTracker(Saiph *saiph) : saiph(saiph) {
	/* set which monsters we track */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || (a >= '1' && a <= '5')  || a == '&' || a == '\'' || a == ':' || a == ';' || a == '~' || a == PET)
			monster[a] = true;
		else    
			monster[a] = false;
	}
}

/* methods */
void MonsterTracker::removeMonsters() {
	/* when we're standing next to where a monster used to be
	 * we'll remove the monster if we can't see it */
	for (map<Point, Monster>::iterator m = monsters[saiph->position.branch][saiph->position.level].begin(); m != monsters[saiph->position.branch][saiph->position.level].end(); ) {
		if (abs(saiph->position.row - m->second.row) > 1 || abs(saiph->position.col - m->second.col) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		unsigned char symbol = saiph->world->view[m->second.row][m->second.col];
		if (saiph->world->color[m->second.row][m->second.col] == INVERSE)
			symbol = PET; // pets are a bit unique
		if (symbol == m->second.symbol && saiph->world->color[m->second.row][m->second.col] == m->second.color) {
			/* we can still see the monster */
			++m;
			continue;
		}
		/* remove monster from monstermap */
		saiph->map[saiph->position.branch][saiph->position.level].monster[m->second.row][m->second.col] = ILLEGAL_MONSTER;
		/* remove monster from list */
		monsters[saiph->position.branch][saiph->position.level].erase(m++);
	}
}

void MonsterTracker::updateMonster(const Point &point) {
	/* add a monster, or update position of an existing monster */
	unsigned char symbol = saiph->world->view[point.row][point.col];
	int color = saiph->world->color[point.row][point.col];
	if (color == INVERSE)
		symbol = PET; // pets are a bit unique
	/* set monster on monstermap */
	saiph->map[saiph->position.branch][saiph->position.level].monster[point.row][point.col] = symbol;
	/* find nearest monster and update position */
	int best_distance = INT_MAX;
	Monster *best_monster = NULL;
	for (map<Point, Monster>::iterator m = monsters[saiph->position.branch][saiph->position.level].begin(); m != monsters[saiph->position.branch][saiph->position.level].end(); ++m) {
		if (m->second.symbol != symbol || m->second.color != color)
			continue; // not the same monster
		if (saiph->world->view[m->second.row][m->second.col] == m->second.symbol && saiph->world->color[m->second.row][m->second.col] == m->second.color)
			continue; // this monster already is on its square
		/* see if this monster is closer than the last found monster */
		int distance = max(abs(m->second.row - point.row), abs(m->second.col - point.col));
		if (distance >= best_distance)
			continue;
		/* it is */
		best_distance = distance;
		best_monster = &m->second;
	}
	if (best_monster != NULL) {
		/* remove monster from monstermap */
		saiph->map[saiph->position.branch][saiph->position.level].monster[best_monster->row][best_monster->col] = ILLEGAL_MONSTER;
		/* update monster */
		best_monster->row = point.row;
		best_monster->col = point.col;
		return;
	}
	/* add monster */
	monsters[saiph->position.branch][saiph->position.level][point] = Monster(point, symbol, color);
}
