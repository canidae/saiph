#ifndef WORLD_H
#define WORLD_H
/* debug */
#define WORLD_DEBUG_NAME "World] "
/* buffer */
#define BUFFER_SIZE 65536
#define MAX_EFFECTS 8
#define MAX_TEXT_LENGTH 16
#define MAX_LEVELNAME_LENGTH 16
/* stuff for parsing attribute & status rows */
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* text used to identify --More-- and menus */
#define MORE "--More--"
#define END "(end) " // yeah, space should be there
#define PAGE "(%d of %d)"
#define PAGE_DIRTY "%*[^(]" PAGE

#include <list>
#include <map>
#include <string>
#include <vector>
#include "Command.h"
#include "Globals.h"
#include "Level.h"
#include "Point.h"
#include "Saiph.h"

namespace action {
	class Action;
}
class Connection;

class World {
public:
	static std::vector<Point> changes; // list of locations changed since last "frame"
	static char view[ROWS][COLS + 1]; // + 1 because we'll make the last character on each line '\0' (for easier parsing)
	static int color[ROWS][COLS]; // not used for string reading, no need for + 1
	static Point cursor; // cursor position
	static int cur_page;
	static int max_page;
	static int command_count;
	static int frame_count;
	static bool menu;
	static bool question;
	static bool engulfed;
	static char levelname[MAX_LEVELNAME_LENGTH];
	static int turn;
	static int real_turn;
	static std::vector<Level> levels;
	static Coordinate branch[BRANCHES];

	static void init(int connection_type);
	static void destroy();
	static void registerAnalyzer(analyzer::Analyzer* analyzer);
	static void unregisterAnalyzer(analyzer::Analyzer* analyzer);
	static int getPriority();
	static int getLastActionID();
	static bool setAction(action::Action* action);
	static bool queueAction(action::Action* action);
	static unsigned char directLine(Point point, bool ignore_sinks, bool ignore_boulders);
	static unsigned char getDungeonSymbol();
	static unsigned char getDungeonSymbol(const Coordinate& coordinate);
	static unsigned char getDungeonSymbol(const Point& point);
	static unsigned char getDungeonSymbol(unsigned char direction);
	static unsigned char getMonsterSymbol(const Coordinate& coordinate);
	static unsigned char getMonsterSymbol(const Point& point);
	static void setDirtyStash();
	static void setDungeonSymbol(const Coordinate& coordinate, unsigned char symbol);
	static void setDungeonSymbol(const Point& point, unsigned char symbol);
	static void setDungeonSymbol(unsigned char symbol);
	static const PathNode& shortestPath(const Point& target);
	static PathNode shortestPath(unsigned char symbol);
	static PathNode shortestPath(const Coordinate& target);
	static void run();

private:
	static Connection* connection;
	static action::Action* action;
	static std::list<action::Action*> action_queue;
	static bool changed[MAP_ROW_END + 1][MAP_COL_END + 1]; // just to prevent that same location is added twice in vector "changes"
	static std::string messages;
	static bool inverse;
	static bool bold;
	static char data[BUFFER_SIZE * 2]; // weird errors from valgrind, oh well, we got enough memory
	static char effects[MAX_EFFECTS][MAX_TEXT_LENGTH];
	static int data_size;
	static std::string msg_str; // helps fetching messages
	static Point last_menu; // needed to help detect menus that persist over turns
	static std::map<std::string, std::vector<int> > levelmap; // used for faster map recognition
	static time_t start_time;
	static std::vector<analyzer::Analyzer*> analyzers;
	static int last_action_id;

	static void addChangedLocation(const Point& point);
	static void detectPosition();
	static Point directionToPoint(unsigned char direction);
	static bool directLineHelper(const Point& point, bool ignore_sinks, bool ignore_boulders);
	static void dumpMaps();
	static bool executeCommand(const std::string& command);
	static void fetchMenu();
	static void fetchMenuText(int stoprow, int startcol, bool addspaces);
	static void fetchMessages();
	static void handleEscapeSequence(int* pos, int* color);
	static bool parseAttributeRow(const char* attributerow);
	static bool parseStatusRow(const char* statusrow);
	static void update();
};

