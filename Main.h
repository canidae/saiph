#ifndef MAIN_H
#define MAIN_H

#include "Saiph.h"
#include "World.h"

using namespace std;

class Main {
	public:
		/* variables */
		Saiph *saiph;
		World *world;

		/* constructors */
		Main();

		/* destructors */
		~Main();

		/* methods */
		void run();
};
#endif
