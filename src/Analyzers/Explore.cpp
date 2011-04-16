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

	if (Saiph::blind() || Saiph::confused() || Saiph::hallucinating() || Saiph::stunned())
		return; // don't explore when we're blind/confused/hallucinating/stunned

	Tile best_tile;
	Coordinate best_coord;
	int best_rank = INT_MAX;
	unsigned int best_cost = UNPASSABLE;
	const char* best_purpose = "";
	unsigned char best_direction;

<<<<<<< HEAD
	/* find stairs on rogue level */
	if (World::currentPriority() < PRIORITY_EXPLORE_ROGUE && World::level().branch() == BRANCH_ROGUE) {
		for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) ROGUE_STAIRS).begin(); s != World::level().symbols((unsigned char) ROGUE_STAIRS).end(); ++s) {
			Tile& tile = World::shortestPath(s->first);
			Debug::custom(name()) << "Want to explore rogue stairs on this level" << endl;
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.direction() == NOWHERE)
				World::setAction(static_cast<action::Action*> (new action::Look(this)));
			else
				//World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_ROGUE, tile.cost()))));
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile, 100)));
			break;
=======
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
>>>>>>> ba97ff0833c0dfc7d8aa3d8d4dadda1b6e31c9e8
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
		return; // There's nothing to find here

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
<<<<<<< HEAD
			Tile tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			if (tile.direction() == NOWHERE)
				tile.direction(UP);
			Debug::custom(name()) << "Want to explore starts up on this level" << endl;
			//World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_UP, tile.cost()))));
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, 100)));
			break;
=======
			addOption(best, RRANK_STAIR, s->first, UP, "finding endpoint of stairs up");
>>>>>>> ba97ff0833c0dfc7d8aa3d8d4dadda1b6e31c9e8
		}
	}

	/* explore level */
