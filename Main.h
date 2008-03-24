#ifndef MAIN_H
#define MAIN_H

#include "Player.h"
#include "World.h"

using namespace std;

class Main {
	public:
		/* variables */
		Player player;
		World world;

		/* constructors */
		Main();

		/* destructors */
		~Main();

		/* methods */
		void run();
};
#endif
