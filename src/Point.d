struct Point {
public:
	@property auto row() const {
		return _row;
	}

	@property auto row(int row) {
		return _row = row;
	}

	@property auto col() const {
		return _col;
	}

	@property auto col(int col) {
		return _col = col;
	}

	bool opEquals(ref const Point p) const {
		return row == p.row && col == p.col;
	}

	auto opCmp(ref const Point p) const {
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

private:
	int _row;
	int _col;
}
