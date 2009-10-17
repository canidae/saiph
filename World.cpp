#include <stdlib.h>
#include <string.h>
#include "Connection.h"
#include "Debug.h"
#include "Inventory.h"
#include "World.h"
#include "Actions/Action.h"
#include "Actions/Move.h"
#include "Analyzers/Analyzer.h"
#include "Data/Item.h"
#include "Data/Monster.h"
#include "Events/Event.h"

using namespace analyzer;
using namespace std;

/* static variables */
vector<Point> World::changes;
char World::view[ROWS][COLS + 1] = {
	{'\0'}
};
int World::color[ROWS][COLS] = {
	{0}
};
Point World::cursor;
int World::cur_page = -1;
int World::max_page = -1;
int World::command_count = 0;
int World::frame_count = 0;
bool World::menu = false;
bool World::question = false;
bool World::engulfed = false;
char World::levelname[MAX_LEVELNAME_LENGTH] = {'\0'};
int World::turn = 0;
int World::real_turn = 0;
vector<Level> World::levels;
Coordinate World::branch[BRANCHES];

Connection *World::connection = NULL;
action::Action *World::action = NULL;
list<action::Action *> World::action_queue;
bool World::changed[MAP_ROW_END + 1][MAP_COL_END + 1] = {
	{false}
};
string World::messages = " ";
bool World::inverse = false;
bool World::bold = false;
char World::data[BUFFER_SIZE * 2] = {'\0'};
char World::effects[MAX_EFFECTS][MAX_TEXT_LENGTH] = {
	{'\0'}
};
int World::data_size = -1;
string World::msg_str;
Point World::last_menu;
map<string, vector<int> > World::levelmap;
time_t World::start_time = time(NULL);
vector<Analyzer *> World::analyzers;
int World::last_action_id = NO_ACTION;

/* methods */
void World::init(int connection_type) {
	connection = Connection::create(connection_type);
	if (connection == NULL) {
		cout << "ERROR: Don't know what interface this is: " << connection_type << endl;
		exit(1);
	}
	/* fetch the first "frame" */
	update();
}

void World::destroy() {
	delete action;
	delete connection;
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		delete *a;
}

void World::registerAnalyzer(Analyzer *analyzer) {
	Debug::info() << SAIPH_DEBUG_NAME << "Registering analyzer " << analyzer->name() << endl;
	analyzers.push_back(analyzer);
}

void World::unregisterAnalyzer(Analyzer *analyzer) {
	Debug::info() << SAIPH_DEBUG_NAME << "Unregistering analyzer " << analyzer->name() << endl;
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
		if ((*a)->name() == analyzer->name()) {
			analyzers.erase(a);
			return;
		}
	}
}

int World::getPriority() {
	if (action == NULL)
		return action::Action::noop.priority();
	return action->command().priority();
}

bool World::setAction(action::Action *action) {
	if (World::action != NULL) {
		if (action->command().priority() <= World::action->command().priority()) {
			delete action;
			return false; // already got an action with higher priority
		}
		delete World::action;
	}
	World::action = action;
	return true;
}

bool World::queueAction(action::Action *action) {
	if (action == NULL) {
		return false; // shouldn't happen, though
	} else if (action->command().priority() <= PRIORITY_TURN_MAX) {
		/* not a zero-turn action, can't queue it */
		delete action;
		return false;
	}
	action_queue.push_back(action);
	return true;
}

