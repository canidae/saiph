#ifndef MAP_H
/* defines */
#define MAP_H

/* includes */

/* namespace */
using namespace std;

/* Map */
class Map {
	public:
		/* variables */
		unsigned char dungeon[ROWS][COLS];
		unsigned char search[ROWS][COLS];

		/* constructors */
		Map();

		/* destructors */
		~Map();

		/* methods */

	private:
		/* variables */

		/* methods */
};
#endif
