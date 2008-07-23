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
		if (abs(saiph->position.row - m->first.row) > 1 || abs(saiph->position.col - m->first.col) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		unsigned char symbol = saiph->world->view[m->first.row][m->first.col];
		if (saiph->world->color[m->first.row][m->first.col] == INVERSE)
			symbol = PET; // pets are a bit unique
		if (symbol == m->second.symbol && saiph->world->color[m->first.row][m->first.col] == m->second.color) {
			/* we can still see the monster */
			++m;
			continue;
		}
		/* remove monster from monstermap */
		saiph->map[saiph->position.branch][saiph->position.level].monster[m->first.row][m->first.col] = ILLEGAL_MONSTER;
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
	map<Point, Monster>::iterator best_monster = monsters[saiph->position.branch][saiph->position.level].end();
	for (map<Point, Monster>::iterator m = monsters[saiph->position.branch][saiph->position.level].begin(); m != monsters[saiph->position.branch][saiph->position.level].end(); ++m) {
		if (m->second.symbol != symbol || m->second.color != color)
			continue; // not the same monster
		if (saiph->world->view[m->first.row][m->first.col] == m->second.symbol && saiph->world->color[m->first.row][m->first.col] == m->second.color)
			continue; // this monster already is on its square
		/* see if this monster is closer than the last found monster */
		int distance = max(abs(m->first.row - point.row), abs(m->first.col - point.col));
		if (distance >= best_distance)
			continue;
		/* it is */
		best_distance = distance;
		best_monster = m;
	}
	if (best_monster != monsters[saiph->position.branch][saiph->position.level].end()) {
		/* remove monster from monstermap */
		saiph->map[saiph->position.branch][saiph->position.level].monster[best_monster->first.row][best_monster->first.col] = ILLEGAL_MONSTER;
		/* update monster */
		monsters[saiph->position.branch][saiph->position.level][point] = best_monster->second;
		monsters[saiph->position.branch][saiph->position.level].erase(best_monster);
		return;
	}
	/* add monster */
	monsters[saiph->position.branch][saiph->position.level][point] = Monster(symbol, color);
}
