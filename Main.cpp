#include "Main.h"

/* constructors */
Main::Main() {
}

/* destructors */
Main::~Main() {
}

/* methods */
void Main::run() {
	for (int r = 0; r < ROWS; r++)
		cout << world.map[r] << endl;
	player.parseAttributesAndStatus(world);
}

/* main */
int main() {
	Main main;
	main.run();
	return 0;
}
