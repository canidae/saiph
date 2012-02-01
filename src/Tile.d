import Level;
import Point;

class Tile {
public:
	this(Level* level, ref const Point point) {
		_level = level;
		_point = point;
	}

	@property auto level() const {
		return _level;
	}

	@property auto point() const {
		return _point;
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

private:
	Level* _level;
	Point _point;
	char _symbol;
	bool _shop;
}
