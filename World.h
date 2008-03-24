#ifndef WORLD_H
#define WORLD_H

/* stuff you can mess with, but shouldn't */
#define NETHACK "/usr/games/nethack"
#define USLEEP 2000000
/* stuff you _really_ shouldn't mess with */
#define BUFFER 4096
#define COLS 80
#define ROWS 24

#include <fcntl.h>
#include <iostream>
#include <pty.h>

using namespace std;

struct Map {
	int row;
	int col;
	char data[BUFFER];
	int data_size;
	char map[ROWS][COLS + 1];
};

class World {
	public:
		/* variables */
		Map map;

		/* constructors */
		World();

		/* destructors */
		~World();

		/* methods */
		void command(char *command);

	private:
		/* variables */
		int input[2];
		int output[2];

		/* methods */
		void handleEscapeSequence(int &pos, int &row, int &col);
		void update();
};
#endif