unsigned char World::directLine(Point point, bool ignore_sinks, bool ignore_boulders) {
	/* is the target in a direct line from the player? */
	if (point.row() < MAP_ROW_BEGIN || point.row() > MAP_ROW_END || point.col() < MAP_COL_BEGIN || point.col() > MAP_COL_END) {
		/* outside map */
		return ILLEGAL_DIRECTION;
	} else if (point == Saiph::position) {
		/* eh? don't do this */
		return NOWHERE;
	} else if (point.row() == Saiph::position.row()) {
		/* aligned horizontally */
		if (point.col() > Saiph::position.col()) {
			while (point.moveWest().col() > Saiph::position.col()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return E;
		} else {
			while (point.moveEast().col() < Saiph::position.col()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return W;
		}
	} else if (point.col() == Saiph::position.col()) {
		/* aligned vertically */
		if (point.row() > Saiph::position.row()) {
			while (point.moveNorth().row() > Saiph::position.row()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return S;
		} else {
			while (point.moveSouth().row() < Saiph::position.row()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return N;
		}
	} else if (abs(point.row() - Saiph::position.row()) == abs(point.col() - Saiph::position.col())) {
		/* aligned diagonally */
		if (point.row() > Saiph::position.row()) {
			if (point.col() > Saiph::position.col()) {
				while (point.moveNorthwest().row() > Saiph::position.row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return SE;
			} else {
				while (point.moveNortheast().row() > Saiph::position.row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return SW;
			}
		} else {
			if (point.col() > Saiph::position.col()) {
				while (point.moveSouthwest().row() < Saiph::position.row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return NE;
			} else {
				while (point.moveSoutheast().row() < Saiph::position.row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return NW;
			}
		}
	}
	return ILLEGAL_DIRECTION;
}

PathNode World::shortestPath(unsigned char symbol) {
	/* returns PathNode for shortest path from player to nearest symbol */
	int pivot = -1;
	int level_count = 1;
	PathNode best_pathnode;
	int level_queue[levels.size()];
	level_queue[0] = Saiph::position.level();
	bool level_added[levels.size()];
	for (int a = 0; a < (int) levels.size(); ++a)
		level_added[a] = false;
	level_added[Saiph::position.level()] = true;
	PathNode level_pathnode[levels.size()];
	level_pathnode[Saiph::position.level()] = PathNode(Point(), NOWHERE, 0, 0);
	while (++pivot < level_count) {
		/* path to symbols on level */
		for (map<Point, int>::iterator s = levels[level_queue[pivot]].symbols[symbol].begin(); s != levels[level_queue[pivot]].symbols[symbol].end(); ++s) {
			const PathNode &node = levels[level_queue[pivot]].shortestPath(s->first);
			Debug::info() << SAIPH_DEBUG_NAME << "Found '" << symbol << "' on level " << level_queue[pivot] << ": " << node.dir << " - " << node.moves << " - " << node.cost << endl;
			if (node.cost == UNREACHABLE)
				continue;
			else if (node.cost == UNPASSABLE && node.moves > 1)
				continue;
			else if (node.cost + level_pathnode[level_queue[pivot]].cost >= best_pathnode.cost)
				continue;
			/* this symbol is closer than the previously found one */
			best_pathnode = node;
			if (pivot != 0) {
				/* symbol is on another level, gotta modify this pathnode a bit */
				best_pathnode.dir = level_pathnode[level_queue[pivot]].dir;
				best_pathnode.moves += level_pathnode[level_queue[pivot]].moves;
				best_pathnode.cost += level_pathnode[level_queue[pivot]].cost;
			}
			Debug::info() << SAIPH_DEBUG_NAME << "Pathing to '" << symbol << "' on level " << level_queue[pivot] << endl;
		}
		/* path to upstairs on level */
		for (map<Point, int>::iterator s = levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_UP].begin(); s != levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_UP].end(); ++s) {
			Debug::info() << SAIPH_DEBUG_NAME << "Found upstairs on level " << level_queue[pivot] << " leading to level " << s->second << endl;
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where these stairs lead
			if (level_added[s->second])
				continue; // already added this level
			const PathNode &node = levels[level_queue[pivot]].shortestPath(s->first);
			if (node.cost >= UNPASSABLE)
				continue;
			else if (node.cost + level_pathnode[level_queue[pivot]].cost >= best_pathnode.cost)
				continue;
			/* distance to these stairs is shorter than shortest path found so far.
			 * we should check the level these stairs lead to as well */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to upstairs on level we're standing on */
				level_pathnode[s->second] = node;
				if (node.dir == NOWHERE)
					level_pathnode[s->second].dir = UP;
			} else {
				/* pathing to upstairs on another level */
				level_pathnode[s->second] = level_pathnode[level_queue[pivot]];
				level_pathnode[s->second].moves += node.moves;
				level_pathnode[s->second].cost += node.cost;
			}
			Debug::info() << SAIPH_DEBUG_NAME << "Added level " << s->second << " to the queue" << endl;
		}
		/* path to downstairs on level */
		for (map<Point, int>::iterator s = levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_DOWN].begin(); s != levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_DOWN].end(); ++s) {
			Debug::info() << SAIPH_DEBUG_NAME << "Found downstairs on level " << level_queue[pivot] << " leading to level " << s->second << endl;
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where these stairs lead
			if (level_added[s->second])
				continue; // already added this level
			const PathNode &node = levels[level_queue[pivot]].shortestPath(s->first);
			if (node.cost >= UNPASSABLE)
				continue;
			else if (node.cost + level_pathnode[level_queue[pivot]].cost >= best_pathnode.cost)
				continue;
			/* distance to these stairs is shorter than shortest path found so far.
			 * we should check the level these stairs lead to as well */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to downstairs on level we're standing on */
				level_pathnode[s->second] = node;
				if (node.dir == NOWHERE)
					level_pathnode[s->second].dir = DOWN;
			} else {
				/* pathing to downstairs on another level */
				level_pathnode[s->second] = level_pathnode[level_queue[pivot]];
				level_pathnode[s->second].moves += node.moves;
				level_pathnode[s->second].cost += node.cost;
			}
			Debug::info() << SAIPH_DEBUG_NAME << "Added level " << s->second << " to the queue" << endl;
		}
		/* path to levels through magic portals */
		for (map<Point, int>::iterator s = levels[level_queue[pivot]].symbols[(unsigned char) MAGIC_PORTAL].begin(); s != levels[level_queue[pivot]].symbols[(unsigned char) MAGIC_PORTAL].end(); ++s) {
			Debug::info() << SAIPH_DEBUG_NAME << "Found magic portal on level " << level_queue[pivot] << " leading to level " << s->second << endl;
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where this magic portal leads
			if (level_added[s->second])
				continue; // already added this level
			const PathNode &node = levels[level_queue[pivot]].shortestPath(s->first);
			if (node.cost >= UNPASSABLE)
				continue;
			else if (node.cost + level_pathnode[level_queue[pivot]].cost >= best_pathnode.cost)
				continue;
			/* distance to these stairs is shorter than shortest path found so far.
			 * we should check the level these stairs lead to as well */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to downstairs on level we're standing on */
				level_pathnode[s->second] = node;
				if (node.dir == NOWHERE)
					level_pathnode[s->second].dir = NOWHERE;
			} else {
				/* pathing to downstairs on another level */
				level_pathnode[s->second] = level_pathnode[level_queue[pivot]];
				level_pathnode[s->second].moves += node.moves;
				level_pathnode[s->second].cost += node.cost;
			}
			Debug::info() << SAIPH_DEBUG_NAME << "Added level " << s->second << " to the queue" << endl;
		}
	}
	return best_pathnode;
}

PathNode World::shortestPath(const Coordinate &target) {
	/* returns PathNode for shortest path from player to target */
	if (target.level() < 0 || target.level() >= (int) levels.size()) {
		return PathNode(); // outside the map
	} else if (target.level() == Saiph::position.level()) {
		/* target on same level */
		return levels[Saiph::position.level()].shortestPath(target);
	} else {
		int pivot = -1;
		int level_count = 1;
		int level_queue[levels.size()];
		level_queue[0] = Saiph::position.level();
		bool level_added[levels.size()];
		for (int a = 0; a < (int) levels.size(); ++a)
			level_added[a] = false;
		level_added[Saiph::position.level()] = true;
		PathNode level_pathnode[levels.size()];
		level_pathnode[Saiph::position.level()] = PathNode(Point(), NOWHERE, 0, 0);
		Debug::info() << SAIPH_DEBUG_NAME << "Interlevel pathing to " << target << endl;
		while (++pivot < level_count) {
			Debug::notice() << SAIPH_DEBUG_NAME << "interlevel pathing: " << pivot << " - " << level_count << endl;
			/* check if target is on level */
			if (level_queue[pivot] == target.level()) {
				const PathNode &node = levels[level_queue[pivot]].shortestPath(target);
				if (node.cost == UNREACHABLE)
					continue;
				else if (node.cost == UNPASSABLE && node.moves > 1)
					continue;
				PathNode best_pathnode = node;
				if (pivot != 0) {
					/* symbol is on another level, gotta modify this pathnode a bit */
					best_pathnode.dir = level_pathnode[level_queue[pivot]].dir;
					best_pathnode.moves += level_pathnode[level_queue[pivot]].moves;
					best_pathnode.cost += level_pathnode[level_queue[pivot]].cost;
				}
				Debug::info() << SAIPH_DEBUG_NAME << "Found " << target << " in " << best_pathnode.moves << " steps" << endl;
				return best_pathnode;
			}
			/* path to upstairs on level */
			for (map<Point, int>::iterator s = levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_UP].begin(); s != levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_UP].end(); ++s) {
				Debug::info() << SAIPH_DEBUG_NAME << "Found upstairs on level " << level_queue[pivot] << " leading to level " << s->second << endl;
				if (s->second == UNKNOWN_SYMBOL_VALUE)
					continue; // we don't know where these stairs lead
				else if (level_added[s->second])
					continue; // already added this level
				const PathNode &node = levels[level_queue[pivot]].shortestPath(s->first);
				if (node.cost >= UNPASSABLE)
					continue;
				/* distance to these stairs is shorter than shortest path found so far.
				 * we should check the level these stairs lead to as well */
				level_added[s->second] = true;
				level_queue[level_count++] = s->second;
				if (pivot == 0) {
					/* pathing to upstairs on level we're standing on */
					level_pathnode[s->second] = node;
					if (node.dir == NOWHERE)
						level_pathnode[s->second].dir = UP;
				} else {
					/* pathing to upstairs on another level */
					level_pathnode[s->second] = level_pathnode[level_queue[pivot]];
					level_pathnode[s->second].dir = level_pathnode[level_queue[pivot]].dir;
					level_pathnode[s->second].moves += level_pathnode[level_queue[pivot]].moves;
					level_pathnode[s->second].cost += level_pathnode[level_queue[pivot]].cost;
				}
				Debug::info() << SAIPH_DEBUG_NAME << "Added level " << s->second << " to the queue" << endl;
			}
			/* path to downstairs on level */
			for (map<Point, int>::iterator s = levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_DOWN].begin(); s != levels[level_queue[pivot]].symbols[(unsigned char) STAIRS_DOWN].end(); ++s) {
				Debug::info() << SAIPH_DEBUG_NAME << "Found downstairs on level " << level_queue[pivot] << " leading to level " << s->second << endl;
				if (s->second == UNKNOWN_SYMBOL_VALUE)
					continue; // we don't know where these stairs lead
				else if (level_added[s->second])
					continue; // already added this level
				const PathNode &node = levels[level_queue[pivot]].shortestPath(s->first);
				if (node.cost >= UNPASSABLE)
					continue;
				/* distance to these stairs is shorter than shortest path found so far.
				 * we should check the level these stairs lead to as well */
				level_added[s->second] = true;
				level_queue[level_count++] = s->second;
				if (pivot == 0) {
					/* pathing to downstairs on level we're standing on */
					level_pathnode[s->second] = node;
					if (node.dir == NOWHERE)
						level_pathnode[s->second].dir = DOWN;
				} else {
					/* pathing to downstairs on another level */
					level_pathnode[s->second] = level_pathnode[level_queue[pivot]];
					level_pathnode[s->second].dir = level_pathnode[level_queue[pivot]].dir;
					level_pathnode[s->second].moves += level_pathnode[level_queue[pivot]].moves;
					level_pathnode[s->second].cost += level_pathnode[level_queue[pivot]].cost;
				}
				Debug::info() << SAIPH_DEBUG_NAME << "Added level " << s->second << " to the queue" << endl;
			}
			/* path to portals on level */
			for (map<Point, int>::iterator s = levels[level_queue[pivot]].symbols[(unsigned char) MAGIC_PORTAL].begin(); s != levels[level_queue[pivot]].symbols[(unsigned char) MAGIC_PORTAL].end(); ++s) {
				Debug::info() << SAIPH_DEBUG_NAME << "Found magic portal on level " << level_queue[pivot] << " leading to level " << s->second << endl;
				if (s->second == UNKNOWN_SYMBOL_VALUE)
					continue; // we don't know where these stairs lead
				else if (level_added[s->second])
					continue; // already added this level
				const PathNode &node = levels[level_queue[pivot]].shortestPath(s->first);
				if (node.cost >= UNPASSABLE)
					continue;
				/* distance to these stairs is shorter than shortest path found so far.
				 * we should check the level these stairs lead to as well */
				level_added[s->second] = true;
				level_queue[level_count++] = s->second;
				if (pivot == 0) {
					/* pathing to downstairs on level we're standing on */
					level_pathnode[s->second] = node;
					if (node.dir == NOWHERE)
						level_pathnode[s->second].dir = DOWN;
				} else {
					/* pathing to downstairs on another level */
					level_pathnode[s->second] = level_pathnode[level_queue[pivot]];
					level_pathnode[s->second].dir = level_pathnode[level_queue[pivot]].dir;
					level_pathnode[s->second].moves += level_pathnode[level_queue[pivot]].moves;
					level_pathnode[s->second].cost += level_pathnode[level_queue[pivot]].cost;
				}
				Debug::info() << SAIPH_DEBUG_NAME << "Added level " << s->second << " to the queue" << endl;
			}
		}
	}
	return PathNode(); // symbol not found
}

