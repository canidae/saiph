#include "Analyzers/Explore.h"

#include "Debug.h"
#include "EventBus.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Look.h"
#include "Actions/Move.h"
#include "Actions/Search.h"

#include <sstream>

// pseudo direction
#define LOOK ':'

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Explore::Explore() : Analyzer("Explore") {
}

/* methods */
std::string ExploreFocus::describe() const {
	std::ostringstream a;
	a << where << "; rank = " << rank << " [" << purpose << "]";
	return a.str();
}

void Explore::analyze() {
	// we need to analyze the current level unconditionally, in case we hit a trap door or something
	_explore_levels[Saiph::position().level()] = analyzeLevel();

	if (World::currentPriority() < PRIORITY_EXPLORE)
		return;

	if (Saiph::blind() || Saiph::confused() || Saiph::hallucinating() || Saiph::stunned())
		return; // don't explore when we're blind/confused/hallucinating/stunned

	Tile best_tile;
	Coordinate best_coord;
	int best_rank = INT_MAX;
	unsigned int best_cost = UNPASSABLE;
	const char* best_purpose = "";
	unsigned char best_direction;

	for (map<int, ExploreFocus>::iterator l = _explore_levels.begin(); l != _explore_levels.end(); ++l) {
		Level& lv = World::level(l->first);
		if (l->second.rank == INT_MAX) {
			Debug::custom(name()) << "Nothing to do on level " << lv.depth() << "," << lv.branch() << endl;
			continue;
		}
		// give a small bonus to exploring shallower levels first
		int rank = l->second.rank * 100 + lv.depth();
		// don't search shallow levels if we can search deep ones
		if (l->second.rank >= RRANK_SEARCH_MIN)
			rank += 1000 * (100 - lv.depth());
		Debug::custom(name()) << "For level " << lv.depth() << "," << lv.branch() << ", best explore point is " << l->second.describe() << endl;
		Coordinate dest(l->first, l->second.where);
		Tile path = World::shortestPath(dest);
		if ((rank < best_rank && path.cost() < UNPASSABLE) || (rank == best_rank && path.cost() < best_cost)) {
			best_tile = path;
			best_coord = dest;
			best_rank = rank;
			best_cost = path.cost();
			best_purpose = l->second.purpose;
			best_direction = l->second.direction;
		}
	}

	if (best_rank == INT_MAX)
		return;

	Debug::custom(name()) << "Going to " << best_coord << ": rank=" << best_rank << "; cost=" << best_cost << "; [" << best_purpose << "]" << endl;

	if (best_cost >= UNPASSABLE) {
		return;
	} else if (best_tile.direction() == NOWHERE) {
		if (best_direction == LOOK) {
			World::setAction(static_cast<action::Action*> (new action::Look(this)));
		} else if (best_direction == NOWHERE) {
			World::setAction(static_cast<action::Action*> (new action::Search(this, PRIORITY_EXPLORE)));
		} else {
			best_tile.direction(best_direction);
			World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, action::Move::calculatePriority(PRIORITY_EXPLORE, best_cost))));
		}
	} else {
		World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, action::Move::calculatePriority(PRIORITY_EXPLORE, best_cost))));
	}
}

/* private methods */
void Explore::addOption(ExploreFocus& best, int rank, Point p, unsigned char direction, const char* purpose) {
	Level& l = World::level();
	unsigned int cost = l.tile(p).cost();
	if ((rank < best.rank && cost < UNPASSABLE) || (rank == best.rank && cost < l.tile(best.where).cost())) {
		best.rank      = rank;
		best.where     = p;
		best.purpose   = purpose;
		best.direction = direction;
	}
}

ExploreFocus Explore::analyzeLevel() {
	ExploreFocus best;
	best.rank = INT_MAX;

	if (World::level().branch() == BRANCH_SOKOBAN)
		return best; // There's nothing to find here

	/* find stairs on rogue level */
	if (World::level().branch() == BRANCH_ROGUE) {
		for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) ROGUE_STAIRS).begin(); s != World::level().symbols((unsigned char) ROGUE_STAIRS).end(); ++s)
			addOption(best, RRANK_STAIR, s->first, LOOK, "finding direction of rogue stairs");
	}

	/* explore stairs up */
	if (World::level().depth() != 1) {
		for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) STAIRS_UP).begin(); s != World::level().symbols((unsigned char) STAIRS_UP).end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			addOption(best, RRANK_STAIR, s->first, UP, "finding endpoint of stairs up");
		}
	}

	/* explore level */
	/* floor */
	for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) FLOOR).begin(); w != World::level().symbols((unsigned char) FLOOR).end(); ++w)
		considerPoint(best, w->first);
	/* corridor */
	for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) CORRIDOR).begin(); w != World::level().symbols((unsigned char) CORRIDOR).end(); ++w)
		considerPoint(best, w->first);
	/* open door */
	for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) OPEN_DOOR).begin(); w != World::level().symbols((unsigned char) OPEN_DOOR).end(); ++w)
		considerPoint(best, w->first);
	/* unknown tile */
	for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) UNKNOWN_TILE).begin(); w != World::level().symbols((unsigned char) UNKNOWN_TILE).end(); ++w)
		considerPoint(best, w->first);

	/* explore stairs down */
	for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) STAIRS_DOWN).begin(); s != World::level().symbols((unsigned char) STAIRS_DOWN).end(); ++s) {
		if (s->second != UNKNOWN_SYMBOL_VALUE)
			continue; // we know where these stairs lead
		addOption(best, RRANK_STAIR, s->first, DOWN, "finding endpoint of stairs down");
	}

	/* explore magic portals */
	for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) MAGIC_PORTAL).begin(); s != World::level().symbols((unsigned char) MAGIC_PORTAL).end(); ++s) {
		if (s->second != UNKNOWN_SYMBOL_VALUE)
			continue; // we know where this portal lead
		if (s->first == Saiph::position())
			continue; // oscillate if the trap doesn't trigger
		addOption(best, RRANK_STAIR, s->first, NOWHERE, "finding endpoint of magic portal");
	}

	if (World::level().branch() == BRANCH_MINES && best.rank >= RRANK_SEARCH_MIN)
		best.rank = INT_MAX; /* don't bother searching the mines */

	return best;
}

