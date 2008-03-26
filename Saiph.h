#ifndef SAIPH_H
#define SAIPH_H

#include "Connection.h"
#include "Globals.h"
#include "World.h"

/* movement */
#define MOVE_NW "y"
#define MOVE_N "k"
#define MOVE_NE "u"
#define MOVE_W "h"
#define MOVE_E "l"
#define MOVE_SW "b"
#define MOVE_S "j"
#define MOVE_SE "n"
#define MOVE_UP "<"
#define MOVE_DOWN ">"

using namespace std;

/* this is our AI */
class Saiph {
	public:
		/* variables */

		/* constructors */
		Saiph(bool remote);

		/* destructors */
		~Saiph();

		/* methods */
		int main();
		void run();

	private:
		/* variables */
		Connection *connection;
		World *world;
};

#endif
