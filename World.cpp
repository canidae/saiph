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
vector<Point> World::_changes;
char World::_view[ROWS][COLS + 1] = {
	{'\0'}
};
int World::_color[ROWS][COLS] = {
	{0}
};
Point World::_cursor;
int World::_cur_page = -1;
int World::_max_page = -1;
int World::_command_count = 0;
int World::_frame_count = 0;
bool World::_menu = false;
bool World::_question = false;
char World::_levelname[MAX_LEVELNAME_LENGTH] = {'\0'};
int World::_turn = 0;
unsigned int World::_internal_turn = 0;
vector<Level> World::_levels;
Coordinate World::_branch[BRANCHES];

Connection* World::_connection = NULL;
action::Action* World::_action = NULL;
list<action::Action*> World::_action_queue;
bool World::_changed[MAP_ROW_END + 1][MAP_COL_END + 1] = {
	{false}
};
string World::_messages = " ";
bool World::_inverse = false;
bool World::_bold = false;
char World::_data[BUFFER_SIZE * 2] = {'\0'};
int World::_data_size = -1;
string World::_msg_str;
Point World::_last_menu;
map<string, vector<int> > World::_levelmap;
time_t World::_start_time = time(NULL);
vector<Analyzer*> World::_analyzers;
int World::_last_action_id = NO_ACTION;

/* methods */
const char& World::view(const Point& point) {
	return _view[point.row()][point.col()];
}

const int& World::color(const Point& point) {
	return _color[point.row()][point.col()];
}

const bool& World::menu() {
	return _menu;
}

const bool& World::question() {
	return _question;
}

const int& World::curPage() {
	return _cur_page;
}

const int& World::maxPage() {
	return _max_page;
}

const int& World::turn() {
	return _turn;
}

const unsigned int& World::internalTurn() {
	return _internal_turn;
}

const vector<Point>& World::changes() {
	return _changes;
}

Level& World::level() {
	return level(Saiph::position().level());
}

Level& World::level(const int& level) {
	return _levels[level];
}

const int& World::currentPriority() {
	if (_action == NULL)
		return action::Action::NOOP.priority();
	return _action->command().priority();
}

const int& World::lastActionID() {
	/* return the id of the last action */
	return _last_action_id;
}

void World::init(const int& connection_type) {
	_connection = Connection::create(connection_type);
	if (_connection == NULL) {
		cout << "ERROR: Don't know what interface this is: " << connection_type << endl;
		exit(1);
	}
	/* fetch the first "frame" */
	update();
}

void World::destroy() {
	delete _action;
	delete _connection;
	for (vector<Analyzer*>::iterator a = _analyzers.begin(); a != _analyzers.end(); ++a)
		delete *a;
}

void World::registerAnalyzer(Analyzer * const analyzer) {
	Debug::info() << "Registering analyzer " << analyzer->name() << endl;
	_analyzers.push_back(analyzer);
}

void World::unregisterAnalyzer(Analyzer * const analyzer) {
	Debug::info() << "Unregistering analyzer " << analyzer->name() << endl;
	for (vector<Analyzer*>::iterator a = _analyzers.begin(); a != _analyzers.end(); ++a) {
		if ((*a)->name() == analyzer->name()) {
			_analyzers.erase(a);
			return;
		}
	}
}

bool World::setAction(action::Action* action) {
	if (World::_action != NULL) {
		if (action->command().priority() <= World::_action->command().priority()) {
			delete action;
			return false; // already got an action with higher priority
		}
		delete World::_action;
	}
	World::_action = action;
	return true;
}

bool World::queueAction(action::Action* action) {
	if (action == NULL) {
		return false; // shouldn't happen, though
	} else if (action->command().priority() <= PRIORITY_TURN_MAX) {
		/* not a zero-turn action, can't queue it */
		delete action;
		return false;
	}
	_action_queue.push_back(action);
	return true;
}

