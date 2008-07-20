#ifndef WORLD_H
/* defines */
#define WORLD_H
/* buffer */
#define BUFFER_SIZE 65536
/* text used to identify --More-- and menus */
#define MORE "--More--"
#define END "(end) " // yeah, space should be there
#define PAGE "(%d of %d)"

/* forward declare */
class World;

/* includes */
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
		bool menu;
		bool question;

		/* constructors */
		World(Connection *connection);

		/* methods */
		bool executeCommand(const string &command);

	private:
		/* variables */
		Connection *connection;
		bool changed[MAP_ROW_END + 1][MAP_COL_END + 1]; // just to prevent that same location is added twice in vector "changes"
		char data[BUFFER_SIZE];
		int data_size;
		string msg_str; // helps fetching messages
		unsigned char uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1]; // remapping ambigous symbols
		Point last_menu; // needed to help detect menus that persist over turns

		/* methods */
		void addChangedLocation(const Point &point);
		void fetchMenu();
		void fetchMenuText(int stoprow, int startcol, bool addspaces);
		void fetchMessages();
		void handleEscapeSequence(int *pos, int *color);
		void update();
};
#endif