<<<<<<< HEAD
	int best_type = INT_MAX - 1;
	if (World::currentPriority() < PRIORITY_EXPLORE_LEVEL) {
		Tile best_tile;
		/* floor */
		for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) FLOOR).begin(); w != World::level().symbols((unsigned char) FLOOR).end(); ++w)
			explorePoint(w->first, &best_tile, &best_type);
		/* corridor */
		for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) CORRIDOR).begin(); w != World::level().symbols((unsigned char) CORRIDOR).end(); ++w)
			explorePoint(w->first, &best_tile, &best_type);
		/* open door */
		for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) OPEN_DOOR).begin(); w != World::level().symbols((unsigned char) OPEN_DOOR).end(); ++w)
			explorePoint(w->first, &best_tile, &best_type);
		/* unknown tile */
		for (map<Point, int>::const_iterator w = World::level().symbols((unsigned char) UNKNOWN_TILE).begin(); w != World::level().symbols((unsigned char) UNKNOWN_TILE).end(); ++w)
			explorePoint(w->first, &best_tile, &best_type);
		/* update value for this level in _explore_levels */
		_explore_levels[Saiph::position().level()] = best_type;
		if (World::level().branch() == BRANCH_MINES && best_type > 1) {
			/* don't bother searching the mines */
		} else if (best_tile.cost() < UNPASSABLE) {
			Debug::custom(name()) << "Want to explore this level at type " << best_type << endl;
			if (best_tile.direction() == NOWHERE)
				World::setAction(static_cast<action::Action*> (new action::Search(this, (best_type < 2) ? PRIORITY_EXPLORE_LEVEL : PRIORITY_EXPLORE_LEVEL / (best_type + 1))));
			else
				//World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, action::Move::calculatePriority((best_type < 2) ? PRIORITY_EXPLORE_LEVEL : PRIORITY_EXPLORE_LEVEL / (best_type + 1), best_tile.cost()))));
				World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, 100)));
		}
	}

	/* explore stairs down */
	if (best_type > 1 && World::currentPriority() < PRIORITY_EXPLORE_STAIRS_DOWN) {
		for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) STAIRS_DOWN).begin(); s != World::level().symbols((unsigned char) STAIRS_DOWN).end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			Tile tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			if (tile.direction() == NOWHERE)
				tile.direction(DOWN);
			Debug::custom(name()) << "Want to explore starts down on this level" << endl;
			//World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_DOWN, tile.cost()))));
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, 100)));
			break;
		}
	}

	/* explore magic portals */
	if (best_type > 1 && World::currentPriority() < PRIORITY_EXPLORE_MAGIC_PORTAL) {
		for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) MAGIC_PORTAL).begin(); s != World::level().symbols((unsigned char) MAGIC_PORTAL).end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where this portal lead
			Tile tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			if (tile.direction() == NOWHERE)
				continue; // shouldn't happen (too often)
			Debug::custom(name()) << "Want to explore magic portal on this level" << endl;
			//World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_MAGIC_PORTAL, tile.cost()))));
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, 100)));
			break;
		}
	}

	/* go to a level we've explored less than this level */
	if (best_type > 1 && World::currentPriority() < PRIORITY_EXPLORE_LEVEL) {
		Tile best_tile;
		for (map<int, int>::iterator l = _explore_levels.begin(); l != _explore_levels.end(); ++l) {
			Level& lv = World::level(l->first);
			ostringstream desco(ostringstream::out);
			desco << lv.depth() << ',' << lv.branch();
			std::string desc(desco.str());
			if (Saiph::position().level() == l->first)
				continue; // same level as we're currently on
			if (lv.branch() == BRANCH_SOKOBAN)
				continue; // no point exploring sokoban
			if (lv.symbols(STAIRS_DOWN).size() <= 0 || lv.symbols(STAIRS_UP).size() <= 0) {
				/* stairs up or down not found on level, should explore this level more unless: */
				if (lv.branch() == BRANCH_MINES && lv.depth() >= 10)
					continue; // may be mine's end
				Debug::custom(name()) << "Looking for stairs up/down on level " << desc << endl;
			} else {
				/* got both stairs up & down, we should explore unless: */
				if (World::branchCoordinate(BRANCH_MINES).level() == -1 && lv.depth() >= 2 && lv.depth() <= 4) {
					/* haven't found mines, level 2-4 got entrance to it */
					Debug::custom(name()) << "Looking for entrance to the mines on level " << desc << endl;
				} else if (World::branchCoordinate(BRANCH_SOKOBAN).level() == -1 && lv.depth() >= 6 && lv.depth() <= 10) {
					/* haven't found sokoban, entrance is between level 6 and 10 */
					Debug::custom(name()) << "Looking for entrance to sokoban on level " << desc << endl;
				} else {
					/* otherwise, don't explore unless we don't know where any of the stairs lead */
					bool explore = false;
					bool stairs_up = false;
					bool stairs_down = false;
					for (map<Point, int>::const_iterator s = lv.symbols(STAIRS_UP).begin(); !explore && s != lv.symbols(STAIRS_UP).end(); ++s) {
						if (s->second == UNKNOWN_SYMBOL_VALUE) {
							explore = true;
							stairs_up = true;
						}
					}
					for (map<Point, int>::const_iterator s = lv.symbols(STAIRS_DOWN).begin(); !explore && s != lv.symbols(STAIRS_DOWN).end(); ++s) {
						if (s->second == UNKNOWN_SYMBOL_VALUE) {
							explore = true;
							stairs_down = true;
						}
					}
					if (!explore && stairs_up && stairs_down)
						continue;
					Debug::custom(name()) << "Looking for stair connections on level " << desc << endl;
				}
			}
			if (l->second > 1 && lv.branch() == BRANCH_MINES) {
				Debug::custom(name()) << "Not travelling to level " << desc << ", it's in the mines and we've already explored it enough" << endl;
				continue;
			} else if (l->second >= best_type) {
				Debug::custom(name()) << "Not travelling to level " << desc << ", type value greater than or equal to best type value: " << l->second << " >= " << best_type << endl;
				continue;
			}
			/* can we path to upstairs on this level? */
			Tile tile;
			for (map<Point, int>::const_iterator s = World::level(l->first).symbols(STAIRS_UP).begin(); tile.distance() == UNREACHABLE && s != World::level(l->first).symbols(STAIRS_UP).end(); ++s) {
				tile = World::shortestPath(Coordinate(l->first, s->first));
				if (tile.cost() < best_tile.cost())
					best_tile = tile;
			}
			/* can we path to downstairs on this level? */
			for (map<Point, int>::const_iterator s = World::level(l->first).symbols(STAIRS_DOWN).begin(); tile.distance() == UNREACHABLE && s != World::level(l->first).symbols(STAIRS_DOWN).end(); ++s) {
				tile = World::shortestPath(Coordinate(l->first, s->first));
				if (tile.cost() < best_tile.cost())
					best_tile = tile;
			}
			/* can we path to portals on this level? */
			for (map<Point, int>::const_iterator s = World::level(l->first).symbols(MAGIC_PORTAL).begin(); tile.distance() == UNREACHABLE && s != World::level(l->first).symbols(MAGIC_PORTAL).end(); ++s) {
				tile = World::shortestPath(Coordinate(l->first, s->first));
				if (tile.cost() < best_tile.cost())
					best_tile = tile;
			}
		}
		if (best_tile.cost() < UNREACHABLE) {
			Debug::custom(name()) << "Heading towards " << best_tile << " to explore level" << endl;
			//World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, action::Move::calculatePriority(PRIORITY_EXPLORE_LEVEL, best_tile.cost()))));
			World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, 100)));
		} else {
			Debug::custom(name()) << "Can't head towards " << best_tile << " to explore level, unable to reach coordinate" << endl;
		}
	}
=======
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
>>>>>>> ba97ff0833c0dfc7d8aa3d8d4dadda1b6e31c9e8
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