void Explore::considerPoint(ExploreFocus& best, Point p) {
	/* get the symbol, wall/solid rock/search count and unpassable directions to the east, north, south and west */
	int search_count = 0;
	int solid_rock_count = 0;
	int wall_count = 0;
	p.moveWest();
	unsigned char hs = World::level().tile(p).symbol();
	bool hu = false;
	if (hs == SOLID_ROCK || hs == VERTICAL_WALL || hs == HORIZONTAL_WALL) {
		if (hs == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::level().tile(p).search();
		if (sc > search_count)
			search_count = sc;
		hu = true;
	}
	p.moveSoutheast();
	unsigned char js = World::level().tile(p).symbol();
	bool ju = false;
	if (js == SOLID_ROCK || js == VERTICAL_WALL || js == HORIZONTAL_WALL) {
		if (js == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::level().tile(p).search();
		if (sc > search_count)
			search_count = sc;
		ju = true;
	}
	p.moveNortheast();
	unsigned char ls = World::level().tile(p).symbol();
	bool lu = false;
	if (ls == SOLID_ROCK || ls == VERTICAL_WALL || ls == HORIZONTAL_WALL) {
		if (ls == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::level().tile(p).search();
		if (sc > search_count)
			search_count = sc;
		lu = true;
	}
	p.moveNorthwest();
	unsigned char ks = World::level().tile(p).symbol();
	bool ku = false;
	if (ks == SOLID_ROCK || ks == VERTICAL_WALL || ks == HORIZONTAL_WALL) {
		if (ks == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::level().tile(p).search();
		if (sc > search_count)
			search_count = sc;
		ku = true;
	}
	/* set point back to original position */
	p.moveSouth();

	/* get search count for point */
	int point_search_count = World::level().tile(p).search();

	/* find out what "type" this place is.
	 * a "type" pretty much just mean which order to explore places.
	 * we should explore places in this order:
	 * 1. visit unlit rooms, corridor tiles and search dead ends
	 * - explore another level -
	 * 2. search corridor corners & room corners
	 * - explore another level -
	 * 3. search triway corridors & room walls
	 * - explore another level -
	 * 4. search dead ends
	 * - explore another level -
	 * 5. goto step 2.
	 */
	int intervals;
	bool corner = !((!hu && !lu) || (!ju && !ku));
	if (solid_rock_count + wall_count > 0)
		intervals = search_count / EXPLORE_SEARCH_INTERVAL;
	else
		intervals = 0;
	if (World::level().tile(p).symbol() == CORRIDOR) {
		/* point is in a corridor */
		if (point_search_count < TILE_FULLY_SEARCHED) {
			/* not visited, visit it */
			addOption(best, RRANK_EXPLORE, p, NOWHERE, "unvisited corridor");
		} else {
			/* visited, search? */
			if (wall_count + solid_rock_count == 3 && wall_count <= 1) {
				/* dead end with at most 1 wall */
				// search EXPLORE_SEARCH_INTERVAL * 2 the first time
				if (intervals < 2)
					addOption(best, RRANK_EXPLORE, p, NOWHERE, "searching dead end corridor");
				else
					// 3, 6, 9, 12, ...
					addOption(best, RRANK_SEARCH_MIN - 1 + 3 * (intervals - 1), p, NOWHERE, "researching dead end corridor");
			} else if (wall_count + solid_rock_count == 2 && corner) {
				/* corridor corner 1, 4, 7, 10, ... */
				addOption(best, RRANK_SEARCH_MIN + 3 * intervals, p, NOWHERE, "searching corridor corner");
				// 2, 5, 8, 11, ...
			} else if (wall_count + solid_rock_count == 1) {
				/* triway corridor 2, 5, 8, 11, ... */
				addOption(best, RRANK_SEARCH_MIN + 1 + 3 * intervals, p, NOWHERE, "searching triway corridor");
			}
		}
	} else {
		/* probably in a room */
		if (solid_rock_count > 0 && point_search_count < TILE_FULLY_SEARCHED) {
			// solid rock next to point not visited, should visit
			addOption(best, RRANK_EXPLORE, p, NOWHERE, "unvisited room tile with unknown neighbor(s)");
		} else if (wall_count == 2 && solid_rock_count == 1) {
			/* probably in doorway with no path out, search  0, 0, 3, 6, 9, 12, ... */
			// search EXPLORE_SEARCH_INTERVAL * 2 the first time
			if (intervals < 2)
				addOption(best, RRANK_EXPLORE, p, NOWHERE, "searching dead-end doorway");
			else
				addOption(best, RRANK_SEARCH_MIN - 1 + 3 * (intervals - 1), p, NOWHERE, "researching dead-end doorway");
		} else if (wall_count == 2 && solid_rock_count == 0 && corner) {
			/* probably room corner  1, 4, 7, 10, ... */
			addOption(best, RRANK_SEARCH_MIN + 3 * intervals, p, NOWHERE, "searching room corner");
		} else if (wall_count == 1 && solid_rock_count == 0) {
			/* probably next to wall  2, 5, 8, 11, ... */
			addOption(best, RRANK_SEARCH_MIN + 1 + 3 * intervals, p, NOWHERE, "searching room wall");
		}
	}
}
