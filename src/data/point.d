module data.point;

struct Point {
	public:
		int row;
		int col;

		auto moveNorthwest() {
			--row;
			--col;
			return this;
		}

		auto moveNorth() {
			--row;
			return this;
		}

		auto moveNortheast() {
			--row;
			++col;
			return this;
		}

		auto moveEast() {
			++col;
			return this;
		}

		auto moveSoutheast() {
			++row;
			++col;
			return this;
		}

		auto moveSouth() {
			++row;
			return this;
		}

		auto moveSouthwest() {
			++row;
			--col;
			return this;
		}

		auto moveWest() {
			--col;
			return this;
		}

		bool opEquals()(Point p) const {
			return row == p.row && col == p.col;
		}

		auto opCmp()(Point p) const {
			if (row < p.row || (row == p.row && col < p.col))
				return -1;
			if (row > p.row || (row == p.row && col > p.col))
				return 1;
			return 0;
		}

		unittest {
			assert(Point(1, 1) == Point(1, 1));
			assert(Point(1, 1) != Point(2, 1));
			assert(Point(1, 1) != Point(1, 2));
			assert(Point(1, 1) < Point(2, 1));
			assert(Point(1, 1) < Point(1, 2));
			assert(Point(1, 1) <= Point(2, 1));
			assert(Point(1, 1) <= Point(1, 2));
			assert(Point(2, 1) >= Point(1, 1));
			assert(Point(1, 2) >= Point(1, 1));
			assert(Point(2, 1) > Point(1, 1));
			assert(Point(1, 2) > Point(1, 1));
		}
}
