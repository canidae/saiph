#include "Explore.h"

/* constructors */
Explore::Explore(Saiph *saiph) : saiph(saiph) {
	memset(search, 0, sizeof (search));
	memset(ep_added, false, sizeof (ep_added));
	memset(visited, false, sizeof (visited));
	move = ILLEGAL_MOVE;
}

/* methods */
void Explore::command(string *command) {
	if (move == SEARCH)
		++search[saiph->current_branch][saiph->current_level][saiph->world->player.row][saiph->world->player.col];
	*command = move;
}

int Explore::finish() {
	/* figure out which place to explore */
	int b = saiph->current_branch;
	int l = saiph->current_level;
	/* make the place the player stands on "visited" */
	visited[b][l][saiph->world->player.row][saiph->world->player.col] = true;
	int best_priority = -1;
	int best_distance = INT_MAX;
	move = ILLEGAL_MOVE;
	for (list<Point>::iterator e = explore.begin(); e != explore.end(); ) {
		if (search[b][l][e->row][e->col] >= EXPLORE_SEARCH_COUNT) {
			/* this place is fully searched out. remove it from the list */
			e = explore.erase(e);
			continue;
		}
		unsigned char hs = saiph->map[b][l].dungeon[e->row][e->col - 1];
		unsigned char js = saiph->map[b][l].dungeon[e->row + 1][e->col];
		unsigned char ks = saiph->map[b][l].dungeon[e->row - 1][e->col];
		unsigned char ls = saiph->map[b][l].dungeon[e->row][e->col + 1];
		int priority = 1;
		int count = 0;
		switch (saiph->map[b][l].dungeon[e->row][e->col]) {
			case CORRIDOR:
				if (!visited[b][l][e->row][e->col]) {
					priority = EXPLORE_VISIT_CORRIDOR;
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
					priority = EXPLORE_SEARCH_CORRIDOR_DEAD_END;
				} else if (!((hs != SOLID_ROCK && ls != SOLID_ROCK) || (js != SOLID_ROCK && ks != SOLID_ROCK))) {
					/* turning corridor */
					priority = EXPLORE_SEARCH_CORRIDOR_CORNER;
				} else {
					/* this place is of no interest to us */
					e = explore.erase(e);
					continue;
				}
				break;

			case OPEN_DOOR:
				if (!visited[b][l][e->row][e->col]) {
					priority = EXPLORE_VISIT_OPEN_DOOR;
					break;
				}
				if (hs == SOLID_ROCK || js == SOLID_ROCK || ks == SOLID_ROCK || ls == SOLID_ROCK) {
					/* door with no exit */
					priority = EXPLORE_SEARCH_DOOR_DEAD_END;
				} else {
					/* door with exit, uninteresting */
					e = explore.erase(e);
					continue;
				}
				break;

			case FLOOR:
				if (visited[b][l][e->row][e->col] && search[b][l][e->row][e->col] >= EXPLORE_SEARCH_COUNT) {
					/* been here & searched, uninteresting place */
					e = explore.erase(e);
					continue;
				}
				if (hs == SOLID_ROCK || js == SOLID_ROCK || ks == SOLID_ROCK || ls == SOLID_ROCK) {
					/* next to unlit place */
					priority = EXPLORE_VISIT_UNLIT_AREA;
				} else if ((hs == VERTICAL_WALL || ls == VERTICAL_WALL) && (js == HORIZONTAL_WALL || ks == HORIZONTAL_WALL)) {
					/* corner of a room */
					priority = EXPLORE_SEARCH_ROOM_CORNER;
				} else if (hs == VERTICAL_WALL || js == HORIZONTAL_WALL || ks == HORIZONTAL_WALL || ls == VERTICAL_WALL) {
					/* wall next to floor */
					priority = EXPLORE_SEARCH_WALL;
				}
				break;

			default:
				/* this never happens */
				e = explore.erase(e);
				continue;
		}
		if (priority < best_priority) {
			++e;
			continue;
		}
		int distance = 0;
		bool straight_line = false;
		unsigned char nextmove = saiph->shortestPath(*e, false, &distance, &straight_line);
		++e;
		if (priority == best_priority && distance > best_distance)
			continue;
		if (nextmove == ILLEGAL_MOVE)
			continue;
		if (nextmove == REST)
			move = SEARCH;
		else
			move = nextmove;
		best_priority = priority;
		best_distance = distance;
	}
	return best_priority;
}

void Explore::inspect(const Point &point) {
	unsigned char s = saiph->world->view[point.row][point.col];
	if (s != CORRIDOR && s != FLOOR && s != OPEN_DOOR)
		return; // we only care about CORRIDOR, FLOOR and OPEN_DOOR
	int b = saiph->current_branch;
	int l = saiph->current_level;
	if (ep_added[b][l][point.row][point.col])
		return; // already added this place
	ep_added[b][l][point.row][point.col] = true;
	explore.push_back(point);
}