void World::run() {
	int last_turn = 0;
	int stuck_counter = 0;
	while (true) {
		/* let Saiph, Inventory and current level parse messages */
		Saiph::parseMessages(messages);
		Inventory::parseMessages(messages);
		levels[Saiph::position.level()].parseMessages(messages);

		/* let Saiph, Inventory and current level analyze */
		Saiph::analyze();
		Inventory::analyze();
		levels[Saiph::position.level()].analyze();

		/* dump maps */
		dumpMaps();

		/* check if we're in the middle of an action */
		if (action == NULL || action->command() == action::Action::noop) {
			/* we got no command, find a new one */
			/* parse messages */
			for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
				(*a)->parseMessages(messages);

			/* analyze */
			if (!question && !menu) {
				for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
					(*a)->analyze();
			}
		}

		/* check if we got some queued actions */
		for (list<action::Action *>::iterator a = action_queue.begin(); a != action_queue.end(); ++a) {
			if (setAction(*a)) {
				/* we will execute this action, remove it from queue.
				 * if it fails, the analyzer that queued the action needs to handle it */
				action_queue.erase(a);
				break;
			}
		}

		/* check if we got a command */
		if (action == NULL || action->command() == action::Action::noop) {
			/* we do not. print debugging and just answer something sensible */
			if (question) {
				Debug::warning() << SAIPH_DEBUG_NAME << "Unhandled question: " << messages << endl;
				last_action_id = NO_ACTION;
				executeCommand(string(1, (char) 27));
				continue;
			} else if (menu) {
				Debug::warning() << SAIPH_DEBUG_NAME << "Unhandled menu: " << messages << endl;
				last_action_id = NO_ACTION;
				executeCommand(string(1, (char) 27));
				continue;
			} else {
				Debug::warning() << SAIPH_DEBUG_NAME << "I have no idea what to do... Searching" << endl;
				cout << (unsigned char) 27 << "[1;82H";
				cout << (unsigned char) 27 << "[K"; // erase everything to the right
				cout << "No idea what to do: s";
				/* return cursor back to where it was */
				cout << (unsigned char) 27 << "[" << cursor.row() + 1 << ";" << cursor.col() + 1 << "H";
				cout.flush();
				++World::real_turn; // command than may increase turn counter
				last_action_id = NO_ACTION;
				executeCommand("s");
				continue;
			}
		}

		/* print what we're doing */
		cout << (unsigned char) 27 << "[1;82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
		cout << action->analyzer()->name() << " " << action->command();
		/* return cursor back to where it was */
		cout << (unsigned char) 27 << "[" << cursor.row() + 1 << ";" << cursor.col() + 1 << "H";
		/* and flush cout. if we don't do this our output looks like garbage */
		cout.flush();
		Debug::notice() << "Analyzer " << action->analyzer()->name() << " " << action->command() << endl;

		/* execute the command */
		if (action->command().priority() <= PRIORITY_TURN_MAX)
			++World::real_turn; // command that may increase turn counter
		last_action_id = action->getID();
		executeCommand(action->command().command());

		/* check if we're stuck */
		if (action != NULL && stuck_counter % 42 == 41 && action->command().command().size() == 1) {
			bool was_move = false;
			/* we'll assume we're moving if the command that's stuck is a direction.
			 * if not, it's probably not a big deal */
			switch (action->command().command()[0]) {
			case NW:
			case NE:
			case SW:
			case SE:
				/* moving diagonally failed.
				 * we could be trying to move diagonally into a door we're
				 * unaware of because of an item blocking the door symbol.
				 * make the tile UNKNOWN_TILE_DIAGONALLY_UNPASSABLE */
				setDungeonSymbol(directionToPoint((unsigned char) action->command().command()[0]), UNKNOWN_TILE_DIAGONALLY_UNPASSABLE);
				was_move = true;
				break;

			case N:
			case E:
			case S:
			case W:
				/* moving cardinally failed, possibly item in wall.
				 * make the tile UNKNOWN_TILE_UNPASSABLE */
				setDungeonSymbol(directionToPoint((unsigned char) action->command().command()[0]), UNKNOWN_TILE_UNPASSABLE);
				was_move = true;
				break;

			default:
				/* certainly not moving */
				break;
			}
			if (!was_move) {
				/* not good. we're not moving and we're stuck */
				Debug::warning() << SAIPH_DEBUG_NAME << "Command failed for analyzer " << action->analyzer()->name() << ": " << action->command() << endl;
			}
		} else if (stuck_counter > 1680) {
			/* failed too many times, #quit */
			Debug::error() << SAIPH_DEBUG_NAME << "Appear to be stuck, quitting game" << endl;
			last_action_id = NO_ACTION;
			executeCommand(string(1, (char) 27));
			executeCommand(QUIT);
			executeCommand(string(1, YES));
			return;
		}
		if (last_turn == turn)
			stuck_counter++;
		else
			stuck_counter = 0;
		last_turn = turn;

		/* and finally update current action */
		if (action != NULL)
			action->update(messages);
	}
}

