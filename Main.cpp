#include "Main.h"

/* constructors */
Main::Main() {
	this->world = new World();
	this->saiph = new Saiph(this->world);
}

/* destructors */
Main::~Main() {
	delete this->saiph;
	delete this->world;
}

/* methods */
void Main::run() {
	for (int r = 0; r < ROWS; r++)
		cout << world->map[r] << endl;
}

/* main */
int main() {
	Main main;
	main.run();
	return 0;
}
