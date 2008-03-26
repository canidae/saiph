#ifndef WORLD_H
#define WORLD_H

#include "Connection.h"
#include "Globals.h"
#include "Player.h"

#define END "(end)"
#define MORE "--More--"
/* characters we should do farlook on.
 * this is monsters, objects, traps and certain dungeon features: \{}
 * only necessary to do this on non-unique characters.
 * certain characters are not interesting ('.', '-' and '|', as examples).
 * see nethackrc for characters used for various stuff */
#define INSPECT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@6'&;:~])[=\"(%!?+/$*0`^\\{}"

using namespace std;

class World {
	public:
		/* variables */
		int row; // cursor position, row
		int col; // cursor position, col
		char map[ROWS][COLS + 1];
		char messages[MESSAGE_BUFFER]; // messages received on first row
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
		char data[MESSAGE_BUFFER];
		ssize_t data_size;

		/* methods */
		void handleEscapeSequence(int &pos);
		void fetchMessages();
		void parsePlayerAttributesAndStatus();
		void update();
};
#endif
