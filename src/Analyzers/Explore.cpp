#include "Analyzers/Explore.h"

#include "Debug.h"
#include "EventBus.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Look.h"
#include "Actions/Move.h"
#include "Actions/Search.h"

#include <sstream>

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Explore::Explore() : Analyzer("Explore") {
}

/* methods */
void Explore::analyze() {
	if (Saiph::blind() || Saiph::confused() || Saiph::hallucinating() || Saiph::stunned())
		return; // don't explore when we're blind/confused/hallucinating/stunned

	if (_explore_levels.find(Saiph::position().level()) == _explore_levels.end())
		_explore_levels[Saiph::position().level()] = 0;

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
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_ROGUE, tile.cost()))));
			break;
		}
	}

	/* explore stairs up */
	if (World::currentPriority() < PRIORITY_EXPLORE_STAIRS_UP && World::level().depth() != 1) {
		for (map<Point, int>::const_iterator s = World::level().symbols((unsigned char) STAIRS_UP).begin(); s != World::level().symbols((unsigned char) STAIRS_UP).end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			Tile tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			if (tile.direction() == NOWHERE)
				tile.direction(UP);
			Debug::custom(name()) << "Want to explore starts up on this level" << endl;
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_UP, tile.cost()))));
			break;
		}
	}

	/* explore level */
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
				World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, action::Move::calculatePriority((best_type < 2) ? PRIORITY_EXPLORE_LEVEL : PRIORITY_EXPLORE_LEVEL / (best_type + 1), best_tile.cost()))));
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
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_DOWN, tile.cost()))));
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
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_EXPLORE_MAGIC_PORTAL, tile.cost()))));
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
					for (map<Point, int>::const_iterator s = lv.symbols(STAIRS_UP).begin(); !explore && s != lv.symbols(STAIRS_UP).end(); ++s) {
						if (s->second == UNKNOWN_SYMBOL_VALUE)
							explore = true;
					}
					for (map<Point, int>::const_iterator s = lv.symbols(STAIRS_DOWN).begin(); !explore && s != lv.symbols(STAIRS_DOWN).end(); ++s) {
						if (s->second == UNKNOWN_SYMBOL_VALUE)
							explore = true;
					}
					if (!explore)
						continue;
					Debug::custom(name()) << "Looking for stair connections on level " << desc << endl;
				}
			}
			if (l->second > 1 && lv.branch() == BRANCH_MINES) {
				Debug::custom(name()) << "Not travelling to level " << desc << ", it's in the mines and we've already explored it enough" << endl;
				continue;
			}
			if (l->second >= best_type) {
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
			Debug::custom(name()) << "Heading towards " << best_tile.coordinate() << " to explore level" << endl;
			World::setAction(static_cast<action::Action*> (new action::Move(this, best_tile, action::Move::calculatePriority(PRIORITY_EXPLORE_LEVEL, best_tile.cost()))));
		}
	}
}

/* private methods */
void Explore::explorePoint(Point p, Tile* best_tile, int* best_type) {
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
	int type = INT_MAX;
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
			type = 1;
		} else {
			/* visited, search? */
			if (wall_count + solid_rock_count == 3 && wall_count <= 1) {
				/* dead end with at most 1 wall */
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
		if (solid_rock_count > 0 && point_search_count < TILE_FULLY_SEARCHED) {
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
	if (type > *best_type)
		return;
	Tile& tile = World::shortestPath(p);
	if (tile.cost() >= UNPASSABLE)
		return;
	else if (type == *best_type && tile.cost() > best_tile->cost())
		return; // found a shorter path already
	*best_tile = tile;
	*best_type = type;
}
