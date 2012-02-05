module data.coordinate;

import data.level;
import data.point;

struct Coordinate {
public:
	@property auto level() const {
		return _level;
	}

	@property auto level(Level level) {
		return _level = level;
	}

	@property auto point() const {
		return _point;
	}

	@property auto point(Point point) {
		return _point = point;
	}

	bool opEquals()(Coordinate c) const {
		return level == c.level && point == c.point;
	}

	auto opCmp()(Coordinate c) const {
		if (level < c.level || (level == c.level && point < c.point))
			return -1;
		if (level > c.level || (level == c.level && point > c.point))
			return 1;
		return 0;
	}

	alias point this;

	unittest {
		/*
		assert(Coordinate(1, Point(1, 1)) == Coordinate(1, Point(1, 1)));
		assert(Coordinate(1, Point(1, 1)) != Coordinate(2, Point(1, 1)));
		assert(Coordinate(1, Point(1, 1)) != Coordinate(1, Point(2, 1)));
		assert(Coordinate(1, Point(1, 1)) != Coordinate(1, Point(1, 2)));
		assert(Coordinate(1, Point(1, 1)) < Coordinate(2, Point(1, 1)));
		assert(Coordinate(1, Point(1, 1)) < Coordinate(1, Point(2, 1)));
		assert(Coordinate(1, Point(1, 1)) < Coordinate(1, Point(1, 2)));
		assert(Coordinate(1, Point(1, 1)) <= Coordinate(2, Point(1, 1)));
		assert(Coordinate(1, Point(1, 1)) <= Coordinate(1, Point(2, 1)));
		assert(Coordinate(1, Point(1, 1)) <= Coordinate(1, Point(1, 2)));
		assert(Coordinate(2, Point(1, 1)) >= Coordinate(1, Point(1, 1)));
		assert(Coordinate(1, Point(2, 1)) >= Coordinate(1, Point(1, 1)));
		assert(Coordinate(1, Point(1, 2)) >= Coordinate(1, Point(1, 1)));
		assert(Coordinate(2, Point(1, 1)) > Coordinate(1, Point(1, 1)));
		assert(Coordinate(1, Point(2, 1)) > Coordinate(1, Point(1, 1)));
		assert(Coordinate(1, Point(1, 2)) > Coordinate(1, Point(1, 1)));
		*/
	}

private:
	Level _level;
	Point _point;
}
