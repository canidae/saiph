#include <string.h>
#include "Explore.h"
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Explore::Explore(Saiph *saiph) : Analyzer("Explore"), saiph(saiph) {
	/* clear search map */
	for (int l = 0; l < EXPLORE_LEVELS; ++l) {
		for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
			for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c)
				search[l][r][c] = 0;
		}
	}
	mines_explored = false;
}

/* methods */
void Explore::analyze() {
	/* figure out which place to explore */
	if (saiph->world->player.blind || saiph->world->player.hallucinating || saiph->world->player.stunned || saiph->world->player.confused)
		return; // no exploring while blind/hallu/stun/conf

	/* make the place the player stands on fully searched */
	search[saiph->position.level][saiph->position.row][saiph->position.col] = EXPLORE_FULLY_SEARCHED;

	/* where do you want to go today? */
	unsigned char best_move = 0;

	/* find rogue stairs if we're on rogue level */
	if (priority < PRIORITY_EXPLORE_FIND_ROGUE_STAIRS && saiph->levels[saiph->position.level].branch == BRANCH_ROGUE) {
		for (map<Point, int>::iterator s = saiph->levels[saiph->position.level].symbols[(unsigned char) ROGUE_STAIRS].begin(); s != saiph->levels[saiph->position.level].symbols[(unsigned char) ROGUE_STAIRS].end(); ++s) {
			int moves = 0;
			unsigned char dir = saiph->shortestPath(s->first, false, &moves);
			if (dir != ILLEGAL_DIRECTION) {
				if (dir == NOWHERE) {
					command = LOOK;
					priority = PRIORITY_LOOK;
					return;
				} else {
					best_move = dir;
				}
				priority = PRIORITY_EXPLORE_FIND_ROGUE_STAIRS;
				break;
			}
		}
	}

	/* explore upstairs */
	if (priority < PRIORITY_EXPLORE_STAIRS_UP && saiph->levels[saiph->position.level].depth != 1) {
		/* explore upstairs unless on depth 1 */
		for (map<Point, int>::iterator s = saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_UP].begin(); s != saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_UP].end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			int moves = 0;
			unsigned char dir = saiph->shortestPath(s->first, false, &moves);
			if (dir != ILLEGAL_DIRECTION) {
				if (dir == NOWHERE)
					best_move = UP;
				else
					best_move = dir;
				priority = PRIORITY_EXPLORE_STAIRS_UP;
				break;
			}
		}
	}

	if (priority < PRIORITY_EXPLORE_EXPLORE) {
		int min_moves = INT_MAX;
		int best_type = INT_MAX;
		unsigned char dir;
		/* floor */
		for (map<Point, int>::iterator w = saiph->levels[saiph->position.level].symbols[(unsigned char) FLOOR].begin(); w != saiph->levels[saiph->position.level].symbols[(unsigned char) FLOOR].end(); ++w) {
			dir = calculatePointScore(w, &min_moves, &best_type);
			if (dir != ILLEGAL_DIRECTION)
				best_move = dir;
		}
		/* corridor */
		for (map<Point, int>::iterator w = saiph->levels[saiph->position.level].symbols[(unsigned char) CORRIDOR].begin(); w != saiph->levels[saiph->position.level].symbols[(unsigned char) CORRIDOR].end(); ++w) {
			dir = calculatePointScore(w, &min_moves, &best_type);
			if (dir != ILLEGAL_DIRECTION)
				best_move = dir;
		}
		/* open door */
		for (map<Point, int>::iterator w = saiph->levels[saiph->position.level].symbols[(unsigned char) OPEN_DOOR].begin(); w != saiph->levels[saiph->position.level].symbols[(unsigned char) OPEN_DOOR].end(); ++w) {
			dir = calculatePointScore(w, &min_moves, &best_type);
			if (dir != ILLEGAL_DIRECTION)
				best_move = dir;
		}
		/* unknown tile */
		for (map<Point, int>::iterator w = saiph->levels[saiph->position.level].symbols[(unsigned char) UNKNOWN_TILE].begin(); w != saiph->levels[saiph->position.level].symbols[(unsigned char) UNKNOWN_TILE].end(); ++w) {
			dir = calculatePointScore(w, &min_moves, &best_type);
			if (dir != ILLEGAL_DIRECTION)
				best_move = dir;
		}
	}

	/* explore downstairs */
	if (priority < PRIORITY_EXPLORE_STAIRS_DOWN) {
		/* explore downstairs unless already exploring upstairs */
		for (map<Point, int>::iterator s = saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].begin(); s != saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			int moves = 0;
			unsigned char dir = saiph->shortestPath(s->first, false, &moves);
			if (dir != ILLEGAL_DIRECTION) {
				if (dir == NOWHERE)
					best_move = DOWN;
				else
					best_move = dir;
				priority = PRIORITY_EXPLORE_STAIRS_DOWN;
				break;
			}
		}
	}

	/* travel */
	if (priority < PRIORITY_EXPLORE_TRAVEL) {
		unsigned char move = ILLEGAL_DIRECTION;
		if (!mines_explored) {
			/* explore mines */
			move = exploreMines();
		/* add more "goals" here */
		} else {
			/* no goals, go to main dungeon and descend */
			if (saiph->levels[saiph->position.level].branch != BRANCH_MAIN && saiph->levels[saiph->position.level].branch != BRANCH_ROGUE) {
				/* not in main dungeon or rogue level, get out */
				int moves = 0;
				move = saiph->shortestPath(saiph->branch_main, false, &moves);
			} else {
				/* in main dungeon, descend */
				for (map<Point, int>::iterator down = saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].begin(); down != saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].end(); ++down) {
					if (down->second != UNKNOWN_SYMBOL_VALUE && saiph->levels[down->second].branch != BRANCH_MAIN && saiph->levels[down->second].branch != BRANCH_ROGUE)
						continue; // avoid other branches than main
					int moves = 0;
					move = saiph->shortestPath(down->first, false, &moves);
					if (move == NOWHERE)
						move = DOWN;
					break;
				}
			}
		}
		/* set best_move & priority */
		if (move != ILLEGAL_DIRECTION) {
			best_move = move;
			priority = PRIORITY_EXPLORE_TRAVEL;
		}
	}

	/* set command */
	if (best_move != ILLEGAL_DIRECTION) {
		if (best_move == NOWHERE)
			command = SEARCH;
		else
			command = best_move;
	} else {
		priority = ILLEGAL_PRIORITY;
	}
}