unsigned char World::directLine(Point point, const bool& ignore_sinks, const bool& ignore_boulders) {
	/* is the target in a direct line from the player? */
	if (point.row() < MAP_ROW_BEGIN || point.row() > MAP_ROW_END || point.col() < MAP_COL_BEGIN || point.col() > MAP_COL_END) {
		/* outside map */
		return ILLEGAL_DIRECTION;
	} else if (point == Saiph::position()) {
		/* eh? don't do this */
		return NOWHERE;
	} else if (point.row() == Saiph::position().row()) {
		/* aligned horizontally */
		if (point.col() > Saiph::position().col()) {
			while (point.moveWest().col() > Saiph::position().col()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return E;
		} else {
			while (point.moveEast().col() < Saiph::position().col()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return W;
		}
	} else if (point.col() == Saiph::position().col()) {
		/* aligned vertically */
		if (point.row() > Saiph::position().row()) {
			while (point.moveNorth().row() > Saiph::position().row()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return S;
		} else {
			while (point.moveSouth().row() < Saiph::position().row()) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return N;
		}
	} else if (abs(point.row() - Saiph::position().row()) == abs(point.col() - Saiph::position().col())) {
		/* aligned diagonally */
		if (point.row() > Saiph::position().row()) {
			if (point.col() > Saiph::position().col()) {
				while (point.moveNorthwest().row() > Saiph::position().row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return SE;
			} else {
				while (point.moveNortheast().row() > Saiph::position().row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return SW;
			}
		} else {
			if (point.col() > Saiph::position().col()) {
				while (point.moveSouthwest().row() < Saiph::position().row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return NE;
			} else {
				while (point.moveSoutheast().row() < Saiph::position().row()) {
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return NW;
			}
		}
	}
	return ILLEGAL_DIRECTION;
}

const Tile& World::shortestPath(const Point& point) {
	/* returns PathNode for shortest path from player to target */
	return level().tile(point);
}

Tile World::shortestPath(const Coordinate& target) {
	/* returns a "fake" Tile with data for shortest path from player to target */
	if (target.level() < 0 || target.level() >= (int) _levels.size())
		return Tile(); // outside map
	else if (target.level() == Saiph::position().level())
		return _levels[Saiph::position().level()].tile(target); // target on same level

	int pivot = -1;
	int level_count = 1;
	int level_queue[_levels.size()];
	level_queue[0] = Saiph::position().level();
	bool level_added[_levels.size()];
	for (int a = 0; a < (int) _levels.size(); ++a)
		level_added[a] = false;
	level_added[Saiph::position().level()] = true;
	Tile level_tile[_levels.size()];
	level_tile[Saiph::position().level()] = shortestPath(Saiph::position());
	Debug::pathing() << "Trying to find path to " << target << endl;
	while (++pivot < level_count) {
		/* check if target is on level */
		if (level_queue[pivot] == target.level()) {
			Tile tile = _levels[level_queue[pivot]].tile(target);
			if (tile.cost() == UNREACHABLE)
				continue;
			else if (tile.cost() == UNPASSABLE && tile.distance() > 1)
				continue;
			/* gotta modify this tile a bit since it's on another level */
			tile.updatePath(target, level_tile[level_queue[pivot]].direction(), tile.distance() + level_tile[level_queue[pivot]].distance() + 1, tile.cost() + level_tile[level_queue[pivot]].cost() + 1);
			Debug::pathing() << "Found " << target << " " << tile.distance() << " tiles away" << endl;
			return tile;
		}
		/* path to upstairs on level */
		for (map<Point, int>::iterator s = _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_UP).begin(); s != _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_UP).end(); ++s) {
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where these stairs lead
			else if (level_added[s->second])
				continue; // already added this level
			const Tile& tile = _levels[level_queue[pivot]].tile(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			Debug::pathing() << "Following upstairs on level " << level_queue[pivot] << " (" << _levels[level_queue[pivot]].name() << ") leading to level " << s->second << " (" << _levels[s->second].name() << ")" << endl;
			/* we know where these stairs lead, add the level to the queue */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to upstairs on level we're standing on */
				level_tile[s->second] = tile;
				if (tile.direction() == NOWHERE)
					level_tile[s->second].direction(UP);
			} else {
				/* pathing to upstairs on another level */
				level_tile[s->second] = level_tile[level_queue[pivot]];
				level_tile[s->second].updatePath(s->first, level_tile[s->second].direction(), level_tile[s->second].distance() + tile.distance() + 1, level_tile[s->second].cost() + tile.cost() + 1);
			}
		}
		/* path to downstairs on level */
		for (map<Point, int>::iterator s = _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_DOWN).begin(); s != _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_DOWN).end(); ++s) {
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where these stairs lead
			else if (level_added[s->second])
				continue; // already added this level
			const Tile& tile = _levels[level_queue[pivot]].tile(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			Debug::pathing() << "Following downstairs on level " << level_queue[pivot] << " (" << _levels[level_queue[pivot]].name() << ") leading to level " << s->second << " (" << _levels[s->second].name() << ")" << endl;
			/* we know where these stairs lead, add the level to the queue */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to downstairs on level we're standing on */
				level_tile[s->second] = tile;
				if (tile.direction() == NOWHERE)
					level_tile[s->second].direction(DOWN);
			} else {
				/* pathing to downstairs on another level */
				level_tile[s->second] = level_tile[level_queue[pivot]];
				level_tile[s->second].updatePath(s->first, level_tile[s->second].direction(), level_tile[s->second].distance() + tile.distance() + 1, level_tile[s->second].cost() + tile.cost() + 1);
			}
		}
		/* path to portals on level */
		for (map<Point, int>::iterator s = _levels[level_queue[pivot]].symbols((unsigned char) MAGIC_PORTAL).begin(); s != _levels[level_queue[pivot]].symbols((unsigned char) MAGIC_PORTAL).end(); ++s) {
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where this portal leads
			else if (level_added[s->second])
				continue; // already added this level
			const Tile& tile = _levels[level_queue[pivot]].tile(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			Debug::info() << "Following magic portal on level " << level_queue[pivot] << " (" << _levels[level_queue[pivot]].name() << ") leading to level " << s->second << " (" << _levels[s->second].name() << ")" << endl;
			/* we know where this portal leads, add the level to the queue */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to portal on level we're standing on */
				level_tile[s->second] = tile;
				if (tile.direction() == NOWHERE)
					level_tile[s->second].direction(DOWN);
			} else {
				/* pathing to portal on another level */
				level_tile[s->second] = level_tile[level_queue[pivot]];
				level_tile[s->second].updatePath(s->first, level_tile[s->second].direction(), level_tile[s->second].distance() + tile.distance() + 1, level_tile[s->second].cost() + tile.cost() + 1);
			}
		}
	}
	return Tile(); // symbol not found
}

Tile World::shortestPath(const unsigned char& symbol) {
	/* returns PathNode for shortest path from player to nearest symbol */
	int pivot = -1;
	int level_count = 1;
	Tile best_tile;
	int level_queue[_levels.size()];
	level_queue[0] = Saiph::position().level();
	bool level_added[_levels.size()];
	for (int a = 0; a < (int) _levels.size(); ++a)
		level_added[a] = false;
	level_added[Saiph::position().level()] = true;
	Tile level_tile[_levels.size()];
	level_tile[Saiph::position().level()] = shortestPath(Saiph::position());
	Debug::pathing() << "Trying to find path to nearest '" << symbol << "'" << endl;
	while (++pivot < level_count) {
		/* path to symbols on level */
		for (map<Point, int>::iterator s = _levels[level_queue[pivot]].symbols(symbol).begin(); s != _levels[level_queue[pivot]].symbols(symbol).end(); ++s) {
			const Tile& tile = _levels[level_queue[pivot]].tile(s->first);
			if (tile.cost() == UNREACHABLE)
				continue;
			else if (tile.cost() == UNPASSABLE && tile.distance() > 1)
				continue;
			else if (tile.cost() + level_tile[level_queue[pivot]].cost() + 1 >= best_tile.cost())
				continue;
			/* this symbol is closer than the previously found one */
			best_tile = tile;
			if (pivot != 0) {
				/* symbol is on another level, gotta modify this pathnode a bit */
				best_tile.updatePath(s->first, level_tile[level_queue[pivot]].direction(), tile.distance() + level_tile[level_queue[pivot]].distance() + 1, tile.cost() + level_tile[level_queue[pivot]].cost() + 1);
			}
			Debug::pathing() << "Found '" << symbol << "' at " << Coordinate(level_queue[pivot], s->first) << ", " << tile.distance() << " tiles away" << endl;
		}
		/* path to upstairs on level */
		for (map<Point, int>::iterator s = _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_UP).begin(); s != _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_UP).end(); ++s) {
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where these stairs lead
			if (level_added[s->second])
				continue; // already added this level
			const Tile& tile = _levels[level_queue[pivot]].tile(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.cost() + level_tile[level_queue[pivot]].cost() + 1 >= best_tile.cost())
				continue;
			Debug::pathing() << "Following upstairs on level " << level_queue[pivot] << " (" << _levels[level_queue[pivot]].name() << ") leading to level " << s->second << " (" << _levels[s->second].name() << ")" << endl;
			/* distance to these stairs is shorter than shortest path found so far.
			 * we should check the level these stairs lead to as well */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to upstairs on level we're standing on */
				level_tile[s->second] = tile;
				if (tile.direction() == NOWHERE)
					level_tile[s->second].direction(UP);
			} else {
				/* pathing to upstairs on another level */
				level_tile[s->second] = level_tile[level_queue[pivot]];
				level_tile[s->second].updatePath(s->first, level_tile[s->second].direction(), level_tile[s->second].distance() + tile.distance() + 1, level_tile[s->second].cost() + tile.cost() + 1);
			}
		}
		/* path to downstairs on level */
		for (map<Point, int>::iterator s = _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_DOWN).begin(); s != _levels[level_queue[pivot]].symbols((unsigned char) STAIRS_DOWN).end(); ++s) {
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where these stairs lead
			if (level_added[s->second])
				continue; // already added this level
			const Tile& tile = _levels[level_queue[pivot]].tile(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.cost() + level_tile[level_queue[pivot]].cost() + 1 >= best_tile.cost())
				continue;
			Debug::pathing() << "Following downstairs on level " << level_queue[pivot] << " (" << _levels[level_queue[pivot]].name() << ") leading to level " << s->second << " (" << _levels[s->second].name() << ")" << endl;
			/* distance to these stairs is shorter than shortest path found so far.
			 * we should check the level these stairs lead to as well */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to downstairs on level we're standing on */
				level_tile[s->second] = tile;
				if (tile.direction() == NOWHERE)
					level_tile[s->second].direction(DOWN);
			} else {
				/* pathing to downstairs on another level */
				level_tile[s->second] = level_tile[level_queue[pivot]];
				level_tile[s->second].updatePath(s->first, level_tile[s->second].direction(), level_tile[s->second].distance() + tile.distance() + 1, level_tile[s->second].cost() + tile.cost() + 1);
			}
		}
		/* path to levels through magic portals */
		for (map<Point, int>::iterator s = _levels[level_queue[pivot]].symbols((unsigned char) MAGIC_PORTAL).begin(); s != _levels[level_queue[pivot]].symbols((unsigned char) MAGIC_PORTAL).end(); ++s) {
			if (s->second == UNKNOWN_SYMBOL_VALUE)
				continue; // we don't know where this magic portal leads
			if (level_added[s->second])
				continue; // already added this level
			const Tile& tile = _levels[level_queue[pivot]].tile(s->first);
			if (tile.cost() >= UNPASSABLE)
				continue;
			else if (tile.cost() + level_tile[level_queue[pivot]].cost() >= best_tile.cost())
				continue;
			Debug::info() << "Following magic portal on level " << level_queue[pivot] << " (" << _levels[level_queue[pivot]].name() << ") leading to level " << s->second << " (" << _levels[s->second].name() << ")" << endl;
			/* distance to this portal is shorter than shortest path found so far.
			 * we should check the level this portal leads to as well */
			level_added[s->second] = true;
			level_queue[level_count++] = s->second;
			if (pivot == 0) {
				/* pathing to downstairs on level we're standing on */
				level_tile[s->second] = tile;
				if (tile.direction() == NOWHERE)
					level_tile[s->second].direction(NOWHERE);
			} else {
				/* pathing to downstairs on another level */
				level_tile[s->second] = level_tile[level_queue[pivot]];
				level_tile[s->second].updatePath(s->first, level_tile[s->second].direction(), level_tile[s->second].distance() + tile.distance() + 1, level_tile[s->second].cost() + tile.cost() + 1);
			}
		}
	}
	return best_tile;
}

