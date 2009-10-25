#include "Explore.h"

#include "../Debug.h"
#include "../EventBus.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Look.h"
#include "../Actions/Move.h"
#include "../Actions/Search.h"
#include "../Events/TakeMeThere.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Explore::Explore() : Analyzer("Explore") {
	/* register events */
	EventBus::registerEvent(TakeMeThere::ID, this);
}

/* methods */
void Explore::parseMessages(const string& messages) {
}

void Explore::analyze() {
	if (Saiph::blind() || Saiph::confused() || Saiph::hallucinating() || Saiph::stunned())
		return; // don't explore when we're blind/confused/hallucinating/stunned

	/* add current level as "not explored" if we haven't already.
	 * TODO: create event "LevelDiscovered" and do this on a such event */
	if (_explore_levels.find(Saiph::position().level()) == _explore_levels.end())
		_explore_levels[Saiph::position().level()] = 0;

	/* find stairs on rogue level */
	if (World::currentPriority() < PRIORITY_EXPLORE_ROGUE && World::level(Saiph::position().level()).branch() == BRANCH_ROGUE) {
		for (map<Point, int>::iterator s = World::level(Saiph::position().level()).symbols((unsigned char) ROGUE_STAIRS).begin(); s != World::level(Saiph::position().level()).symbols((unsigned char) ROGUE_STAIRS).end(); ++s) {
			const Tile& tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.direction() == NOWHERE)
				World::setAction(static_cast<action::Action*> (new action::Look(this)));
			else
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority(PRIORITY_EXPLORE_ROGUE, tile.distance()))));
			break;
		}
	}

	/* explore stairs up */
	if (World::currentPriority() < PRIORITY_EXPLORE_STAIRS_UP && World::level(Saiph::position().level()).depth() != 1) {
		for (map<Point, int>::iterator s = World::level(Saiph::position().level()).symbols((unsigned char) STAIRS_UP).begin(); s != World::level(Saiph::position().level()).symbols((unsigned char) STAIRS_UP).end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			const Tile& tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.direction() == NOWHERE)
				World::setAction(static_cast<action::Action*> (new action::Move(this, UP, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_UP, tile.distance()))));
			else
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_UP, tile.distance()))));
			break;
		}
	}

	/* explore level */
	int best_type = INT_MAX;
	if (World::currentPriority() < PRIORITY_EXPLORE_LEVEL) {
		unsigned int min_moves = UNREACHABLE;
		/* floor */
		for (map<Point, int>::iterator w = World::level(Saiph::position().level()).symbols((unsigned char) FLOOR).begin(); w != World::level(Saiph::position().level()).symbols((unsigned char) FLOOR).end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
		/* corridor */
		for (map<Point, int>::iterator w = World::level(Saiph::position().level()).symbols((unsigned char) CORRIDOR).begin(); w != World::level(Saiph::position().level()).symbols((unsigned char) CORRIDOR).end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
		/* open door */
		for (map<Point, int>::iterator w = World::level(Saiph::position().level()).symbols((unsigned char) OPEN_DOOR).begin(); w != World::level(Saiph::position().level()).symbols((unsigned char) OPEN_DOOR).end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
		/* unknown tile */
		for (map<Point, int>::iterator w = World::level(Saiph::position().level()).symbols((unsigned char) UNKNOWN_TILE).begin(); w != World::level(Saiph::position().level()).symbols((unsigned char) UNKNOWN_TILE).end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
		/* update value for this level in _explore_levels */
		_explore_levels[Saiph::position().level()] = best_type;
	}

	/* explore stairs down */
	if (World::currentPriority() < PRIORITY_EXPLORE_STAIRS_DOWN) {
		for (map<Point, int>::iterator s = World::level(Saiph::position().level()).symbols((unsigned char) STAIRS_DOWN).begin(); s != World::level(Saiph::position().level()).symbols((unsigned char) STAIRS_DOWN).end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			const Tile& tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.direction() == NOWHERE)
				World::setAction(static_cast<action::Action*> (new action::Move(this, DOWN, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_DOWN, tile.distance()))));
			else
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_DOWN, tile.distance()))));
			break;
		}
	}

	/* explore magic portals */
	if (World::currentPriority() < PRIORITY_EXPLORE_MAGIC_PORTAL) {
		for (map<Point, int>::iterator s = World::level(Saiph::position().level()).symbols((unsigned char) MAGIC_PORTAL).begin(); s != World::level(Saiph::position().level()).symbols((unsigned char) MAGIC_PORTAL).end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			const Tile& tile = World::shortestPath(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.direction() == NOWHERE)
				continue; // shouldn't happen
			else
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority(PRIORITY_EXPLORE_MAGIC_PORTAL, tile.distance()))));
			break;
		}
	}

	/* go to a level we've explored less than this level */
	if (World::currentPriority() < PRIORITY_EXPLORE_LEVEL && best_type > 1) {
		Coordinate best_level;
		for (map<int, int>::iterator l = _explore_levels.begin(); l != _explore_levels.end(); ++l) {
			if (l->second > best_type || l->first == Saiph::position().level())
				continue;
			Tile tile;
			/* can we path to upstairs on this level? */
			for (map<Point, int>::iterator s = World::level(l->first).symbols(STAIRS_UP).begin(); tile.distance() == UNREACHABLE && s != World::level(l->first).symbols(STAIRS_UP).end(); ++s) {
				tile = World::shortestPath(Coordinate(l->first, s->first));
				best_level = Coordinate(l->first, s->first);
			}
			/* can we path to downstairs on this level? */
			for (map<Point, int>::iterator s = World::level(l->first).symbols(STAIRS_DOWN).begin(); tile.distance() == UNREACHABLE && s != World::level(l->first).symbols(STAIRS_DOWN).end(); ++s) {
				tile = World::shortestPath(Coordinate(l->first, s->first));
				best_level = Coordinate(l->first, s->first);
			}
			/* can we path to portals on this level? */
			for (map<Point, int>::iterator s = World::level(l->first).symbols(MAGIC_PORTAL).begin(); tile.distance() == UNREACHABLE && s != World::level(l->first).symbols(MAGIC_PORTAL).end(); ++s) {
				tile = World::shortestPath(Coordinate(l->first, s->first));
				best_level = Coordinate(l->first, s->first);
			}
			best_type = l->second;
		}
		if (best_level.level() != -1) {
			_visit[best_level] = PRIORITY_EXPLORE_LEVEL;
			Debug::analyzer(name()) << "Heading towards " << best_level << " to explore that level" << endl;
		}
	}

	/* travel */
	map<Coordinate, int>::iterator v = _visit.begin();
	while (v != _visit.end()) {
		const Tile& tile = World::shortestPath(v->first);
		if (tile.direction() == NOWHERE) {
			_visit.erase(v++);
			Debug::analyzer(name()) << "Reached destination at " << v->first << ", removing location from list of places to visit" << endl;
			continue;
		} else if (tile.cost() < UNPASSABLE) {
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority(v->second, tile.distance()))));
			Debug::analyzer(name()) << "Travelling to " << v->first << endl;
		} else {
			Debug::analyzer(name()) << "Unable to travel to " << v->first << ", tile = " << tile << endl;
		}
		++v;
	}
}