void Explore::complete() {
	if (command == SEARCH) {
		/* increase search for all adjacent squares */
		for (int r = saiph->position.row - 1; r <= saiph->position.row + 1; ++r) {
			if (r < MAP_ROW_BEGIN || r > MAP_ROW_END)
				continue;
			for (int c = saiph->position.col - 1; c <= saiph->position.col + 1; ++c) {
				if (c < MAP_COL_BEGIN || c > MAP_COL_END)
					continue;
				if (search[saiph->position.level][r][c] >= EXPLORE_FULLY_SEARCHED)
					continue;
				++search[saiph->position.level][r][c];
			}
		}
	}
}

void Explore::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(MESSAGE_TELEPORT_WHERE, 0) != string::npos) {
		/* temporary hack for teleport control */
		command = "><,";
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && messages.find(MESSAGE_TELEPORT_INSTRUCTIONS, 0) != string::npos) {
		/* ignore the instructions */
		command = "";
		priority = PRIORITY_CONTINUE_ACTION;
	}
	/* TODO
	 * apply stethoscope to search for hidden door/passage.
	 * You hear a hollow sound.  This must be a secret door! - probably not needed (we'll see the door)
	 * You hear a hollow sound.  This must be a secret passage! - probably not needed (we'll see the passage)
	 * You hear nothing special.
	 * You hear your heart beat. */
}

