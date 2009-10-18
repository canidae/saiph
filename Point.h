#ifndef POINT_H
#define POINT_H

#include <iostream>

#include "Globals.h"

class Point {
public:

	Point(int row = -1, int col = -1) : _row(row), _col(col) {
	}

	int col() const {
		return _col;
	}

	int col(int col) {
		_col = col;
		return this->col();
	}

	int row() const {
		return _row;
	}

	int row(int row) {
		_row = row;
		return this->row();
	}

	bool insideMap() {
		return row() >= MAP_ROW_BEGIN && row() <= MAP_ROW_END && col() >= MAP_COL_BEGIN && col() <= MAP_COL_END;
	}

	Point& moveNorth() {
		--_row;
		return *this;
	}

	Point& moveNortheast() {
		--_row;
		++_col;
		return *this;
	}

	Point& moveEast() {
		++_col;
		return *this;
	}

	Point& moveSoutheast() {
		++_row;
		++_col;
		return *this;
	}

	Point& moveSouth() {
		++_row;
		return *this;
	}

	Point& moveSouthwest() {
		++_row;
		--_col;
		return *this;
	}

	Point& moveWest() {
		--_col;
		return *this;
	}

	Point& moveNorthwest() {
		--_row;
		--_col;
		return *this;
	}

	bool operator<(const Point& p) const {
		return (_row > p._row || (_row == p._row && _col > p._col));
	}

	bool operator>(const Point& p) const {
		return (_row > p._row || (_row == p._row && _col > p._col));
	}

	bool operator<=(const Point& p) const {
		return (_row < p._row || (_row == p._row && _col <= p._col));
	}

	bool operator>=(const Point& p) const {
		return (_row > p._row || (_row == p._row && _col >= p._col));
	}

	bool operator==(const Point& p) const {
		return (_row == p._row && _col == p._col);
	}

	bool operator!=(const Point& p) const {
		return (_row != p._row || _col != p._col);
	}

private:
	int _row;
	int _col;
};

std::ostream& operator<<(std::ostream& os, const Point& p);
#endif
