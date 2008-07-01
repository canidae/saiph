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
#include "Connection.h"
#include "Globals.h"
#include "Player.h"

/* namespace */
using namespace std;

/* the world in our eyes */
class World {
	public:
		/* variables */
		char map[ROWS][COLS + 1];
		unsigned char color[ROWS][COLS];
		char messages[BUFFER_SIZE];
		bool menu;
		bool question;
		Player player;
		int row; // cursor position, row
		int col; // cursor position, col
		char data[BUFFER_SIZE];
		ssize_t data_size;
		string command; // yes, i know. i'm hopeless. i save lots of allocations when all analyzers share this string

		/* constructors */
		World(Connection *connection);

		/* destructors */
		~World();

		/* methods */
		bool executeCommand();

	private:
		/* variables */
		Connection *connection;
		int messages_pos; // used for concatenation multiple messages together
		string msg_str; // used for fetching messages

		/* methods */
		void handleEscapeSequence(int *pos, int *color);
		void fetchMessages();
		void fetchMessagesHelper(int row, int startcol);
		void update();
};
#endif