void World::run() {
	int last_turn = 0;
	int stuck_counter = 0;
	while (true) {
		/* check if we're in the middle of an action
		 * Inventory and Level may send events that analyzers react on and set an action,
		 * so we check if we're in the middle of an action here and remember it for later */
		bool analyze_and_parse = (_action == NULL || _action->command() == action::Action::NOOP);

		/* let Saiph, Inventory and current level parse messages */
		Saiph::parseMessages(_messages);
		Inventory::parseMessages(_messages);
		level().parseMessages(_messages);

		/* let Saiph, Inventory and current level analyze */
		Saiph::analyze();
		Inventory::analyze();
		level().analyze();

		/* dump maps */
		dumpMaps();

		/* analyze and parse messages if we're not continuing an action */
		if (analyze_and_parse) {
			/* let analyzers parse messages */
			for (vector<Analyzer*>::iterator a = _analyzers.begin(); a != _analyzers.end(); ++a)
				(*a)->parseMessages(_messages);

			/* let analyzers analyze */
			if (!_question && !_menu) {
				for (vector<Analyzer*>::iterator a = _analyzers.begin(); a != _analyzers.end(); ++a)
					(*a)->analyze();
			}
		}

		/* check if we got some queued actions */
		for (list<action::Action*>::iterator a = _action_queue.begin(); a != _action_queue.end(); ++a) {
			if (setAction(*a)) {
				/* we will execute this action, remove it from queue.
				 * if it fails, the analyzer that queued the action needs to handle it */
				_action_queue.erase(a);
				break;
			}
		}

		/* check if we got a command */
		if (_action == NULL || _action->command() == action::Action::NOOP) {
			/* we do not. print debugging and just answer something sensible */
			if (_question) {
				Debug::warning() << "Unhandled question: " << _messages << endl;
				_last_action_id = NO_ACTION;
				executeCommand(string(1, (char) 27));
				continue;
			} else if (_menu) {
				Debug::warning() << "Unhandled menu: " << _messages << endl;
				_last_action_id = NO_ACTION;
				executeCommand(string(1, (char) 27));
				continue;
			} else {
				Debug::warning() << "I have no idea what to do... Searching" << endl;
				cout << (unsigned char) 27 << "[1;82H";
				cout << (unsigned char) 27 << "[K"; // erase everything to the right
				cout << "No idea what to do: s";
				/* return cursor back to where it was */
				cout << (unsigned char) 27 << "[" << _cursor.row() + 1 << ";" << _cursor.col() + 1 << "H";
				cout.flush();
				++World::_internal_turn; // command than may increase turn counter
				_last_action_id = NO_ACTION;
				executeCommand("s");
				continue;
			}
		}

		/* print what we're doing */
		cout << (unsigned char) 27 << "[1;82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
		cout << _action->analyzer()->name() << " " << _action->command();
		/* return cursor back to where it was */
		cout << (unsigned char) 27 << "[" << _cursor.row() + 1 << ";" << _cursor.col() + 1 << "H";
		/* and flush cout. if we don't do this our output looks like garbage */
		cout.flush();
		Debug::command() << "Analyzer " << _action->analyzer()->name() << " " << _action->command() << endl;

		/* execute the command */
		if (_action->command().priority() <= PRIORITY_TURN_MAX)
			++World::_internal_turn; // command that may increase turn counter
		_last_action_id = _action->id();
		executeCommand(_action->command().command());

		/* check if we're stuck */
		if (_action != NULL && stuck_counter % 42 == 41 && _action->command().command().size() == 1) {
			bool was_move = false;
			/* we'll assume we're moving if the command that's stuck is a direction.
			 * if not, it's probably not a big deal */
			switch (_action->command().command()[0]) {
			case NW:
			case NE:
			case SW:
			case SE:
				/* moving diagonally failed.
				 * we could be trying to move diagonally into a door we're
				 * unaware of because of an item blocking the door symbol.
				 * make the tile UNKNOWN_TILE_DIAGONALLY_UNPASSABLE */
				level().setDungeonSymbol(directionToPoint((unsigned char) _action->command().command()[0]), UNKNOWN_TILE_DIAGONALLY_UNPASSABLE);
				was_move = true;
				break;

			case N:
			case E:
			case S:
			case W:
				/* moving cardinally failed, possibly item in wall.
				 * make the tile UNKNOWN_TILE_UNPASSABLE */
				level().setDungeonSymbol(directionToPoint((unsigned char) _action->command().command()[0]), UNKNOWN_TILE_UNPASSABLE);
				was_move = true;
				break;

			default:
				/* certainly not moving */
				break;
			}
			if (!was_move) {
				/* not good. we're not moving and we're stuck */
				Debug::warning() << "Command failed for analyzer " << _action->analyzer()->name() << ": " << _action->command() << endl;
			}
		} else if (stuck_counter > 1680) {
			/* failed too many times, #quit */
			Debug::error() << "Appear to be stuck, quitting game" << endl;
			_last_action_id = NO_ACTION;
			executeCommand(string(1, (char) 27));
			executeCommand(QUIT);
			executeCommand(string(1, YES));
			return;
		}
		if (last_turn == _turn)
			stuck_counter++;
		else
			stuck_counter = 0;
		last_turn = _turn;

		/* and finally update current action */
		if (_action != NULL)
			_action->update(_messages);
	}
}

