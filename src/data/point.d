module data.point;

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

	auto moveNorthwest() {
		--_row;
		--_col;
		return this;
	}

	auto moveNorth() {
		--_row;
		return this;
	}

	auto moveNortheast() {
		--_row;
		++_col;
		return this;
	}

	auto moveEast() {
		++_col;
		return this;
	}

	auto moveSoutheast() {
		++_row;
		++_col;
		return this;
	}

	auto moveSouth() {
		++_row;
		return this;
	}

	auto moveSouthwest() {
		++_row;
		--_col;
		return this;
	}

	auto moveWest() {
		--_col;
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

private:
	int _row;
	int _col;
}