/* private methods */
void World::addChangedLocation(const Point &point) {
	/* add a location changed since last frame unless it's already added */
	if (point.row() < MAP_ROW_BEGIN || point.row() > MAP_ROW_END || point.col() < MAP_COL_BEGIN || point.col() > MAP_COL_END || changed[point.row()][point.col()])
		return;
	changes.push_back(point);
}

void World::detectPosition() {
	string level = levelname;
	if (Saiph::position.level() < 0) {
		/* this happens when we start */
		Saiph::position.row(cursor.row());
		Saiph::position.col(cursor.col());
		Saiph::position.level(levels.size());
		branch[BRANCH_MAIN] = Saiph::position;
		levels.push_back(Level(level));
		levelmap[level].push_back(Saiph::position.level());
		return;
	}
	if ((int) levels.size() > Saiph::position.level() && level == levels[Saiph::position.level()].name) {
		/* same level as last frame, update row & col */
		Saiph::position.row(cursor.row());
		Saiph::position.col(cursor.col());
		if (branch[BRANCH_SOKOBAN].level() == -1 && levels[Saiph::position.level()].branch == BRANCH_MAIN && levels[Saiph::position.level()].depth >= 5 && levels[Saiph::position.level()].depth <= 9) {
			/* look for sokoban level 1a or 1b */
			if (getDungeonSymbol(Point(8, 37)) == BOULDER && getDungeonSymbol(Point(8, 38)) == BOULDER && getDungeonSymbol(Point(8, 43)) == BOULDER && getDungeonSymbol(Point(9, 38)) == BOULDER && getDungeonSymbol(Point(9, 39)) == BOULDER && getDungeonSymbol(Point(9, 42)) == BOULDER && getDungeonSymbol(Point(9, 44)) == BOULDER && getDungeonSymbol(Point(11, 41)) == BOULDER && getDungeonSymbol(Point(14, 39)) == BOULDER && getDungeonSymbol(Point(14, 40)) == BOULDER && getDungeonSymbol(Point(14, 41)) == BOULDER && getDungeonSymbol(Point(14, 42)) == BOULDER) {
				/* sokoban 1a */
				Debug::notice() << SAIPH_DEBUG_NAME << "Found Sokoban level 1a: " << Saiph::position << endl;
				levels[Saiph::position.level()].branch = BRANCH_SOKOBAN;
				branch[BRANCH_SOKOBAN] = Saiph::position;
			} else if (getDungeonSymbol(Point(8, 34)) == BOULDER && getDungeonSymbol(Point(8, 42)) == BOULDER && getDungeonSymbol(Point(9, 34)) == BOULDER && getDungeonSymbol(Point(9, 41)) == BOULDER && getDungeonSymbol(Point(10, 42)) == BOULDER && getDungeonSymbol(Point(13, 40)) == BOULDER && getDungeonSymbol(Point(14, 41)) == BOULDER && getDungeonSymbol(Point(15, 41)) == BOULDER && getDungeonSymbol(Point(16, 40)) == BOULDER && getDungeonSymbol(Point(16, 42)) == BOULDER) {
				/* sokoban 1b */
				Debug::notice() << SAIPH_DEBUG_NAME << "Found Sokoban level 1b: " << Saiph::position << endl;
				levels[Saiph::position.level()].branch = BRANCH_SOKOBAN;
				branch[BRANCH_SOKOBAN] = Saiph::position;
			}

		}
		if (levels[Saiph::position.level()].branch == BRANCH_MAIN && levels[Saiph::position.level()].depth >= 3 && levels[Saiph::position.level()].depth <= 5) {
			/* if mines are not found and depth is between 3 & 5, we should attempt to detect mines */
			for (map<Point, int>::iterator hw = levels[Saiph::position.level()].symbols[(unsigned char) HORIZONTAL_WALL].begin(); hw != levels[Saiph::position.level()].symbols[(unsigned char) HORIZONTAL_WALL].end(); ++hw) {
				if (hw->first.row() <= MAP_ROW_BEGIN || hw->first.row() >= MAP_ROW_END || hw->first.col() <= MAP_COL_BEGIN || hw->first.col() >= MAP_COL_END)
					continue;
				/* if we see horizontal walls adjacent to this point (except west & east),
				 * then we're in the mines */
				if (getDungeonSymbol(Point(hw->first.row() - 1, hw->first.col() - 1)) == HORIZONTAL_WALL || getDungeonSymbol(Point(hw->first.row() - 1, hw->first.col())) == HORIZONTAL_WALL || getDungeonSymbol(Point(hw->first.row() - 1, hw->first.col() + 1)) == HORIZONTAL_WALL || getDungeonSymbol(Point(hw->first.row() + 1, hw->first.col() - 1)) == HORIZONTAL_WALL || getDungeonSymbol(Point(hw->first.row() + 1, hw->first.col())) == HORIZONTAL_WALL || getDungeonSymbol(Point(hw->first.row() + 1, hw->first.col() + 1)) == HORIZONTAL_WALL) {
					/* we're in the mines */
					Debug::notice() << SAIPH_DEBUG_NAME << "Found the mines: " << Saiph::position << endl;
					levels[Saiph::position.level()].branch = BRANCH_MINES;
					branch[BRANCH_MINES] = Saiph::position;
					break;
				}
			}
		}
		if (levels[Saiph::position.level()].branch != BRANCH_ROGUE && view[STATUS_ROW][8] == '*') {
			/* rogue level, set branch attribute */
			Debug::notice() << SAIPH_DEBUG_NAME << "Found the rogue level: " << Saiph::position << endl;
			levels[Saiph::position.level()].branch = BRANCH_ROGUE;
		}
		return;
	}
	/* level has changed.
	 * we need to figure out if it's a new level or one we already know of */
	int found = UNKNOWN_SYMBOL_VALUE;
	unsigned char symbol = getDungeonSymbol();
	/* maybe we already know where these stairs lead? */
	if (symbol == STAIRS_DOWN) {
		/* we did stand on stairs down, and if we don't know where they lead then
		 * the next line will still just set found to UNKNOWN_SYMBOL_VALUE */
		found = levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_DOWN][Saiph::position];
	} else if (symbol == STAIRS_UP) {
		/* we did stand on stairs up, and if we don't know where they lead then
		 * the next line will still just set found to UNKNOWN_SYMBOL_VALUE */
		found = levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_UP][Saiph::position];
	} else if (symbol == MAGIC_PORTAL) {
		/* we did stand on a magic portal, and if we don't know where it leads then
		 * the next line will still just set found to UNKNOWN_SYMBOL_VALUE */
		found = levels[Saiph::position.level()].symbols[(unsigned char) MAGIC_PORTAL][Saiph::position];
	}
	if (found == UNKNOWN_SYMBOL_VALUE) {
		/* we didn't know where the stairs would take us */
		for (vector<int>::iterator lm = levelmap[level].begin(); lm != levelmap[level].end(); ++lm) {
			/* check if level got walls on same locations.
			 * since walls can disappear, we'll allow a 80% match */
			int total = 0;
			int matched = 0;
			for (map<Point, int>::iterator s = levels[*lm].symbols[(unsigned char) VERTICAL_WALL].begin(); s != levels[*lm].symbols[(unsigned char) VERTICAL_WALL].end(); ++s) {
				if (view[s->first.row()][s->first.col()] == VERTICAL_WALL)
					++matched;
				++total;
			}
			for (map<Point, int>::iterator s = levels[*lm].symbols[(unsigned char) HORIZONTAL_WALL].begin(); s != levels[*lm].symbols[(unsigned char) HORIZONTAL_WALL].end(); ++s) {
				if (view[s->first.row()][s->first.col()] == HORIZONTAL_WALL)
					++matched;
				++total;
			}
			if (matched > 0 && min(matched, total) * 5 >= max(matched, total) * 4) {
				found = *lm;
				Debug::notice() << SAIPH_DEBUG_NAME << "Recognized level " << found << ": '" << level << "' - '" << levels[found].name << "'" << endl;
				break;
			}
		}
	}
	if (found == UNKNOWN_SYMBOL_VALUE) {
		/* new level */
		found = levels.size();
		/* when we discover a new level it's highly likely it's in the
		 * same branch as the previous level.
		 * exception is rogue level, which really isn't a branch */
		levels.push_back(Level(level, (levels[Saiph::position.level()].branch != BRANCH_ROGUE) ? levels[Saiph::position.level()].branch : BRANCH_MAIN));
		levelmap[level].push_back(found);
		Debug::notice() << SAIPH_DEBUG_NAME << "Found new level " << found << ": " << level << endl;
	}
	/* were we on stairs on last Saiph::position? */
	if (symbol == STAIRS_DOWN) {
		/* yes, we were on stairs down */
		levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_DOWN][Saiph::position] = found;
	} else if (symbol == STAIRS_UP) {
		/* yes, we were on stairs up */
		levels[Saiph::position.level()].symbols[(unsigned char) STAIRS_UP][Saiph::position] = found;
	} else if (symbol == MAGIC_PORTAL) {
		/* yes, we were on a magic portal */
		levels[Saiph::position.level()].symbols[(unsigned char) MAGIC_PORTAL][Saiph::position] = found;
	}
	Saiph::position.row(cursor.row());
	Saiph::position.col(cursor.col());
	Saiph::position.level(found);
}

