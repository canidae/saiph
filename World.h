#ifndef WORLD_H
#define WORLD_H

#include <fcntl.h>
#include <iostream>
#include <pty.h>
#include "Player.h"

/* stuff you can mess with, but shouldn't */
#define NETHACK "/usr/games/nethack"
#define USLEEP 2000000
/* stuff you _really_ shouldn't mess with */
#define BUFFER 4096
#define COLS 80
#define END "(end)"
#define MORE "--More--"
#define ROWS 24
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
		char messages[BUFFER]; // messages received on first row
		int messages_pos; // used when we get "--More--" and "(end)" messages
		Player player;

		/* constructors */
		World();

		/* destructors */
		~World();

		/* methods */
		void command(const char *command);

	private:
		/* variables */
		char data[BUFFER];
		int data_size;
		int input[2];
		int output[2];

		/* methods */
		void handleEscapeSequence(int &pos);
		void fetchMessages();
		void parsePlayerAttributesAndStatus();
		void update();
};
#endif
