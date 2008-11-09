#include "Explore.h"
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Explore::Explore(Saiph *saiph) : Analyzer("Explore"), saiph(saiph) {
	/* this analyzer needs a _huge_ overhaul, it just sucks */
	for (int a = 0; a < MAX_DUNGEON_DEPTH; ++a) {
		for (int b = 0; b < MAP_ROW_END + 1; ++b) {
			for (int c = 0; c < MAP_COL_END + 1; ++c) {
				search[a][b][c] = 0;
				visited[a][b][c] = false;
			}
		}
	}
}

/* methods */
void Explore::analyze() {
	/* figure out which place to explore */
	if (saiph->world->player.blind || saiph->world->player.hallucinating || saiph->world->player.stunned)
		return; // no exploring while blind/hallu/stun
	/* make the place the player stands on "visited" */
	visited[saiph->position.level][saiph->world->player.row][saiph->world->player.col] = true;
	if (commands[0].priority < EXPLORE_UNKNOWN_STAIRS && saiph->levels[saiph->position.level].depth != 1) {
		/* explore unknown stairs on level, unless we're on depth 1.
		 * there's only 1 stairs down on dlvl 1, and we don't want her to escape */
		/* go up first (or we'll never check the upstairs) */
		for (map<Point, int>::iterator s = saiph->levels[saiph->position.level].symbols[STAIRS_UP].begin(); s != saiph->levels[saiph->position.level].symbols[STAIRS_UP].end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			int moves = 0;
			unsigned char move = saiph->shortestPath(s->first, false, &moves);
			if (move != ILLEGAL_MOVE) {
				if (move == MOVE_NOWHERE)
					setCommand(0, EXPLORE_UNKNOWN_STAIRS, string(1, MOVE_UP));
				else
					setCommand(0, EXPLORE_UNKNOWN_STAIRS, string(1, move));
				sequence = 0;
				return;
			}
		}
		for (map<Point, int>::iterator s = saiph->levels[saiph->position.level].symbols[STAIRS_DOWN].begin(); s != saiph->levels[saiph->position.level].symbols[STAIRS_DOWN].end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			int moves = 0;
			unsigned char move = saiph->shortestPath(s->first, false, &moves);
			if (move != ILLEGAL_MOVE) {
				if (move == MOVE_NOWHERE)
					setCommand(0, EXPLORE_UNKNOWN_STAIRS, string(1, MOVE_DOWN));
				else
					setCommand(0, EXPLORE_UNKNOWN_STAIRS, string(1, move));
				sequence = 0;
				return;
			}
		}
	}
	int best_moves = INT_MAX;
	for (list<Point>::iterator e = explore.begin(); e != explore.end(); ) {
		if (search[saiph->position.level][e->row][e->col] >= EXPLORE_SEARCH_COUNT) {
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
				if (!visited[saiph->position.level][e->row][e->col]) {
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
				if (!visited[saiph->position.level][e->row][e->col]) {
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
				if (visited[saiph->position.level][e->row][e->col] && search[saiph->position.level][e->row][e->col] >= EXPLORE_SEARCH_COUNT) {
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
				if (!visited[saiph->position.level][e->row][e->col]) {
					/* visit this place */
					cur_priority = EXPLORE_VISIT_UNKNOWN_TILE;
					break;;
				}

			default:
				/* this never happens */
				e = explore.erase(e);
				continue;
		}
		if (cur_priority < commands[0].priority) {
			++e;
			continue;
		}
		int moves = 0;
		unsigned char move = saiph->shortestPath(*e, false, &moves);
		++e;
		if (cur_priority == commands[0].priority && moves > best_moves)
			continue;
		if (move == ILLEGAL_MOVE)
			continue;
		if (move == MOVE_NOWHERE)
			setCommand(0, cur_priority, SEARCH);
		else
			setCommand(0, cur_priority, string(1, move));
		sequence = 0;
		best_moves = moves;
	}
	if (saiph->levels[saiph->position.level].branch == BRANCH_MINES && commands[0].priority < EXPLORE_DESCEND) {
		/* if we're in the mines, go up */
		for (map<Point, int>::iterator up = saiph->levels[saiph->position.level].symbols[STAIRS_UP].begin(); up != saiph->levels[saiph->position.level].symbols[STAIRS_DOWN].end(); ++up) {
			int moves = 0;
			unsigned char move = saiph->shortestPath(up->first, false, &moves);
			if (move != ILLEGAL_MOVE) {
				if (move == MOVE_NOWHERE)
					setCommand(0, EXPLORE_DESCEND, string(1, MOVE_UP));
				else
					setCommand(0, EXPLORE_DESCEND, string(1, move));
				sequence = 0;
				break;
			}
		}
	}
	if (commands[0].priority < EXPLORE_DESCEND) {
		/* descend */
		for (map<Point, int>::iterator down = saiph->levels[saiph->position.level].symbols[STAIRS_DOWN].begin(); down != saiph->levels[saiph->position.level].symbols[STAIRS_DOWN].end(); ++down) {
			if (down->second != UNKNOWN_SYMBOL_VALUE && saiph->levels[down->second].branch == BRANCH_MINES)
				continue; // avoid mines
			int moves = 0;
			unsigned char move = saiph->shortestPath(down->first, false, &moves);
			if (move != ILLEGAL_MOVE) {
				if (move == MOVE_NOWHERE)
					setCommand(0, EXPLORE_DESCEND, string(1, MOVE_DOWN));
				else
					setCommand(0, EXPLORE_DESCEND, string(1, move));
				sequence = 0;
				break;
			}
		}
	}
}

void Explore::complete() {
	if (getCommand() == SEARCH)
		++search[saiph->position.level][saiph->world->player.row][saiph->world->player.col];
}

void Explore::inspect(const Point &point) {
	unsigned char ds = saiph->levels[saiph->position.level].dungeonmap[point.row][point.col];
	if (ds != CORRIDOR && ds != FLOOR && ds != OPEN_DOOR && ds != UNKNOWN_TILE && ds != UNKNOWN_TILE_DIAGONALLY_PASSABLE)
		return; // we only care about CORRIDOR, FLOOR, OPEN_DOOR, UNKNOWN_TILE & UNKNOWN_TILE_DIAGONALLY_PASSABLE
	explore.push_back(point);
}

void Explore::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(MESSAGE_TELEPORT_WHERE, 0) != string::npos) {
		/* temporary hack for teleport control */
		setCommand(0, PRIORITY_CONTINUE_ACTION, "><,");
		sequence = 0;
	}
}
