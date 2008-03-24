#include "Main.h"

/* constructors */
Main::Main() {
}

/* destructors */
Main::~Main() {
}

/* methods */
void Main::run() {
	Map map = world.getMap();
	cout << "Size: " << map.size << endl;
	sleep(3);
	cout << map.data << endl;
	sleep(4);
}

/* main */
int main() {
	Main main;
	main.run();
	return 0;
}
