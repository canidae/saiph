#ifndef WORLD_H
/* defines */
#define WORLD_H
/* debug */
#define WORLD_DEBUG_NAME "[World      ] "
#define DATA_DEBUG_NAME "[Data       ] " // data from game, give that an unique name
/* buffer */
#define BUFFER_SIZE 65536
/* stuff for parsing attribute & status rows */
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* text used to identify --More-- and menus */
#define MORE "--More--"
#define END "(end) " // yeah, space should be there
#define PAGE "(%d of %d)"
#define PAGE_DIRTY "%*[^(]" PAGE

/* forward declare */
class World;

/* includes */
#include <fstream>
#include <string>
#include <vector>
#include "Connection.h"
#include "Globals.h"
#include "Player.h"
#include "Point.h"

/* namespace */
using namespace std;

/* the world in our eyes */
class World {
	public:
		/* variables */
		vector<Point> changes; // list of locations changed since last "frame"
		char view[ROWS][COLS + 1]; // + 1 because we'll make the last character on each line '\0' (for easier parsing)
		int color[ROWS][COLS]; // not used for string reading, no need for + 1
		Point cursor; // cursor position
		Player player;
		string messages;
		int cur_page;
		int max_page;
		bool menu;
		bool question;

		/* constructors */
		World(Connection *connection, ofstream *debugfile);

		/* methods */
		bool executeCommand(const string &command);

	private:
		/* variables */
		Connection *connection;
		ofstream *debugfile;
		bool changed[MAP_ROW_END + 1][MAP_COL_END + 1]; // just to prevent that same location is added twice in vector "changes"
		char data[BUFFER_SIZE + 1]; // "+ 1" because we'll add an extra '\0' at the end for string parsing
		int data_size;
		string msg_str; // helps fetching messages
		unsigned char uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1]; // remapping ambigous symbols
		Point last_menu; // needed to help detect menus that persist over turns
		bool updated_status_row; // statusrow will be updated every frame, we can use this to determine if we got all the data
		Point last_cursor; // must place the cursor where it was if we didn't get all the data
		string last_messages; // if we didn't get all the data, then messages may be screwed

		/* methods */
		void addChangedLocation(const Point &point);
		void fetchMenu();
		void fetchMenuText(int stoprow, int startcol, bool addspaces);
		bool fetchMessages();
		void handleEscapeSequence(int *pos, int *color);
		void update(int buffer_pos = 0);
};
#endif
