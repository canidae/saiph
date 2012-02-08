module data.tile;

import data.coordinate;
import data.point;

class Tile {
public:
	Coordinate coordinate;
	char symbol;
	bool shop;

	this(Coordinate coordinate) {
		this.coordinate = coordinate;
	}

	alias coordinate this;
}