/* private methods */
unsigned char Explore::calculatePointScore(map<Point, int>::iterator w, int *min_moves, int *best_type) {
	/* get the symbol to the east, north, south and west */
	unsigned char hs = (w->first.col - 1 < MAP_COL_BEGIN ? SOLID_ROCK : saiph->levels[saiph->position.level].dungeonmap[w->first.row][w->first.col - 1]);
	unsigned char js = (w->first.row + 1 > MAP_ROW_END ? SOLID_ROCK : saiph->levels[saiph->position.level].dungeonmap[w->first.row + 1][w->first.col]);
	unsigned char ks = (w->first.row - 1 < MAP_ROW_BEGIN ? SOLID_ROCK : saiph->levels[saiph->position.level].dungeonmap[w->first.row - 1][w->first.col]);
	unsigned char ls = (w->first.col + 1 > MAP_COL_END ? SOLID_ROCK : saiph->levels[saiph->position.level].dungeonmap[w->first.row][w->first.col + 1]);

	/* get wall/solid rock/search count and unpassable directions */
	int search_count = 0;
	int solid_rock_count = 0;
	int wall_count = 0;
	bool hu = false;
	bool ju = false;
	bool ku = false;
	bool lu = false;
	if (hs == SOLID_ROCK) {
		++solid_rock_count;
		search_count += (w->first.col - 1 < MAP_COL_BEGIN ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row][w->first.col - 1]);
		hu = true;
	}
	if (js == SOLID_ROCK) {
		++solid_rock_count;
		search_count += (w->first.row + 1 > MAP_ROW_END ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row + 1][w->first.col]);
		ju = true;
	}
	if (ks == SOLID_ROCK) {
		++solid_rock_count;
		search_count += (w->first.row - 1 < MAP_ROW_BEGIN ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row - 1][w->first.col]);
		ku = true;
	}
	if (ls == SOLID_ROCK) {
		++solid_rock_count;
		search_count += (w->first.col + 1 > MAP_COL_END ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row][w->first.col + 1]);
		lu = true;
	}
	if (hs == VERTICAL_WALL || hs == HORIZONTAL_WALL) {
		++wall_count;
		search_count += (w->first.col - 1 < MAP_COL_BEGIN ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row][w->first.col - 1]);
		hu = true;
	}
	if (js == HORIZONTAL_WALL || js == VERTICAL_WALL) {
		++wall_count;
		search_count += (w->first.row + 1 > MAP_ROW_END ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row + 1][w->first.col]);
		ju = true;
	}
	if (ks == HORIZONTAL_WALL || ks == VERTICAL_WALL) {
		++wall_count;
		search_count += (w->first.row - 1 < MAP_ROW_BEGIN ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row - 1][w->first.col]);
		ku = true;
	}
	if (ls == VERTICAL_WALL || ls == HORIZONTAL_WALL) {
		++wall_count;
		search_count += (w->first.col + 1 > MAP_COL_END ? EXPLORE_FULLY_SEARCHED : search[saiph->position.level][w->first.row][w->first.col + 1]);
		lu = true;
	}


	/* get search count for point */
	int point_search_count = search[saiph->position.level][w->first.row][w->first.col];

	/* find out what "type" this place is.
	 * a "type" pretty much just mean which order to explore places.
	 * we should explore places in this order:
	 * 0. visit unlit rooms (and search dead ends)
	 * 1. visit all corridor squares (and search dead ends)
	 * - descend if stairs found -
	 * 2. search corridor corners & room corners
	 * 3. search triway corridors & room walls
	 * 4. search dead ends
	 *
	 * repeat step 2-4
	 */
	int type = INT_MAX;
	int intervals;
	bool corner = !((!hu && !lu) || (!ju && !ku));
	if (solid_rock_count + wall_count > 0)
		intervals = search_count / EXPLORE_SEARCH_INTERVAL / (solid_rock_count + wall_count);
	else
		intervals = 0;
	if (saiph->levels[saiph->position.level].dungeonmap[w->first.row][w->first.col] == CORRIDOR) {
		/* point is in a corridor */
		if (point_search_count < EXPLORE_FULLY_SEARCHED) {
			/* not visited, visit it */
			type = 1;
		} else {
			/* visited, search? */
			if (wall_count + solid_rock_count == 3) {
				/* dead end */
				if (intervals < 2)
					type = 1; // search EXPLORE_SEARCH_INTERVAL * 2 the first time
				else
					type = 3 * (intervals - 1) + 1; // 4, 7, 10, 13, ...
			} else if (wall_count + solid_rock_count == 2 && corner) {
				/* corridor corner */
				type = 3 * intervals + 2; // 2, 5, 8, 11, ...
			} else if (wall_count + solid_rock_count == 1) {
				/* triway corridor */
				type = 3 * intervals + 3; // 3, 6, 9, 12, ...
			}
		}
	} else {
		/* probably in a room */
		if (solid_rock_count > 0 && point_search_count < EXPLORE_FULLY_SEARCHED) {
			type = 1; // solid rock next to point not visited, should visit
		} else if (wall_count == 2 && solid_rock_count == 1) {
			/* probably in doorway with no path out, search */
			if (intervals < 2)
				type = 1; // search EXPLORE_SEARCH_INTERVAL * 2 the first time
			else
				type = 3 * (intervals - 1) + 1; // 4, 7, 10, 13, ...
		} else if (wall_count == 2 && solid_rock_count == 0 && corner) {
			/* probably room corner */
			type = 3 * intervals + 2; // 2, 5, 8, 11, ...
		} else if (wall_count == 1 && solid_rock_count == 0) {
			/* probably next to wall */
			type = 3 * intervals + 3; // 3, 6, 9, 12, ...
		}
	}

	/* check if this "type" is worse than what we already got */
	if (type == INT_MAX || type > *best_type)
		return ILLEGAL_DIRECTION;

	int moves = 0;
	unsigned char dir = saiph->shortestPath(w->first, false, &moves);
	if (dir == ILLEGAL_DIRECTION)
		return dir;
	if (type == *best_type) {
		/* same type as previous best, check distance */
		if (moves > *min_moves)
			return ILLEGAL_DIRECTION; // found a shorter path already
		if (saiph->levels[saiph->position.level].dungeonmap[w->first.row + 1][w->first.col] == CORRIDOR && moves == 1 && moves == *min_moves && type == *best_type && (dir == NW || dir == NE || dir == SW || dir == SE))
			return ILLEGAL_DIRECTION; // prefer cardinal moves in corridors when distance is 1
	}
	*min_moves = moves;
	*best_type = type;
	priority = (type < 2 ? PRIORITY_EXPLORE_EXPLORE : PRIORITY_EXPLORE_SEARCH);
	return dir;
}

