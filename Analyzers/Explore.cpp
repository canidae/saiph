#include "Explore.h"
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
	EventBus::registerEvent(TakeMeThere::id, this);
}

/* methods */
void Explore::parseMessages(const string &messages) {
}

void Explore::analyze() {
	if (Saiph::blind || Saiph::confused || Saiph::hallucinating || Saiph::stunned)
		return; // don't explore when we're blind/confused/hallucinating/stunned

	/* find stairs on rogue level */
	if (World::levels[Saiph::position.level()].branch == BRANCH_ROGUE) {
		for (map<Point, int>::iterator s = World::levels[Saiph::position.level()].symbols[(unsigned char) ROGUE_STAIRS].begin(); s != World::levels[Saiph::position.level()].symbols[(unsigned char) ROGUE_STAIRS].end(); ++s) {
			const PathNode &node = World::shortestPath(s->first);
			if (node.cost >= UNPASSABLE)
				continue;
			else if (node.dir == NOWHERE)
				World::setAction(static_cast<action::Action *> (new action::Look(this)));
			else
				World::setAction(static_cast<action::Action *> (new action::Move(this, node.dir, action::Move::calculatePriority(PRIORITY_EXPLORE_ROGUE, node.moves))));
			break;
		}
	}

	/* explore stairs up */
	if (World::levels[Saiph::position.level()].depth != 1) {
		for (map<Point, int>::iterator s = World::levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_UP].begin(); s != World::levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_UP].end(); ++s) {
			if (s->second != UNKNOWN_SYMBOL_VALUE)
				continue; // we know where these stairs lead
			const PathNode &node = World::shortestPath(s->first);
			if (node.cost >= UNPASSABLE)
				continue;
			else if (node.dir == NOWHERE)
				World::setAction(static_cast<action::Action *> (new action::Move(this, UP, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_UP, node.moves))));
			else
				World::setAction(static_cast<action::Action *> (new action::Move(this, node.dir, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_UP, node.moves))));
			break;
		}
	}

	/* explore level */
	if (World::getPriority() < PRIORITY_EXPLORE_LEVEL) {
		unsigned int min_moves = UNREACHABLE;
		int best_type = INT_MAX;
		/* floor */
		for (map<Point, int>::iterator w = World::levels[Saiph::position.level()].symbols[(unsigned char) FLOOR].begin(); w != World::levels[Saiph::position.level()].symbols[(unsigned char) FLOOR].end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
		/* corridor */
		for (map<Point, int>::iterator w = World::levels[Saiph::position.level()].symbols[(unsigned char) CORRIDOR].begin(); w != World::levels[Saiph::position.level()].symbols[(unsigned char) CORRIDOR].end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
		/* open door */
		for (map<Point, int>::iterator w = World::levels[Saiph::position.level()].symbols[(unsigned char) OPEN_DOOR].begin(); w != World::levels[Saiph::position.level()].symbols[(unsigned char) OPEN_DOOR].end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
		/* unknown tile */
		for (map<Point, int>::iterator w = World::levels[Saiph::position.level()].symbols[(unsigned char) UNKNOWN_TILE].begin(); w != World::levels[Saiph::position.level()].symbols[(unsigned char) UNKNOWN_TILE].end(); ++w)
			explorePoint(w->first, &min_moves, &best_type);
	}

	/* explore stairs down */
	for (map<Point, int>::iterator s = World::levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_DOWN].begin(); s != World::levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_DOWN].end(); ++s) {
		if (s->second != UNKNOWN_SYMBOL_VALUE)
			continue; // we know where these stairs lead
		const PathNode &node = World::shortestPath(s->first);
		if (node.cost >= UNPASSABLE)
			continue;
		else if (node.dir == NOWHERE)
			World::setAction(static_cast<action::Action *> (new action::Move(this, DOWN, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_DOWN, node.moves))));
		else
			World::setAction(static_cast<action::Action *> (new action::Move(this, node.dir, action::Move::calculatePriority(PRIORITY_EXPLORE_STAIRS_DOWN, node.moves))));
		break;
	}

	/* explore magic portals */
	for (map<Point, int>::iterator s = World::levels[Saiph::position.level()].symbols[(unsigned char) MAGIC_PORTAL].begin(); s != World::levels[Saiph::position.level()].symbols[(unsigned char) MAGIC_PORTAL].end(); ++s) {
		if (s->second != UNKNOWN_SYMBOL_VALUE)
			continue; // we know where these stairs lead
		const PathNode &node = World::shortestPath(s->first);
		if (node.cost >= UNPASSABLE)
			continue;
		else if (node.dir == NOWHERE)
			continue; // shouldn't happen
		else
			World::setAction(static_cast<action::Action *> (new action::Move(this, node.dir, action::Move::calculatePriority(PRIORITY_EXPLORE_MAGIC_PORTAL, node.moves))));
		break;
	}

	/* travel */
	map<Coordinate, int>::iterator v = visit.begin();
	while (v != visit.end()) {
		const PathNode &node = World::shortestPath(v->first);
		if (node.dir == NOWHERE) {
			visit.erase(v++);
			continue;
		} else if (node.cost < UNPASSABLE) {
			World::setAction(static_cast<action::Action *> (new action::Move(this, node.dir, action::Move::calculatePriority(v->second, node.moves))));
		}
		++v;
	}
}

