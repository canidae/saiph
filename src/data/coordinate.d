module data.coordinate;

import data.level;
import data.point;

struct Coordinate {
public:
	Level level;
	Point point;

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
}