unsigned char Explore::exploreMines() {
	if (saiph->levels[saiph->position.level].branch == BRANCH_MINES) {
		/* we're in the mines, but haven't found mine's end yet */
		for (map<Point, int>::iterator down = saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].begin(); down != saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].end(); ++down) {
			int moves = 0;
			unsigned char dir = saiph->shortestPath(down->first, false, &moves);
			if (dir == NOWHERE)
				return DOWN;
			else
				return dir;
		}
		if (saiph->levels[saiph->position.level].depth >= 10 && saiph->levels[saiph->position.level].depth <= 13) {
			Debug::notice(saiph->last_turn) << EXPLORE_DEBUG_NAME << "Mine's End presumably found" << endl;
			mines_explored = true; // possibly at mine's end
		}
	} else if (saiph->branch_mines.level != -1) {
		/* we've seen the mines, but we're not in the mines and haven't explored it.
		 * path to the mines */
		int moves = 0;
		unsigned char move = saiph->shortestPath(saiph->branch_mines, false, &moves);
		if (move != ILLEGAL_DIRECTION)
			return move;
		/* this may happen, for example when she can't find the path there.
		 * just descend for now, regardless of which branch we're in */
		for (map<Point, int>::iterator down = saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].begin(); down != saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].end(); ++down) {
			int moves = 0;
			move = saiph->shortestPath(down->first, false, &moves);
			if (move == NOWHERE)
				move = DOWN;
			break;
		}
		return move;
	} else {
		/* we're not in the mines nor have we seen the mines */
		if (saiph->levels[saiph->position.level].depth > 4) {
			/* crap, we're below level 4 and haven't seen the mines? */
			/* FIXME: need to find the mines, not just descend */
			for (map<Point, int>::iterator down = saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].begin(); down != saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].end(); ++down) {
				int moves = 0;
				unsigned char move = saiph->shortestPath(down->first, false, &moves);
				if (move == NOWHERE)
					move = DOWN;
				return move;
			}
		} else {
			/* we're still not below level 4, just descend */
			for (map<Point, int>::iterator down = saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].begin(); down != saiph->levels[saiph->position.level].symbols[(unsigned char) STAIRS_DOWN].end(); ++down) {
				int moves = 0;
				unsigned char move = saiph->shortestPath(down->first, false, &moves);
				if (move == NOWHERE)
					move = DOWN;
				return move;
			}
		}
	}
	return ILLEGAL_DIRECTION;
}
