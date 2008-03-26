#ifndef WORLD_H
#define WORLD_H

#include "Connection.h"
#include "Globals.h"
#include "Player.h"

/* where we can find various stuff */
#define MESSAGE_ROW 0
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* text telling us there are more messages */
#define END "(end)"
#define MORE "--More--"
/* we don't have enough characters to make unique symbols for every tile,
 * these values are supposed to help on that */
#define OPEN_DOOR 128
#define TREE 129
#define IRON_BARS 130
#define THRONE 131
#define SINK 132
#define LAVA 133
#define ICE 134
#define LOWERED_DRAWBRIDGE 135

using namespace std;

class World {
	public:
		/* variables */
		int row; // cursor position, row
		int col; // cursor position, col
		char map[ROWS][COLS + 1]; // a black/white map of the dungeon
		char messages[BUFFER_SIZE]; // messages received on first row
		int messages_pos; // used when we get "--More--" and "(end)" messages
		Connection *connection;
		Player player;

		/* constructors */
		World(Connection *connection);

		/* destructors */
		~World();

		/* methods */
		void command(const char *command);

	private:
		/* variables */
		char data[BUFFER_SIZE];
		ssize_t data_size;

		/* methods */
		void handleEscapeSequence(int &pos, int &colour);
		void fetchMessages();
		void update();
};
#endif
