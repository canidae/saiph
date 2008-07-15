#ifndef WORLD_H
/* defines */
#define WORLD_H
/* buffer */
#define BUFFER_SIZE 65536
/* text telling us there are more messages */
#define MORE "--More--"
#define MORE_LENGTH 8
/* we need these to determine if we're looking at a menu */
#define END "(end)"
#define END_LENGTH 6 // seems to be a bug here, there's an space after; "(end) "
#define PAGE " of "
#define PAGE_LENGTH 6 // must add 2 because of "y)" in "(x of y)"
/* and these for looking for questions */
#define QUESTION_YN "[yn]"
#define QUESTION_YNQ "[ynq]"
#define QUESTION_DY "(y)"
#define QUESTION_DN "(n)"
#define QUESTION_DQ "(q)"
#define QUESTION_LENGTH 20

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
		ssize_t data_size;
		string msg_str; // helps fetching messages
		unsigned char uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1]; // remapping ambigous symbols

		/* methods */
		void addChangedLocation(const Point &point);
		void fetchMessages();
		void handleEscapeSequence(int *pos, int *color);
		void update();
};
#endif
