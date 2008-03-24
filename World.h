/* stuff you can mess with, but shouldn't */
#define NETHACK "/usr/games/nethack"
#define USLEEP 2000000
/* stuff you _really_ shouldn't mess with */
#define BUFFER 4096
#define COLS 80
#define HISTORY 16
#define ROWS 24

#include <fcntl.h>
#include <iostream>
#include <pty.h>

using namespace std;

struct Map {
	char *data;
	int size;
	char map[ROWS][COLS + 1];
	char *messages[BUFFER];
};

class World {
	public:
		/* variables */

		/* constructors */
		World();

		/* destructors */
		~World();

		/* methods */
		void command(char *command);
		Map getMap();

	private:
		/* variables */
		Map *map;
		int map_current;

		int output[2];
		int input[2];

		/* methods */
		void update();
		void handleEscapeSequence(int &pos, int &row, int &col);
};
