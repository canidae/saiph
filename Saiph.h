#ifndef SAIPH_H
#define SAIPH_H

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
		World *world;

		/* constructors */
		Saiph(World *world);

		/* destructors */
		~Saiph();

		/* methods */
		void action();

	private:
};

#endif
