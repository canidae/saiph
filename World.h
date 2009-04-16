#ifndef WORLD_H
#define WORLD_H
/* debug */
#define WORLD_DEBUG_NAME "World] "
#define DATA_DEBUG_NAME "Data] " // data from game, give that an unique name
/* buffer */
#define BUFFER_SIZE 65536
#define MAX_EFFECTS 8
#define MAX_TEXT_LENGTH 16
/* stuff for parsing attribute & status rows */
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* text used to identify --More-- and menus */
#define MORE "--More--"
#define END "(end) " // yeah, space should be there
#define PAGE "(%d of %d)"
#define PAGE_DIRTY "%*[^(]" PAGE

#include <fstream>
#include <string>
#include <vector>
#include "Globals.h"
#include "Point.h"

class Connection;

class World {
public:
	static std::vector<Point> changes; // list of locations changed since last "frame"
	static char view[ROWS][COLS + 1]; // + 1 because we'll make the last character on each line '\0' (for easier parsing)
	static int color[ROWS][COLS]; // not used for string reading, no need for + 1
	static Point cursor; // cursor position
	static std::string messages;
	static int cur_page;
	static int max_page;
	static int command_count;
	static int frame_count;
	static bool menu;
	static bool question;
	static char level[MAX_TEXT_LENGTH];
	static int turn;

	static void destroy();
	static bool executeCommand(const std::string &command);
	static void init(int connection_type);

private:
	static Connection *connection;
	static bool changed[MAP_ROW_END + 1][MAP_COL_END + 1]; // just to prevent that same location is added twice in vector "changes"
	static bool inverse;
	static bool bold;
	static char data[BUFFER_SIZE * 2]; // weird errors from valgrind, oh well, we got enough memory
	static char effects[MAX_EFFECTS][MAX_TEXT_LENGTH];
	static int data_size;
	static std::string msg_str; // helps fetching messages
	static Point last_menu; // needed to help detect menus that persist over turns

	static void addChangedLocation(const Point &point);
	static void fetchMenu();
	static void fetchMenuText(int stoprow, int startcol, bool addspaces);
	static void fetchMessages();
	static void handleEscapeSequence(int *pos, int *color);
	static bool parseAttributeRow(const char *attributerow);
	static bool parseStatusRow(const char *statusrow);
	static void update();
};
#endif
