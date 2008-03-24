#include "Main.h"

/* constructors */
Main::Main() {
}

/* destructors */
Main::~Main() {
}

/* methods */
void Main::run() {
	//cout << "Size: " << map.data_size << endl;
	//cout << map.data << endl;
	for (int r = 0; r < ROWS; r++)
		cout << world.map[r] << endl;
	player.parseAttributesAndStatus(world);
	//cout << world.data << endl;
}

/* main */
int main() {
	Main main;
	main.run();
	return 0;
}