/* private methods */
void World::addChangedLocation(const Point& point) {
	/* add a location changed since last frame unless it's already added */
	if (point.row() < MAP_ROW_BEGIN || point.row() > MAP_ROW_END || point.col() < MAP_COL_BEGIN || point.col() > MAP_COL_END || _changed[point.row()][point.col()])
		return;
	_changes.push_back(point);
}

void World::detectPosition() {
	if (Saiph::position().level() < 0) {
		/* this happens when we start */
		Saiph::position(Coordinate(_levels.size(), _cursor));
		_branch[BRANCH_MAIN] = Saiph::position();
		_levels.push_back(Level(_levels.size(), _levelname));
		_levelmap[_levelname].push_back(Saiph::position().level());
		return;
	}
	if ((int) _levels.size() > Saiph::position().level() && _levelname == _levels[Saiph::position().level()].name()) {
		/* same level as last frame, update row & col */
		Saiph::position(Coordinate(Saiph::position().level(), _cursor));
		if (_branch[BRANCH_SOKOBAN].level() == -1 && _levels[Saiph::position().level()].branch() == BRANCH_MAIN && _levels[Saiph::position().level()].depth() >= 5 && _levels[Saiph::position().level()].depth() <= 9) {
			/* look for sokoban level 1a or 1b */
			if (level().tile(Point(8, 37)).symbol() == BOULDER && level().tile(Point(8, 38)).symbol() == BOULDER && level().tile(Point(8, 43)).symbol() == BOULDER && level().tile(Point(9, 38)).symbol() == BOULDER && level().tile(Point(9, 39)).symbol() == BOULDER && level().tile(Point(9, 42)).symbol() == BOULDER && level().tile(Point(9, 44)).symbol() == BOULDER && level().tile(Point(11, 41)).symbol() == BOULDER && level().tile(Point(14, 39)).symbol() == BOULDER && level().tile(Point(14, 40)).symbol() == BOULDER && level().tile(Point(14, 41)).symbol() == BOULDER && level().tile(Point(14, 42)).symbol() == BOULDER) {
				/* sokoban 1a */
				Debug::notice() << "Found Sokoban level 1a: " << Saiph::position() << endl;
				_levels[Saiph::position().level()].branch(BRANCH_SOKOBAN);
				_branch[BRANCH_SOKOBAN] = Saiph::position();
			} else if (level().tile(Point(8, 34)).symbol() == BOULDER && level().tile(Point(8, 42)).symbol() == BOULDER && level().tile(Point(9, 34)).symbol() == BOULDER && level().tile(Point(9, 41)).symbol() == BOULDER && level().tile(Point(10, 42)).symbol() == BOULDER && level().tile(Point(13, 40)).symbol() == BOULDER && level().tile(Point(14, 41)).symbol() == BOULDER && level().tile(Point(15, 41)).symbol() == BOULDER && level().tile(Point(16, 40)).symbol() == BOULDER && level().tile(Point(16, 42)).symbol() == BOULDER) {
				/* sokoban 1b */
				Debug::notice() << "Found Sokoban level 1b: " << Saiph::position() << endl;
				_levels[Saiph::position().level()].branch(BRANCH_SOKOBAN);
				_branch[BRANCH_SOKOBAN] = Saiph::position();
			}

		}
		if (_levels[Saiph::position().level()].branch() == BRANCH_MAIN && _levels[Saiph::position().level()].depth() >= 3 && _levels[Saiph::position().level()].depth() <= 5) {
			/* if mines are not found and depth is between 3 & 5, we should attempt to detect mines */
			for (map<Point, int>::iterator hw = _levels[Saiph::position().level()].symbols((unsigned char) HORIZONTAL_WALL).begin(); hw != _levels[Saiph::position().level()].symbols((unsigned char) HORIZONTAL_WALL).end(); ++hw) {
				if (hw->first.row() <= MAP_ROW_BEGIN || hw->first.row() >= MAP_ROW_END || hw->first.col() <= MAP_COL_BEGIN || hw->first.col() >= MAP_COL_END)
					continue;
				/* if we see horizontal walls adjacent to this point (except west & east),
				 * then we're in the mines */
				if (level().tile(Point(hw->first.row() - 1, hw->first.col() - 1)).symbol() == HORIZONTAL_WALL || level().tile(Point(hw->first.row() - 1, hw->first.col())).symbol() == HORIZONTAL_WALL || level().tile(Point(hw->first.row() - 1, hw->first.col() + 1)).symbol() == HORIZONTAL_WALL || level().tile(Point(hw->first.row() + 1, hw->first.col() - 1)).symbol() == HORIZONTAL_WALL || level().tile(Point(hw->first.row() + 1, hw->first.col())).symbol() == HORIZONTAL_WALL || level().tile(Point(hw->first.row() + 1, hw->first.col() + 1)).symbol() == HORIZONTAL_WALL) {
					/* we're in the mines */
					Debug::notice() << "Found the mines: " << Saiph::position() << endl;
					_levels[Saiph::position().level()].branch(BRANCH_MINES);
					_branch[BRANCH_MINES] = Saiph::position();
					break;
				}
			}
		}
		if (_levels[Saiph::position().level()].branch() != BRANCH_ROGUE && _view[STATUS_ROW][8] == '*') {
			/* rogue level, set branch attribute */
			Debug::notice() << "Found the rogue level: " << Saiph::position() << endl;
			_levels[Saiph::position().level()].branch(BRANCH_ROGUE);
		}
		return;
	}
	/* level has changed.
	 * we need to figure out if it's a new level or one we already know of */
	int found = UNKNOWN_SYMBOL_VALUE;
	unsigned char symbol = level().tile().symbol();
	/* maybe we already know where these stairs lead? */
	if (symbol == STAIRS_DOWN) {
		/* we did stand on stairs down, and if we don't know where they lead then
		 * the next line will still just set found to UNKNOWN_SYMBOL_VALUE */
		found = _levels[Saiph::position().level()].symbols((unsigned char) STAIRS_DOWN)[Saiph::position()];
	} else if (symbol == STAIRS_UP) {
		/* we did stand on stairs up, and if we don't know where they lead then
		 * the next line will still just set found to UNKNOWN_SYMBOL_VALUE */
		found = _levels[Saiph::position().level()].symbols((unsigned char) STAIRS_UP)[Saiph::position()];
	} else if (symbol == MAGIC_PORTAL) {
		/* we did stand on a magic portal, and if we don't know where it leads then
		 * the next line will still just set found to UNKNOWN_SYMBOL_VALUE */
		found = _levels[Saiph::position().level()].symbols((unsigned char) MAGIC_PORTAL)[Saiph::position()];
	}
	if (found == UNKNOWN_SYMBOL_VALUE) {
		/* we didn't know where the stairs would take us */
		for (vector<int>::iterator lm = _levelmap[_levelname].begin(); lm != _levelmap[_levelname].end(); ++lm) {
			/* check if level got walls on same locations.
			 * since walls can disappear, we'll allow a 80% match */
			int total = 0;
			int matched = 0;
			for (map<Point, int>::iterator s = _levels[*lm].symbols((unsigned char) VERTICAL_WALL).begin(); s != _levels[*lm].symbols((unsigned char) VERTICAL_WALL).end(); ++s) {
				if (_view[s->first.row()][s->first.col()] == VERTICAL_WALL)
					++matched;
				++total;
			}
			for (map<Point, int>::iterator s = _levels[*lm].symbols((unsigned char) HORIZONTAL_WALL).begin(); s != _levels[*lm].symbols((unsigned char) HORIZONTAL_WALL).end(); ++s) {
				if (_view[s->first.row()][s->first.col()] == HORIZONTAL_WALL)
					++matched;
				++total;
			}
			if (matched > 0 && min(matched, total) * 5 >= max(matched, total) * 4) {
				found = *lm;
				Debug::notice() << "Recognized level " << found << ": '" << _levelname << "' - '" << _levels[found].name() << "'" << endl;
				break;
			}
		}
	}
	if (found == UNKNOWN_SYMBOL_VALUE) {
		/* new level */
		found = _levels.size();
		/* when we discover a new level it's highly likely it's in the
		 * same branch as the previous level.
		 * exception is rogue level, which really isn't a branch */
		_levels.push_back(Level(_levels.size(), _levelname, (_levels[Saiph::position().level()].branch() != BRANCH_ROGUE) ? _levels[Saiph::position().level()].branch() : BRANCH_MAIN));
		_levelmap[_levelname].push_back(found);
		Debug::notice() << "Found new level " << found << ": " << _levelname << endl;
	}
	/* were we on stairs on last Saiph::position()? */
	if (symbol == STAIRS_DOWN) {
		/* yes, we were on stairs down */
		_levels[Saiph::position().level()].symbols((unsigned char) STAIRS_DOWN)[Saiph::position()] = found;
	} else if (symbol == STAIRS_UP) {
		/* yes, we were on stairs up */
		_levels[Saiph::position().level()].symbols((unsigned char) STAIRS_UP)[Saiph::position()] = found;
	} else if (symbol == MAGIC_PORTAL) {
		/* yes, we were on a magic portal */
		_levels[Saiph::position().level()].symbols((unsigned char) MAGIC_PORTAL)[Saiph::position()] = found;
	}
	Saiph::position(Coordinate(found, _cursor));
}