/* inline methods */
inline int World::getLastActionID() {
	/* return the id of the last action */
	return last_action_id;
}

inline unsigned char World::getDungeonSymbol() {
	/* return dungeon symbol at player position */
	return World::levels[Saiph::position.level()].getDungeonSymbol(Saiph::position);
}

inline unsigned char World::getDungeonSymbol(const Coordinate& coordinate) {
	/* return dungeon symbol at given coordinate */
	if (coordinate.level() < 0 || coordinate.level() > (int) World::levels.size())
		return OUTSIDE_MAP;
	return World::levels[coordinate.level()].getDungeonSymbol(coordinate);
}

inline unsigned char World::getDungeonSymbol(const Point& point) {
	/* return dungeon symbol at given point on current level */
	return World::levels[Saiph::position.level()].getDungeonSymbol(point);
}

inline unsigned char World::getDungeonSymbol(unsigned char direction) {
	/* return dungeon symbol in given direction on current level */
	switch (direction) {
	case NW:
		return getDungeonSymbol(Point(Saiph::position.row() - 1, Saiph::position.col() - 1));

	case N:
		return getDungeonSymbol(Point(Saiph::position.row() - 1, Saiph::position.col()));

	case NE:
		return getDungeonSymbol(Point(Saiph::position.row() - 1, Saiph::position.col() + 1));

	case W:
		return getDungeonSymbol(Point(Saiph::position.row(), Saiph::position.col() - 1));

	case NOWHERE:
	case DOWN:
	case UP:
		return getDungeonSymbol();

	case E:
		return getDungeonSymbol(Point(Saiph::position.row(), Saiph::position.col() + 1));

	case SW:
		return getDungeonSymbol(Point(Saiph::position.row() + 1, Saiph::position.col() - 1));

	case S:
		return getDungeonSymbol(Point(Saiph::position.row() + 1, Saiph::position.col()));

	case SE:
		return getDungeonSymbol(Point(Saiph::position.row() + 1, Saiph::position.col() + 1));

	default:
		return OUTSIDE_MAP;
	}
}

inline unsigned char World::getMonsterSymbol(const Coordinate& coordinate) {
	/* return monster symbol at given point on current level */
	if (coordinate.level() < 0 || coordinate.level() > (int) World::levels.size())
		return ILLEGAL_MONSTER;
	return World::levels[coordinate.level()].getMonsterSymbol(coordinate);
}

inline unsigned char World::getMonsterSymbol(const Point& point) {
	/* return monster symbol at given point on current level */
	return World::levels[Saiph::position.level()].getMonsterSymbol(point);
}

inline void World::setDirtyStash() {
	/* set stash at player position dirty */
	std::map<Point, Stash>::iterator s = World::levels[Saiph::position.level()].stashes.find(Saiph::position);
	if (s != World::levels[Saiph::position.level()].stashes.end())
		s->second.items.clear();
}

inline void World::setDungeonSymbol(unsigned char symbol) {
	/* set dungeon symbol at player position */
	World::levels[Saiph::position.level()].setDungeonSymbol(Saiph::position, symbol);
}

inline void World::setDungeonSymbol(const Coordinate& coordinate, unsigned char symbol) {
	/* set dungeon symbol at given coordinate */
	if (coordinate.level() < 0 || coordinate.level() > (int) World::levels.size())
		return;
	World::levels[coordinate.level()].setDungeonSymbol(coordinate, symbol);
}

inline void World::setDungeonSymbol(const Point& point, unsigned char symbol) {
	/* set dungeon symbol at given point on current level */
	World::levels[Saiph::position.level()].setDungeonSymbol(point, symbol);
}

inline const PathNode& World::shortestPath(const Point& point) {
	/* returns PathNode for shortest path from player to target */
	return World::levels[Saiph::position.level()].shortestPath(point);
}
#endif