void Explore::onEvent(Event * const event) {
	if (event->getID() == TakeMeThere::id) {
		TakeMeThere *e = static_cast<TakeMeThere *> (event);
		map<Coordinate, int>::iterator v = visit.find(e->coordinate);
		if (v == visit.end() || v->second < e->max_priority)
			visit[e->coordinate] = e->max_priority;
	}
}

/* private methods */
void Explore::explorePoint(Point p, unsigned int *min_moves, int *best_type) {
	/* get the symbol, wall/solid rock/search count and unpassable directions to the east, north, south and west */
	int search_count = 0;
	int solid_rock_count = 0;
	int wall_count = 0;
	p.moveWest();
	unsigned char hs = World::getDungeonSymbol(p);
	bool hu = false;
	if (hs == SOLID_ROCK || hs == VERTICAL_WALL || hs == HORIZONTAL_WALL) {
		if (hs == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::levels[Saiph::position.level()].getSearchCount(p);
		if (sc > search_count)
			search_count = sc;
		hu = true;
	}
	p.moveSoutheast();
	unsigned char js = World::getDungeonSymbol(p);
	bool ju = false;
	if (js == SOLID_ROCK || js == VERTICAL_WALL || js == HORIZONTAL_WALL) {
		if (js == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::levels[Saiph::position.level()].getSearchCount(p);
		if (sc > search_count)
			search_count = sc;
		ju = true;
	}
	p.moveNortheast();
	unsigned char ls = World::getDungeonSymbol(p);
	bool lu = false;
	if (ls == SOLID_ROCK || ls == VERTICAL_WALL || ls == HORIZONTAL_WALL) {
		if (ls == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::levels[Saiph::position.level()].getSearchCount(p);
		if (sc > search_count)
			search_count = sc;
		lu = true;
	}
	p.moveNorthwest();
	unsigned char ks = World::getDungeonSymbol(p);
	bool ku = false;
	if (ks == SOLID_ROCK || ks == VERTICAL_WALL || ks == HORIZONTAL_WALL) {
		if (ks == SOLID_ROCK)
			++solid_rock_count;
		else
			++wall_count;
		int sc = World::levels[Saiph::position.level()].getSearchCount(p);
		if (sc > search_count)
			search_count = sc;
		ku = true;
	}
	/* set point back to original position */
	p.moveSouth();

	/* get search count for point */
	int point_search_count = World::levels[Saiph::position.level()].getSearchCount(p);

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
		intervals = search_count / EXPLORE_SEARCH_INTERVAL;
	else
		intervals = 0;
	if (World::getDungeonSymbol(p) == CORRIDOR) {
		/* point is in a corridor */
		if (point_search_count < POINT_FULLY_SEARCHED) {
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
		if (solid_rock_count > 0 && point_search_count < POINT_FULLY_SEARCHED) {
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

	const PathNode &node = World::shortestPath(p);
	if (node.cost >= UNPASSABLE)
		return;
	if (type == *best_type) {
		/* same type as previous best, check distance */
		if (node.moves > *min_moves)
			return; // found a shorter path already
		if (World::getDungeonSymbol(p) == CORRIDOR && node.moves == 1 && node.moves == *min_moves && type == *best_type && (node.dir == NW || node.dir == NE || node.dir == SW || node.dir == SE))
			return; // prefer cardinal moves in corridors when distance is 1
	}
	*min_moves = node.moves;
	*best_type = type;
	if (node.dir == NOWHERE)
		World::setAction(static_cast<action::Action *> (new action::Search(this, action::Move::calculatePriority((type < 2) ? PRIORITY_EXPLORE_LEVEL : PRIORITY_EXPLORE_LEVEL / (type + 1), node.moves))));
	else
		World::setAction(static_cast<action::Action *> (new action::Move(this, node.dir, action::Move::calculatePriority((type < 2) ? PRIORITY_EXPLORE_LEVEL : PRIORITY_EXPLORE_LEVEL / (type + 1), node.moves))));
}
