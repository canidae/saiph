module data.point;

struct Point {
	public:
		int row;
		int col;

		Point moveNorthwest() {
			--row;
			--col;
			return this;
		}

		Point moveNorth() {
			--row;
			return this;
		}

		Point moveNortheast() {
			--row;
			++col;
			return this;
		}

		Point moveEast() {
			++col;
			return this;
		}

		Point moveSoutheast() {
			++row;
			++col;
			return this;
		}

		Point moveSouth() {
			++row;
			return this;
		}

		Point moveSouthwest() {
			++row;
			--col;
			return this;
		}

		Point moveWest() {
			--col;
			return this;
		}

		bool opEquals()(Point p) const {
			return row == p.row && col == p.col;
		}

		int opCmp()(Point p) const {
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