void Explore::onEvent(Event * const event) {
	if (event->id() == TakeMeThere::ID) {
		TakeMeThere* e = static_cast<TakeMeThere*> (event);
		map<Coordinate, int>::iterator v = _visit.find(e->coordinate());
		if (v == _visit.end() || v->second < e->max_priority())
			_visit[e->coordinate()] = e->max_priority();
	}
}

/* private methods */
void Explore::explorePoint(Point p, unsigned int* min_moves, int* best_type) {
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
		int sc = World::level(Saiph::position().level()).tile(p).search();
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
		int sc = World::level(Saiph::position().level()).tile(p).search();
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
		int sc = World::level(Saiph::position().level()).tile(p).search();
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
		int sc = World::level(Saiph::position().level()).tile(p).search();
		if (sc > search_count)
			search_count = sc;
		ku = true;
	}
	/* set point back to original position */
	p.moveSouth();

	/* get search count for point */
	int point_search_count = World::level(Saiph::position().level()).tile(p).search();

	/* find out what "type" this place is.
	 * a "type" pretty much just mean which order to explore places.
	 * we should explore places in this order:
	 * 0. visit unlit rooms (and search dead ends)
	 * 1. visit all corridor squares (and search dead ends)
	 * - explore another level -
	 * 2. search corridor corners & room corners
	 * - explore another level -
	 * 3. search triway corridors & room walls
	 * - explore another level -
	 * 4. search dead ends
	 * - explore another level -
	 *
	 * repeat step 2-4
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
	if (type == INT_MAX || type > *best_type)
		return;

	const Tile& tile = World::shortestPath(p);
	if (tile.cost() >= UNPASSABLE)
		return;
	if (type == *best_type) {
		/* same type as previous best, check distance */
		if (tile.distance() > *min_moves)
			return; // found a shorter path already
		if (World::level().tile(p).symbol() == CORRIDOR && tile.distance() == 1 && tile.distance() == *min_moves && type == *best_type && (tile.direction() == NW || tile.direction() == NE || tile.direction() == SW || tile.direction() == SE))
			return; // prefer cardinal moves in corridors when distance is 1
	}
	*min_moves = tile.distance();
	*best_type = type;
	if (tile.direction() == NOWHERE)
		World::setAction(static_cast<action::Action*> (new action::Search(this, action::Move::calculatePriority((type < 2) ? PRIORITY_EXPLORE_LEVEL : PRIORITY_EXPLORE_LEVEL / (type + 1), tile.distance()))));
	else
		World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority((type < 2) ? PRIORITY_EXPLORE_LEVEL : PRIORITY_EXPLORE_LEVEL / (type + 1), tile.distance()))));
}
