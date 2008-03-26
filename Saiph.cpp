#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	this->connection = new Connection(remote);
	this->world = new World(this->connection);
}

/* destructors */
Saiph::~Saiph() {
	delete world;
	delete connection;
}

/* methods */
void Saiph::run() {
	for (int r = 0; r < ROWS; r++)
		cout << world->map[r] << endl;
}

/* main */
int main() {
	Saiph saiph(false);
	saiph.run();
}