Point World::directionToPoint(unsigned char direction) {
	/* return the position we'd be at if we do the given move */
	Point pos = Saiph::position;
	switch (direction) {
	case NW:
		pos.moveNorthwest();
		break;

	case N:
		pos.moveNorth();
		break;

	case NE:
		pos.moveNortheast();
		break;

	case E:
		pos.moveEast();
		break;

	case SE:
		pos.moveSoutheast();
		break;

	case S:
		pos.moveSouth();
		break;

	case SW:
		pos.moveSouthwest();
		break;

	case W:
		pos.moveWest();
		break;
	}
	return pos;
}

bool World::directLineHelper(const Point &point, bool ignore_sinks, bool ignore_boulders) {
	unsigned char symbol = getDungeonSymbol(point);
	if (!Level::passable[symbol] && (!ignore_boulders || symbol != BOULDER))
		return false;
	else if (!ignore_sinks && symbol == SINK)
		return false;
	else if (getMonsterSymbol(point) != ILLEGAL_MONSTER && levels[Saiph::position.level()].monsters[point].visible)
		return false;
	return true;
}

void World::dumpMaps() {
	/* XXX: World echoes output from the game in the top left corner */
	/* commands/frames/turns per second */
	int seconds = (int) difftime(time(NULL), start_time);
	if (seconds == 0)
		++seconds;
	int cps = command_count / seconds;
	int fps = frame_count / seconds;
	int tps = turn / seconds;
	cout << (unsigned char) 27 << "[25;1H";
	cout << "CPS/FPS/TPS: ";
	cout << (unsigned char) 27 << "[34m" << cps << (unsigned char) 27 << "[0m/";
	cout << (unsigned char) 27 << "[35m" << fps << (unsigned char) 27 << "[0m/";
	cout << (unsigned char) 27 << "[36m" << tps << (unsigned char) 27 << "[0m      ";

	/* monsters and map as saiph sees it */
	Point p;
	for (p.row(MAP_ROW_BEGIN); p.insideMap(); p.moveSouth()) {
		cout << (unsigned char) 27 << "[" << p.row() + 26 << ";2H";
		for (p.col(MAP_COL_BEGIN); p.insideMap(); p.moveEast()) {
			unsigned char monster = getMonsterSymbol(p);
			if (p.row() == Saiph::position.row() && p.col() == Saiph::position.col())
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (monster != ILLEGAL_MONSTER)
				cout << monster;
			else
				cout << getDungeonSymbol(p);
		}
	}

	/* path map */
	/*
	for (p.row = MAP_ROW_BEGIN; p.row <= MAP_ROW_END; ++p.row) {
		cout << (unsigned char) 27 << "[" << p.row + 26 << ";2H";
		for (p.col = MAP_COL_BEGIN; p.col <= MAP_COL_END; ++p.col) {
			if (p.row == postion.row && p.col == Saiph::position.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (levels[Saiph::position.level].pathmap[p.row][p.col].dir != ILLEGAL_DIRECTION)
				//cout << (unsigned char) levels[Saiph::position.level].pathmap[p.row][p.col].dir;
				cout << (char) (levels[Saiph::position.level].pathmap[p.row][p.col].cost % 64 + 48);
			else
				cout << getDungeonSymbol(p);
		}
	}
	 */

	/* status & inventory */
	cout << (unsigned char) 27 << "[2;82H";
	if (Saiph::intrinsics & PROPERTY_COLD)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[34m" << "Cold " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_DISINT)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[35m" << "DisInt " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_FIRE)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[31m" << "Fire " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_POISON)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[32m" << "Poison " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_SHOCK)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[36m" << "Shock " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_SLEEP)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[33m" << "Sleep " << (unsigned char) 27 << "[m";

	cout << (unsigned char) 27 << "[3;82H";
	if (Saiph::intrinsics & PROPERTY_ESP)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[35m" << "ESP " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_TELEPORT_CONTROL)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[36m" << "TeleCon " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_TELEPORT)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[33m" << "Teleport " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics & PROPERTY_LYCANTHROPY)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[31m" << "Lycan " << (unsigned char) 27 << "[m";
	if (Saiph::hurt_leg)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[34m" << "Leg " << (unsigned char) 27 << "[m";
	if (Saiph::polymorphed)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[32m" << "Poly " << (unsigned char) 27 << "[m";

	int ir = 0;
	for (map<unsigned char, Item>::iterator i = Inventory::items.begin(); i != Inventory::items.end() && ir < 46; ++i) {
		cout << (unsigned char) 27 << "[" << (4 + ir) << ";82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
		if (i->second.beatitude == BLESSED)
			cout << (unsigned char) 27 << "[32m";
		else if (i->second.beatitude == CURSED)
			cout << (unsigned char) 27 << "[31m";
		else if (i->second.beatitude == UNCURSED)
			cout << (unsigned char) 27 << "[33m";
		cout << i->first;
		cout << " - " << i->second;
		cout << (unsigned char) 27 << "[m";
		++ir;
	}
	for (; ir < 46; ++ir) {
		cout << (unsigned char) 27 << "[" << (5 + ir) << ";82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
	}
}

bool World::executeCommand(const string &command) {
	/* send a command to nethack */
	for (vector<Point>::iterator c = changes.begin(); c != changes.end(); ++c)
		changed[c->row()][c->col()] = false;
	changes.clear();
	messages = "  "; // we want 2 spaces before the first message too
	if (command.size() <= 0) {
		/* huh? no command? */
		return false;
	}
	connection->transmit(command);
	++command_count;
	update();
	return true;
}

void World::fetchMenuText(int stoprow, int startcol, bool addspaces) {
	/* fetch text from a menu */
	for (int r = 0; r <= stoprow; ++r) {
		msg_str = &view[r][startcol];
		/* trim */
		string::size_type fns = msg_str.find_first_not_of(" ");
		string::size_type lns = msg_str.find_last_not_of(" ");
		if (fns == string::npos || lns == string::npos || fns >= lns)
			continue; // blank line?
		msg_str = msg_str.substr(fns, lns - fns + 1);
		if (addspaces)
			msg_str.append(2, ' '); // append 2 spaces for later splitting
		messages.append(msg_str);
	}
}

void World::fetchMessages() {
	/* even yet a try on fetching messages sanely */
	question = false; // we can do this as a question max last 1 turn
	msg_str = &data[data_size - sizeof (MORE)];
	string::size_type pos = string::npos;
	if ((pos = msg_str.find(MORE, 0)) != string::npos) {
		/* "--More--" found */
		menu = false; // we don't have a menu then
		int r = cursor.row();
		int c = cursor.col() - sizeof (MORE) + 1; // +1 because sizeof (MORE) is 9, not 8
		if (r == 0) {
			/* only one line, remove "--More--" from end of line */
			msg_str = view[r];
			msg_str = msg_str.substr(0, c);
			/* append 2 spaces for later splitting */
			msg_str.append(2, ' ');
			messages.append(msg_str);
		} else {
			/* more than 1 line */
			if (c == 0 || view[r][c - 1] != ' ') {
				/* this is just a very long line, not a list */
				c = 0;
				fetchMenuText(r, c, false);
				messages.erase(messages.size() - sizeof (MORE) + 1); // remove "--More--"
				messages.append(2, ' '); // add two spaces
			} else {
				/* this is a list */
				/* sometimes "--More--" is placed 1 char to the right of the menu.
				 * this happens at least when the entire page is filled.
				 * check that the line above also is ' ', if not, c - 1 */
				if (view[r - 1][c - 1] == ' ')
					fetchMenuText(r - 1, c, true); // "r - 1" to avoid the last "--More--"
				else
					fetchMenuText(r - 1, c - 1, true); // "r - 1" to avoid the last "--More--"
			}
		}
		/* request the remaining messages */
		connection->transmit(" ");
		++command_count;
		update();
		return;
	} else if (cursor.row() == 0) {
		/* looks like we got a question.
		 * we might want to significantly improve this later,
		 * as we sometimes get partial data */
		question = true;
		menu = false; // no menu when we got a question
	} else {
		/* --More-- not found, but we might have a menu.
		 * this is pain */
		if (menu) {
			/* we had a menu last frame, check if we still do */
			msg_str = &view[last_menu.row()][last_menu.col()];
			cur_page = -1;
			max_page = -1;
			if (msg_str.find(END, 0) == string::npos && sscanf(&view[last_menu.row()][last_menu.col()], PAGE, &cur_page, &max_page) != 2) {
				/* nah, last menu is gone */
				menu = false;
				last_menu.row(-1);
				last_menu.col(-1);
			} else {
				/* still got a menu */
				if (cur_page == -1) {
					/* only 1 page */
					cur_page = 1;
					max_page = 1;
				}
			}
		}
		if (!menu) {
			/* check if we got a new menu */
			msg_str = &data[data_size - sizeof (PAGE_DIRTY)];
			cur_page = -1;
			max_page = -1;
			if (msg_str.find(END, 0) != string::npos || sscanf(msg_str.c_str(), PAGE_DIRTY, &cur_page, &max_page) == 2) {
				/* hot jiggity! we got a list */
				/* now find the "(" in "(end) " or "(x of y)" */
				if (cur_page == -1) {
					/* only 1 page */
					cur_page = 1;
					max_page = 1;
				}
				int c;
				for (c = cursor.col(); c >= 0 && view[cursor.row()][c] != '('; --c)
					;
				menu = true;
				last_menu.row(cursor.row());
				last_menu.col(c);
			}
		}
		if (menu) {
			/* finally parse the menu */
			fetchMenuText(last_menu.row() - 1, last_menu.col(), true); // "r - 1" to avoid the last "(end) " or "(x of y)"
		}
	}
	if (!menu) {
		/* no "--More--", no question and no menu?
		 * well, it gotta be no messages or the message is on 1 line, then */
		msg_str = view[0];
		/* trim */
		string::size_type fns = msg_str.find_first_not_of(" ");
		string::size_type lns = msg_str.find_last_not_of(" ");
		if (fns == string::npos || lns == string::npos || fns >= lns)
			return; // blank line?
		msg_str = msg_str.substr(fns, lns - fns + 1);
		messages.append(msg_str);
		messages.append(2, ' ');
	}
}

void World::handleEscapeSequence(int *pos, int *color) {
	if (data[*pos] == 27) {
		/* sometimes we get 2 escape chars in a row,
		 * just return in those cases */
		return;
	} else if (data[*pos] == '[') {
		int divider = -1;
		int start = *pos;
		for (; *pos < data_size; ++*pos) {
			if (data[*pos] == ';') {
				/* divider for values */
				divider = *pos;
			} else if (data[*pos] == 'A') {
				/* move cursor up */
				if (cursor.row() > 0)
					cursor.moveNorth();
				break;
			} else if (data[*pos] == 'B') {
				/* move cursor down */
				if (cursor.row() < ROWS)
					cursor.moveSouth();
				break;
			} else if (data[*pos] == 'C') {
				/* move cursor right */
				if (cursor.col() < COLS)
					cursor.moveEast();
				break;
			} else if (data[*pos] == 'D') {
				/* move cursor left */
				if (cursor.col() > 0)
					cursor.moveWest();
				break;
			} else if (data[*pos] == 'H') {
				/* set cursor position */
				cursor.row(0);
				cursor.col(0);
				if (divider < 0)
					break;
				/* we got a position */
				int tmprow = cursor.row();
				int tmpcol = cursor.col();
				if (sscanf(&data[start + 1], "%d;%d", &tmprow, &tmpcol) < 2) {
					Debug::error() << WORLD_DEBUG_NAME << "Unable to place cursor: " << &data[start] << endl;
					exit(13);
				}
				cursor.row(--tmprow); // terminal starts counting from 1
				cursor.col(--tmpcol); // ditto ^^
				break;
			} else if (data[*pos] == 'J') {
				/* erase in display */
				if (data[*pos - 1] == '[') {
					/* erase everything below current position */
					for (int r = cursor.row() + 1; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c)
							view[r][c] = ' ';
					}
				} else if (data[*pos - 1] == '1') {
					/* erase everything above current position */
					for (int r = cursor.row() - 1; r >= 0; --r) {
						for (int c = 0; c < COLS; ++c)
							view[r][c] = ' ';
					}
				} else if (data[*pos - 1] == '2') {
					/* erase entire display */
					memset(view, ' ', sizeof (view));
					for (int r = 0; r < ROWS; ++r)
						view[r][COLS] = '\0';
					cursor.row(0);
					cursor.col(0);
					*color = 0;
				} else {
					Debug::error() << WORLD_DEBUG_NAME << "Unhandled sequence: " << &data[*pos] << endl;
					exit(9);
				}
				break;
			} else if (data[*pos] == 'K') {
				/* erase in line */
				if (data[*pos - 1] == '[') {
					/* erase everything to the right */
					for (int c = cursor.col(); c < COLS; ++c)
						view[cursor.row()][c] = ' ';
				} else if (data[*pos - 1] == '1') {
					/* erase everything to the left */
					for (int c = 0; c < cursor.col(); ++c)
						view[cursor.row()][c] = ' ';
				} else if (data[*pos - 1] == '2') {
					/* erase entire line */
					for (int c = 0; c < COLS; ++c)
						view[cursor.row()][c] = ' ';
				} else {
					Debug::error() << WORLD_DEBUG_NAME << "Unhandled sequence: " << &data[*pos] << endl;
					exit(9);
				}
				break;
			} else if (data[*pos] == 'h') {
				/* can possibly be ignored */
				/* probably [?1049h */
				break;
			} else if (data[*pos] == 'l') {
				/* DEC Private Mode Reset? :s */
				break;
			} else if (data[*pos] == 'm') {
				/* character attribute (bold, inverse, color, etc) */
				if (divider > 0) {
					Debug::error() << WORLD_DEBUG_NAME << "Unsupported character color" << &data[*pos] << endl;
					exit(15);
					break;
				}
				*color = 0;
				if (*pos == start + 1)
					break;
				int value = 0;
				int matched = sscanf(&data[start + 1], "%d", &value);
				if (matched < 1) {
					Debug::error() << WORLD_DEBUG_NAME << "Expected numeric value for character attribute: " << &data[*pos] << endl;
					exit(14);
				}
				switch (value) {
				case NO_COLOR:
					bold = false;
					inverse = false;
					break;

				case BOLD:
					bold = true;
					break;

				case INVERSE:
					inverse = true;
					break;

				default:
					if (bold)
						value += BOLD_OFFSET;
					if (inverse)
						value += INVERSE_OFFSET;
					*color = value;
				}
				break;
			} else if (data[*pos] == 'r') {
				/* this is some scrolling crap, ignore it */
				break;
			} else if (data[*pos] == 27) {
				/* escape char found, that shouldn't happen */
				Debug::rawCharArray(data, start, *pos + 1);
				exit(7);
			} else if (*pos - start > 7) {
				/* too long escape sequence? */
				Debug::error() << WORLD_DEBUG_NAME << "Suspiciously long sequence: " << &data[*pos] << endl;
				exit(8);
			}
		}
		if (*pos >= data_size) {
			Debug::error() << WORLD_DEBUG_NAME << "Did not find stop char for sequence: " << data << endl;
			exit(6);
		}
	} else if (data[*pos] == '(') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == ')') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == '*') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == '+') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == 'M') {
		/* reverse linefeed? */
		if (cursor.row() > 0)
			cursor.moveNorth();
	} else if (data[*pos] == '=') {
		/* application numpad?
		 * ignore */
	} else if (data[*pos] == '>') {
		/* normal numpad?
		 * ignore */
	} else {
		Debug::error() << WORLD_DEBUG_NAME << "Unsupported escape sequence code at char " << *pos << ": " << &data[*pos] << endl;
		exit(5);
	}
}

bool World::parseAttributeRow(const char *attributerow) {
	/* fetch attributes */
	int matched = sscanf(attributerow, "%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s", &Saiph::strength, &Saiph::dexterity, &Saiph::constitution, &Saiph::intelligence, &Saiph::wisdom, &Saiph::charisma, effects[0]);
	if (matched < 7)
		return false;
	if (effects[0][0] == 'L')
		Saiph::alignment = LAWFUL;
	else if (effects[0][0] == 'N')
		Saiph::alignment = NEUTRAL;
	else
		Saiph::alignment = CHAOTIC;
	return true;
}

bool World::parseStatusRow(const char *statusrow) {
	/* fetch status */
	Saiph::encumbrance = UNENCUMBERED;
	Saiph::hunger = CONTENT;
	Saiph::blind = false;
	Saiph::confused = false;
	Saiph::foodpoisoned = false;
	Saiph::hallucinating = false;
	Saiph::ill = false;
	Saiph::slimed = false;
	Saiph::stunned = false;
	int matched = sscanf(statusrow, "%16[^$*]%*[^:]:%d%*[^:]:%d(%d%*[^:]:%d(%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s%s%s%s%s", levelname, &Saiph::zorkmids, &Saiph::hitpoints, &Saiph::hitpoints_max, &Saiph::power, &Saiph::power_max, &Saiph::armor_class, &Saiph::experience, &turn, effects[0], effects[1], effects[2], effects[3], effects[4]);
	if (matched < 9)
		return false;
	int effects_found = matched - 9;
	for (int e = 0; e < effects_found; ++e) {
		if (strcmp(effects[e], "Burdened") == 0) {
			Saiph::encumbrance = BURDENED;
		} else if (strcmp(effects[e], "Stressed") == 0) {
			Saiph::encumbrance = STRESSED;
		} else if (strcmp(effects[e], "Strained") == 0) {
			Saiph::encumbrance = STRAINED;
		} else if (strcmp(effects[e], "Overtaxed") == 0) {
			Saiph::encumbrance = OVERTAXED;
		} else if (strcmp(effects[e], "Overloaded") == 0) {
			Saiph::encumbrance = OVERLOADED;
		} else if (strcmp(effects[e], "Fainting") == 0) {
			Saiph::hunger = FAINTING;
		} else if (strcmp(effects[e], "Fainted") == 0) {
			Saiph::hunger = FAINTING;
		} else if (strcmp(effects[e], "Weak") == 0) {
			Saiph::hunger = WEAK;
		} else if (strcmp(effects[e], "Hungry") == 0) {
			Saiph::hunger = HUNGRY;
		} else if (strcmp(effects[e], "Satiated") == 0) {
			Saiph::hunger = SATIATED;
		} else if (strcmp(effects[e], "Oversatiated") == 0) {
			Saiph::hunger = OVERSATIATED;
		} else if (strcmp(effects[e], "Blind") == 0) {
			Saiph::blind = true;
		} else if (strcmp(effects[e], "Conf") == 0) {
			Saiph::confused = true;
		} else if (strcmp(effects[e], "FoodPois") == 0) {
			Saiph::foodpoisoned = true;
		} else if (strcmp(effects[e], "Hallu") == 0) {
			Saiph::hallucinating = true;
		} else if (strcmp(effects[e], "Ill") == 0) {
			Saiph::ill = true;
		} else if (strcmp(effects[e], "Slime") == 0) {
			Saiph::slimed = true;
		} else if (strcmp(effects[e], "Stun") == 0) {
			Saiph::stunned = true;
		}
	}
	return true;
}

void World::update() {
	/* update the view */
	int charcolor = 0; // color of the char
	data_size = connection->retrieve(data, BUFFER_SIZE);
	if (data_size <= 0) {
		/* no data? sleep a sec and try again */
		sleep(1);
		data_size = connection->retrieve(data, BUFFER_SIZE);
		if (data_size <= 0) {
			Debug::error() << "No data received, quitting" << endl;
			exit(42);
		}
	}
	/* print world & data (to cerr, for debugging)
	 * this must be done here because if we get --More-- messages we'll update again */
	/* also, we do this in two loops because otherwise it flickers a lot */
	for (int a = 0; a < data_size; ++a)
		cout << data[a];
	cout.flush(); // same reason as in saiph.dumpMaps()
	Debug::rawCharArray(data, 0, data_size);
	for (int pos = 0; pos < data_size; ++pos) {
		switch (data[pos]) {
		case 0:
			/* sometimes we get lots of \0 characters.
			 * seemingly this happens when certain effects happen.
			 * for example, gas spore explotion, dagger thrown, etc.
			 * let's simply ignore these */
			break;
		case 8:
			/* backspace.
			 * make it go 1 char left */
			if (cursor.col() > 0)
				cursor.moveWest();
			break;

		case 10:
			/* line feed */
			cursor.moveSouth();
			break;

		case 13:
			/* carriage return */
			cursor.col(0);
			break;

		case 14:
			/* shift out, invoke G1 character set */
			break;

		case 15:
			/* shift in, invoke G0 character set */
			break;

		case 27:
			/* escape sequence coming up */
			++pos;
			handleEscapeSequence(&pos, &charcolor);
			break;

		default:
			/* add this char to the view */
			if (cursor.col() >= COLS || cursor.row() >= ROWS || cursor.col() < 0 || cursor.row() < 0) {
				Debug::warning() << WORLD_DEBUG_NAME << "Fell out of the dungeon: " << cursor.row() << ", " << cursor.col() << endl;
				break;
			}
			view[cursor.row()][cursor.col()] = (unsigned char) data[pos];
			color[cursor.row()][cursor.col()] = charcolor;
			addChangedLocation(cursor);
			cursor.moveEast();
			break;
		}
	}

	fetchMessages();

	/* parse attribute & status rows */
	bool parsed_attributes = parseAttributeRow(view[ATTRIBUTES_ROW]);
	bool parsed_status = parseStatusRow(view[STATUS_ROW]);
	/* check that the data we received seems ok */
	if (!menu && !question && (!parsed_attributes || !parsed_status || cursor.row() < MAP_ROW_BEGIN || cursor.row() > MAP_ROW_END || cursor.col() < MAP_COL_BEGIN || cursor.col() > MAP_COL_END)) {
		/* hmm, what else can it be?
		 * could we be missing data?
		 * this is bad, we'll lose messages, this should never happen */
		Debug::warning() << WORLD_DEBUG_NAME << "CURSOR ON UNEXPECTED LOCATION: " << cursor.row() << ", " << cursor.col() << endl;
		update();
		return;
	}
	++frame_count;
	if (messages == "  ")
		messages.clear(); // no messages

	/* check if we get the question where we want to teleport */
	if (messages.find(MESSAGE_FOR_INSTRUCTIONS, 0) != string::npos) {
		/* a bit unique case, this is a question.
		 * the data doesn't end with the sequence we check in World.
		 * however, the cursor is placed on the player when we get this message */
		question = true;
	}

	/* check if we're engulfed */
	if (cursor.row() > MAP_ROW_BEGIN && cursor.row() < MAP_ROW_END && cursor.col() > MAP_COL_BEGIN && cursor.col() < MAP_COL_END && view[cursor.row() - 1][cursor.col() - 1] == '/' && view[cursor.row() - 1][cursor.col() + 1] == '\\' && view[cursor.row() + 1][cursor.col() - 1] == '\\' && view[cursor.row() + 1][cursor.col() + 1] == '/')
		engulfed = true;
	else
		engulfed = false;

	if (!menu && !question && !engulfed)
		detectPosition();
}

/* main */
int main(int argc, const char *argv[]) {
	int connection_type = CONNECTION_TELNET;
	string logfile = "saiph.log";

	bool showUsage = false;
	if (argc > 1) {
		for (int a = 1; a < argc; ++a) {
			if (strlen(argv[a]) < 2) {
				showUsage = true;
				continue;
			}

			if (argv[a][0] == '-') {
				switch (argv[a][1]) {
				case 'h':
					showUsage = true;
					break;
				case 'l':
					connection_type = CONNECTION_LOCAL;
					break;
				case 't':
					connection_type = CONNECTION_TELNET;
					break;
				case 'L':
					if (argc > ++a)
						logfile = argv[a];
					else
						showUsage = true;
					break;
				default:
					cout << "Invalid argument " << argv[a] << endl;
					showUsage = true;
					break;
				}
			} else {
				cout << "Unknown argument specified." << endl;
			}
		}

		if (showUsage) {
			cout << "Usage: " << argv[0] << " [-l|-t] [-L <logfile>]" << endl;
			cout << endl;
			cout << "\t-l  Use local nethack executable" << endl;
			cout << "\t-t  Use telnet nethack server" << endl;
			cout << endl;
			cout << "\t-L <logfile>  Log file to write Saiph output" << endl;
			return 1;
		}
	}

	/* init */
	Debug::init(logfile);
	action::Action::init();
	data::Monster::init();
	data::Item::init();
	event::Event::init();
	World::init(connection_type);
	Analyzer::init();

	/* run */
	World::run();
	Debug::notice() << SAIPH_DEBUG_NAME << "Quitting gracefully" << endl;

	/* destroy */
	Analyzer::destroy();
	World::destroy();
	event::Event::destroy();
	data::Monster::destroy();
	data::Item::destroy();
	action::Action::destroy();
	Debug::destroy();
}