Point World::directionToPoint(const unsigned char& direction) {
	/* return the position we'd be at if we do the given move */
	Point pos = Saiph::position();
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

bool World::directLineHelper(const Point& point, const bool& ignore_sinks, const bool& ignore_boulders) {
	const Tile& t = level().tile(point);
	if (!Level::isPassable(t.symbol()) && (!ignore_boulders || t.symbol() != BOULDER))
		return false;
	else if (!ignore_sinks && t.symbol() == SINK)
		return false;
	else if (t.monster() != ILLEGAL_MONSTER && _levels[Saiph::position().level()].monsters()[point].visible())
		return false;
	return true;
}

void World::dumpMaps() {
	/* XXX: World echoes output from the game in the top left corner */
	/* commands/frames/turns per second */
	int seconds = (int) difftime(time(NULL), _start_time);
	if (seconds == 0)
		++seconds;
	int cps = _command_count / seconds;
	int fps = _frame_count / seconds;
	int tps = _turn / seconds;
	cout << (unsigned char) 27 << "[25;1H";
	cout << "CPS/FPS/TPS: ";
	cout << (unsigned char) 27 << "[34m" << cps << (unsigned char) 27 << "[0m/";
	cout << (unsigned char) 27 << "[35m" << fps << (unsigned char) 27 << "[0m/";
	cout << (unsigned char) 27 << "[36m" << tps << (unsigned char) 27 << "[0m      ";

	/* monsters and map as saiph sees it */
	Point p;
	for (p.row(MAP_ROW_BEGIN); p.row() <= MAP_ROW_END; p.moveSouth()) {
		cout << (unsigned char) 27 << "[" << p.row() + 26 << ";2H";
		for (p.col(MAP_COL_BEGIN); p.col() <= MAP_COL_END; p.moveEast()) {
			if (!p.insideMap())
				continue;
			const Tile& t = level().tile(p);
			if (p.row() == Saiph::position().row() && p.col() == Saiph::position().col())
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (t.monster() != ILLEGAL_MONSTER)
				cout << t.monster();
			else
				cout << t.symbol();
		}
	}

	/* path map */
	/*
	for (p.row = MAP_ROW_BEGIN; p.row <= MAP_ROW_END; ++p.row) {
		cout << (unsigned char) 27 << "[" << p.row + 26 << ";2H";
		for (p.col = MAP_COL_BEGIN; p.col <= MAP_COL_END; ++p.col) {
			if (p.row == postion.row && p.col == Saiph::position().col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (levels[Saiph::position().level].pathmap[p.row][p.col].dir != ILLEGAL_DIRECTION)
				//cout << (unsigned char) levels[Saiph::position().level].pathmap[p.row][p.col].dir;
				cout << (char) (levels[Saiph::position().level].pathmap[p.row][p.col].cost % 64 + 48);
			else
				cout << getDungeonSymbol(p);
		}
	}
	 */

	/* status & inventory */
	cout << (unsigned char) 27 << "[2;82H";
	if (Saiph::intrinsics() & PROPERTY_COLD)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[34m" << "Cold " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_DISINT)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[35m" << "DisInt " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_FIRE)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[31m" << "Fire " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_POISON)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[32m" << "Poison " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_SHOCK)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[36m" << "Shock " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_SLEEP)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[33m" << "Sleep " << (unsigned char) 27 << "[m";

	cout << (unsigned char) 27 << "[3;82H";
	if (Saiph::intrinsics() & PROPERTY_ESP)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[35m" << "ESP " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_TELEPORT_CONTROL)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[36m" << "TeleCon " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_TELEPORT)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[33m" << "Teleport " << (unsigned char) 27 << "[m";
	if (Saiph::intrinsics() & PROPERTY_LYCANTHROPY)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[31m" << "Lycan " << (unsigned char) 27 << "[m";
	if (Saiph::hurtLeg())
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[34m" << "Leg " << (unsigned char) 27 << "[m";
	if (Saiph::polymorphed())
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[32m" << "Poly " << (unsigned char) 27 << "[m";

	int ir = 0;
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end() && ir < 46; ++i) {
		cout << (unsigned char) 27 << "[" << (4 + ir) << ";82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
		if (i->second.beatitude() == BLESSED)
			cout << (unsigned char) 27 << "[32m";
		else if (i->second.beatitude() == CURSED)
			cout << (unsigned char) 27 << "[31m";
		else if (i->second.beatitude() == UNCURSED)
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

bool World::executeCommand(const string& command) {
	/* send a command to nethack */
	for (vector<Point>::iterator c = _changes.begin(); c != _changes.end(); ++c)
		_changed[c->row()][c->col()] = false;
	_changes.clear();
	_messages = "  "; // we want 2 spaces before the first message too
	if (command.size() <= 0) {
		/* huh? no command? */
		return false;
	}
	_connection->transmit(command);
	++_command_count;
	update();
	return true;
}

void World::fetchMenuText(const int& stoprow, const int& startcol, const bool& addspaces) {
	/* fetch text from a menu */
	for (int r = 0; r <= stoprow; ++r) {
		_msg_str = &_view[r][startcol];
		/* trim */
		string::size_type fns = _msg_str.find_first_not_of(" ");
		string::size_type lns = _msg_str.find_last_not_of(" ");
		if (fns == string::npos || lns == string::npos || fns >= lns)
			continue; // blank line?
		_msg_str = _msg_str.substr(fns, lns - fns + 1);
		if (addspaces)
			_msg_str.append(2, ' '); // append 2 spaces for later splitting
		_messages.append(_msg_str);
	}
}

void World::fetchMessages() {
	/* even yet a try on fetching messages sanely */
	_question = false; // we can do this as a question max last 1 turn
	_msg_str = &_data[_data_size - sizeof (MORE)];
	string::size_type pos = string::npos;
	if ((pos = _msg_str.find(MORE, 0)) != string::npos) {
		/* "--More--" found */
		_menu = false; // we don't have a menu then
		int r = _cursor.row();
		int c = _cursor.col() - sizeof (MORE) + 1; // +1 because sizeof (MORE) is 9, not 8
		if (r == 0) {
			/* only one line, remove "--More--" from end of line */
			_msg_str = _view[r];
			_msg_str = _msg_str.substr(0, c);
			/* append 2 spaces for later splitting */
			_msg_str.append(2, ' ');
			_messages.append(_msg_str);
		} else {
			/* more than 1 line */
			if (c == 0 || _view[r][c - 1] != ' ') {
				/* this is just a very long line, not a list */
				c = 0;
				fetchMenuText(r, c, false);
				_messages.erase(_messages.size() - sizeof (MORE) + 1); // remove "--More--"
				_messages.append(2, ' '); // add two spaces
			} else {
				/* this is a list */
				/* sometimes "--More--" is placed 1 char to the right of the menu.
				 * this happens at least when the entire page is filled.
				 * check that the line above also is ' ', if not, c - 1 */
				if (_view[r - 1][c - 1] == ' ')
					fetchMenuText(r - 1, c, true); // "r - 1" to avoid the last "--More--"
				else
					fetchMenuText(r - 1, c - 1, true); // "r - 1" to avoid the last "--More--"
			}
		}
		/* request the remaining messages */
		_connection->transmit(" ");
		++_command_count;
		update();
		return;
	} else if (_cursor.row() == 0) {
		/* looks like we got a question.
		 * we might want to significantly improve this later,
		 * as we sometimes get partial data */
		_question = true;
		_menu = false; // no menu when we got a question
	} else {
		/* --More-- not found, but we might have a menu.
		 * this is pain */
		if (_menu) {
			/* we had a menu last frame, check if we still do */
			_msg_str = &_view[_last_menu.row()][_last_menu.col()];
			_cur_page = -1;
			_max_page = -1;
			if (_msg_str.find(END, 0) == string::npos && sscanf(&_view[_last_menu.row()][_last_menu.col()], PAGE, &_cur_page, &_max_page) != 2) {
				/* nah, last menu is gone */
				_menu = false;
				_last_menu.row(-1);
				_last_menu.col(-1);
			} else {
				/* still got a menu */
				if (_cur_page == -1) {
					/* only 1 page */
					_cur_page = 1;
					_max_page = 1;
				}
			}
		}
		if (!_menu) {
			/* check if we got a new menu */
			_msg_str = &_data[_data_size - sizeof (PAGE_DIRTY)];
			_cur_page = -1;
			_max_page = -1;
			if (_msg_str.find(END, 0) != string::npos || sscanf(_msg_str.c_str(), PAGE_DIRTY, &_cur_page, &_max_page) == 2) {
				/* hot jiggity! we got a list */
				/* now find the "(" in "(end) " or "(x of y)" */
				if (_cur_page == -1) {
					/* only 1 page */
					_cur_page = 1;
					_max_page = 1;
				}
				int c;
				for (c = _cursor.col(); c >= 0 && _view[_cursor.row()][c] != '('; --c)
					;
				_menu = true;
				_last_menu.row(_cursor.row());
				_last_menu.col(c);
			}
		}
		if (_menu) {
			/* finally parse the menu */
			fetchMenuText(_last_menu.row() - 1, _last_menu.col(), true); // "r - 1" to avoid the last "(end) " or "(x of y)"
		}
	}
	if (!_menu) {
		/* no "--More--", no question and no menu?
		 * well, it gotta be no messages or the message is on 1 line, then */
		_msg_str = _view[0];
		/* trim */
		string::size_type fns = _msg_str.find_first_not_of(" ");
		string::size_type lns = _msg_str.find_last_not_of(" ");
		if (fns == string::npos || lns == string::npos || fns >= lns)
			return; // blank line?
		_msg_str = _msg_str.substr(fns, lns - fns + 1);
		_messages.append(_msg_str);
		_messages.append(2, ' ');
	}
}

void World::handleEscapeSequence(int* pos, int* color) {
	if (_data[*pos] == 27) {
		/* sometimes we get 2 escape chars in a row,
		 * just return in those cases */
		return;
	} else if (_data[*pos] == '[') {
		int divider = -1;
		int start = *pos;
		for (; *pos < _data_size; ++*pos) {
			if (_data[*pos] == ';') {
				/* divider for values */
				divider = *pos;
			} else if (_data[*pos] == 'A') {
				/* move cursor up */
				if (_cursor.row() > 0)
					_cursor.moveNorth();
				break;
			} else if (_data[*pos] == 'B') {
				/* move cursor down */
				if (_cursor.row() < ROWS)
					_cursor.moveSouth();
				break;
			} else if (_data[*pos] == 'C') {
				/* move cursor right */
				if (_cursor.col() < COLS)
					_cursor.moveEast();
				break;
			} else if (_data[*pos] == 'D') {
				/* move cursor left */
				if (_cursor.col() > 0)
					_cursor.moveWest();
				break;
			} else if (_data[*pos] == 'H') {
				/* set cursor position */
				_cursor.row(0);
				_cursor.col(0);
				if (divider < 0)
					break;
				/* we got a position */
				int tmprow = _cursor.row();
				int tmpcol = _cursor.col();
				if (sscanf(&_data[start + 1], "%d;%d", &tmprow, &tmpcol) < 2) {
					Debug::error() << WORLD_DEBUG_NAME << "Unable to place cursor: " << &_data[start] << endl;
					exit(13);
				}
				_cursor.row(--tmprow); // terminal starts counting from 1
				_cursor.col(--tmpcol); // ditto ^^
				break;
			} else if (_data[*pos] == 'J') {
				/* erase in display */
				if (_data[*pos - 1] == '[') {
					/* erase everything below current position */
					for (int r = _cursor.row() + 1; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c)
							_view[r][c] = ' ';
					}
				} else if (_data[*pos - 1] == '1') {
					/* erase everything above current position */
					for (int r = _cursor.row() - 1; r >= 0; --r) {
						for (int c = 0; c < COLS; ++c)
							_view[r][c] = ' ';
					}
				} else if (_data[*pos - 1] == '2') {
					/* erase entire display */
					memset(_view, ' ', sizeof (_view));
					for (int r = 0; r < ROWS; ++r)
						_view[r][COLS] = '\0';
					_cursor.row(0);
					_cursor.col(0);
					*color = 0;
				} else {
					Debug::error() << WORLD_DEBUG_NAME << "Unhandled sequence: " << &_data[*pos] << endl;
					exit(9);
				}
				break;
			} else if (_data[*pos] == 'K') {
				/* erase in line */
				if (_data[*pos - 1] == '[') {
					/* erase everything to the right */
					for (int c = _cursor.col(); c < COLS; ++c)
						_view[_cursor.row()][c] = ' ';
				} else if (_data[*pos - 1] == '1') {
					/* erase everything to the left */
					for (int c = 0; c < _cursor.col(); ++c)
						_view[_cursor.row()][c] = ' ';
				} else if (_data[*pos - 1] == '2') {
					/* erase entire line */
					for (int c = 0; c < COLS; ++c)
						_view[_cursor.row()][c] = ' ';
				} else {
					Debug::error() << WORLD_DEBUG_NAME << "Unhandled sequence: " << &_data[*pos] << endl;
					exit(9);
				}
				break;
			} else if (_data[*pos] == 'h') {
				/* can possibly be ignored */
				/* probably [?1049h */
				break;
			} else if (_data[*pos] == 'l') {
				/* DEC Private Mode Reset? :s */
				break;
			} else if (_data[*pos] == 'm') {
				/* character attribute (bold, inverse, color, etc) */
				if (divider > 0) {
					Debug::error() << WORLD_DEBUG_NAME << "Unsupported character color" << &_data[*pos] << endl;
					exit(15);
					break;
				}
				*color = 0;
				if (*pos == start + 1)
					break;
				int value = 0;
				int matched = sscanf(&_data[start + 1], "%d", &value);
				if (matched < 1) {
					Debug::error() << WORLD_DEBUG_NAME << "Expected numeric value for character attribute: " << &_data[*pos] << endl;
					exit(14);
				}
				switch (value) {
				case NO_COLOR:
					_bold = false;
					_inverse = false;
					break;

				case BOLD:
					_bold = true;
					break;

				case INVERSE:
					_inverse = true;
					break;

				default:
					if (_bold)
						value += BOLD_OFFSET;
					if (_inverse)
						value += INVERSE_OFFSET;
					*color = value;
				}
				break;
			} else if (_data[*pos] == 'r') {
				/* this is some scrolling crap, ignore it */
				break;
			} else if (_data[*pos] == 27) {
				/* escape char found, that shouldn't happen */
				Debug::rawCharArray(_data, start, *pos + 1);
				exit(7);
			} else if (*pos - start > 7) {
				/* too long escape sequence? */
				Debug::error() << WORLD_DEBUG_NAME << "Suspiciously long sequence: " << &_data[*pos] << endl;
				exit(8);
			}
		}
		if (*pos >= _data_size) {
			Debug::error() << WORLD_DEBUG_NAME << "Did not find stop char for sequence: " << _data << endl;
			exit(6);
		}
	} else if (_data[*pos] == '(') {
		/* designate character set, ignore */
		++*pos;
	} else if (_data[*pos] == ')') {
		/* designate character set, ignore */
		++*pos;
	} else if (_data[*pos] == '*') {
		/* designate character set, ignore */
		++*pos;
	} else if (_data[*pos] == '+') {
		/* designate character set, ignore */
		++*pos;
	} else if (_data[*pos] == 'M') {
		/* reverse linefeed? */
		if (_cursor.row() > 0)
			_cursor.moveNorth();
	} else if (_data[*pos] == '=') {
		/* application numpad?
		 * ignore */
	} else if (_data[*pos] == '>') {
		/* normal numpad?
		 * ignore */
	} else {
		Debug::error() << WORLD_DEBUG_NAME << "Unsupported escape sequence code at char " << *pos << ": " << &_data[*pos] << endl;
		exit(5);
	}
}

void World::update() {
	/* update the view */
	int charcolor = 0; // color of the char
	_data_size = _connection->retrieve(_data, BUFFER_SIZE);
	if (_data_size <= 0) {
		/* no data? sleep a sec and try again */
		sleep(1);
		_data_size = _connection->retrieve(_data, BUFFER_SIZE);
		if (_data_size <= 0) {
			Debug::error() << "No data received, quitting" << endl;
			exit(42);
		}
	}
	/* print world & data (to cerr, for debugging)
	 * this must be done here because if we get --More-- messages we'll update again */
	/* also, we do this in two loops because otherwise it flickers a lot */
	for (int a = 0; a < _data_size; ++a)
		cout << _data[a];
	cout.flush(); // same reason as in saiph.dumpMaps()
	Debug::rawCharArray(_data, 0, _data_size);
	for (int pos = 0; pos < _data_size; ++pos) {
		switch (_data[pos]) {
		case 0:
			/* sometimes we get lots of \0 characters.
			 * seemingly this happens when certain effects happen.
			 * for example, gas spore explotion, dagger thrown, etc.
			 * let's simply ignore these */
			break;
		case 8:
			/* backspace.
			 * make it go 1 char left */
			if (_cursor.col() > 0)
				_cursor.moveWest();
			break;

		case 10:
			/* line feed */
			_cursor.moveSouth();
			break;

		case 13:
			/* carriage return */
			_cursor.col(0);
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
			if (_cursor.col() >= COLS || _cursor.row() >= ROWS || _cursor.col() < 0 || _cursor.row() < 0) {
				Debug::warning() << WORLD_DEBUG_NAME << "Fell out of the dungeon: " << _cursor.row() << ", " << _cursor.col() << endl;
				break;
			}
			_view[_cursor.row()][_cursor.col()] = (unsigned char) _data[pos];
			_color[_cursor.row()][_cursor.col()] = charcolor;
			addChangedLocation(_cursor);
			_cursor.moveEast();
			break;
		}
	}

	fetchMessages();

	/* parse attribute & status rows */
	bool parsed_attributes = Saiph::parseAttributeRow(_view[ATTRIBUTES_ROW]);
	bool parsed_status = Saiph::parseStatusRow(_view[STATUS_ROW], _levelname, &_turn);
	/* check that the data we received seems ok */
	if (!_menu && !_question && (!parsed_attributes || !parsed_status || _cursor.row() < MAP_ROW_BEGIN || _cursor.row() > MAP_ROW_END || _cursor.col() < MAP_COL_BEGIN || _cursor.col() > MAP_COL_END)) {
		/* hmm, what else can it be?
		 * could we be missing data?
		 * this is bad, we'll lose messages, this should never happen */
		Debug::warning() << WORLD_DEBUG_NAME << "CURSOR ON UNEXPECTED LOCATION: " << _cursor.row() << ", " << _cursor.col() << endl;
		update();
		return;
	}
	++_frame_count;
	if (_messages == "  ")
		_messages.clear(); // no messages

	/* check if we get the question where we want to teleport */
	if (_messages.find(MESSAGE_FOR_INSTRUCTIONS, 0) != string::npos) {
		/* a bit unique case, this is a question.
		 * the data doesn't end with the sequence we check in World.
		 * however, the cursor is placed on the player when we get this message */
		_question = true;
	}

	/* check if we're engulfed */
	/* TODO: somehow this should be done in Saiph::analyze() */
	Saiph::engulfed(_cursor.insideMap() && _view[_cursor.row() - 1][_cursor.col() - 1] == '/' && _view[_cursor.row() - 1][_cursor.col() + 1] == '\\' && _view[_cursor.row() + 1][_cursor.col() - 1] == '\\' && _view[_cursor.row() + 1][_cursor.col() + 1] == '/');

	if (!_menu && !_question && !Saiph::engulfed())
		detectPosition();
}

/* main */
int main(int argc, const char* argv[]) {
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
	data::Monster::init();
	data::Item::init();
	Level::init();
	World::init(connection_type);
	Analyzer::init();

	/* run */
	World::run();
	Debug::notice() << "Quitting gracefully" << endl;

	/* destroy */
	Analyzer::destroy();
	World::destroy();
	Level::destroy();
	data::Monster::destroy();
	data::Item::destroy();
	Debug::destroy();
}
