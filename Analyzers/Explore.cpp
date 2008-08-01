#include "Explore.h"

/* constructors */
Explore::Explore(Saiph *saiph) : Analyzer("Explore"), saiph(saiph) {
	memset(search, 0, sizeof (search));
	memset(ep_added, false, sizeof (ep_added));
	memset(visited, false, sizeof (visited));
	move = ILLEGAL_MOVE;
}

/* methods */
void Explore::complete() {
	if (move == SEARCH)
		++search[saiph->position.branch][saiph->position.level][saiph->world->player.row][saiph->world->player.col];
}

void Explore::finish() {
	/* figure out which place to explore */
	/* make the place the player stands on "visited" */
	visited[saiph->position.branch][saiph->position.level][saiph->world->player.row][saiph->world->player.col] = true;
	int best_moves = INT_MAX;
	move = ILLEGAL_MOVE;
	for (list<Point>::iterator e = explore.begin(); e != explore.end(); ) {
		if (search[saiph->position.branch][saiph->position.level][e->row][e->col] >= EXPLORE_SEARCH_COUNT) {
			/* this place is fully searched out. remove it from the list */
			e = explore.erase(e);
			continue;
		}
		unsigned char hs = saiph->levels[saiph->position.level].dungeonmap[e->row][e->col - 1];
		unsigned char js = saiph->levels[saiph->position.level].dungeonmap[e->row + 1][e->col];
		unsigned char ks = saiph->levels[saiph->position.level].dungeonmap[e->row - 1][e->col];
		unsigned char ls = saiph->levels[saiph->position.level].dungeonmap[e->row][e->col + 1];
		int cur_priority = 1;
		int count = 0;
		switch (saiph->levels[saiph->position.level].dungeonmap[e->row][e->col]) {
			case CORRIDOR:
				if (!visited[saiph->position.branch][saiph->position.level][e->row][e->col]) {
					cur_priority = EXPLORE_VISIT_CORRIDOR;
					break;
				}
				if (hs == SOLID_ROCK || hs == VERTICAL_WALL || hs == HORIZONTAL_WALL)
					++count;
				if (js == SOLID_ROCK || js == VERTICAL_WALL || js == HORIZONTAL_WALL)
					++count;
				if (ks == SOLID_ROCK || ks == VERTICAL_WALL || ks == HORIZONTAL_WALL)
					++count;
				if (ls == SOLID_ROCK || ls == VERTICAL_WALL || ls == HORIZONTAL_WALL)
					++count;
				if (count == 3) {
					/* dead end */
					cur_priority = EXPLORE_SEARCH_CORRIDOR_DEAD_END;
				} else if (!((hs != SOLID_ROCK && ls != SOLID_ROCK) || (js != SOLID_ROCK && ks != SOLID_ROCK))) {
					/* turning corridor */
					cur_priority = EXPLORE_SEARCH_CORRIDOR_CORNER;
				} else {
					/* this place is of no interest to us */
					e = explore.erase(e);
					continue;
				}
				break;

			case OPEN_DOOR:
				if (!visited[saiph->position.branch][saiph->position.level][e->row][e->col]) {
					cur_priority = EXPLORE_VISIT_OPEN_DOOR;
					break;
				}
				if (hs == SOLID_ROCK || js == SOLID_ROCK || ks == SOLID_ROCK || ls == SOLID_ROCK) {
					/* door with no exit */
					cur_priority = EXPLORE_SEARCH_DOOR_DEAD_END;
				} else {
					/* door with exit, uninteresting */
					e = explore.erase(e);
					continue;
				}
				break;

			case FLOOR:
				if (visited[saiph->position.branch][saiph->position.level][e->row][e->col] && search[saiph->position.branch][saiph->position.level][e->row][e->col] >= EXPLORE_SEARCH_COUNT) {
					/* been here & searched, uninteresting place */
					e = explore.erase(e);
					continue;
				}
				if (hs == SOLID_ROCK || js == SOLID_ROCK || ks == SOLID_ROCK || ls == SOLID_ROCK) {
					/* next to unlit place */
					cur_priority = EXPLORE_VISIT_UNLIT_AREA;
				} else if ((hs == VERTICAL_WALL || ls == VERTICAL_WALL) && (js == HORIZONTAL_WALL || ks == HORIZONTAL_WALL)) {
					/* corner of a room */
					cur_priority = EXPLORE_SEARCH_ROOM_CORNER;
				} else if (hs == VERTICAL_WALL || js == HORIZONTAL_WALL || ks == HORIZONTAL_WALL || ls == VERTICAL_WALL) {
					/* wall next to floor */
					cur_priority = EXPLORE_SEARCH_WALL;
				}
				break;

			case UNKNOWN_TILE:
			case UNKNOWN_TILE_DIAGONALLY_PASSABLE:
				if (!visited[saiph->position.branch][saiph->position.level][e->row][e->col]) {
					/* visit this place */
					cur_priority = EXPLORE_VISIT_UNKNOWN_TILE;
					break;;
				}

			default:
				/* this never happens */
				e = explore.erase(e);
				continue;
		}
		if (cur_priority < priority) {
			++e;
			continue;
		}
		int moves = 0;
		unsigned char nextmove = saiph->shortestPath(*e, false, &moves);
		++e;
		if (cur_priority == priority && moves > best_moves)
			continue;
		if (nextmove == ILLEGAL_MOVE)
			continue;
		if (nextmove == REST)
			move = SEARCH;
		else
			move = nextmove;
		priority = cur_priority;
		best_moves = moves;
	}
	command = move;
}

void Explore::inspect(const Point &point) {
	unsigned char ds = saiph->levels[saiph->position.level].dungeonmap[point.row][point.col];
	if (ds != CORRIDOR && ds != FLOOR && ds != OPEN_DOOR && ds != UNKNOWN_TILE && ds != UNKNOWN_TILE_DIAGONALLY_PASSABLE)
		return; // we only care about CORRIDOR, FLOOR, OPEN_DOOR, UNKNOWN_TILE & UNKNOWN_TILE_DIAGONALLY_PASSABLE
	if (ep_added[saiph->position.branch][saiph->position.level][point.row][point.col])
		return; // already added this place
	ep_added[saiph->position.branch][saiph->position.level][point.row][point.col] = true;
	explore.push_back(point);
}
