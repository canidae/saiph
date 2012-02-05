import coordinate;
import point;

class Tile {
public:
	this(Coordinate coordinate) {
		_coordinate = coordinate;
	}

	@property auto coordinate() const {
		return _coordinate;
	}

	@property auto symbol() const {
		return _symbol;
	}

	@property auto symbol(char symbol) {
		return _symbol = symbol;
	}

	@property auto shop(bool shop) {
		return _shop = shop;
	}

	alias coordinate this;

private:
	Coordinate _coordinate;
	char _symbol;
	bool _shop;
}
