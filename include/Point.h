#ifndef POINT_H
#define POINT_H

#include <iostream>
#include "Globals.h"

class Point {
public:
	Point(int row = -1, int col = -1) : _row(row), _col(col) { }

	int col() const  { return _col; }
	int col(int col) { return (_col = col); }
	int row() const  { return _row; }
	int row(int row) { return (_row = row); }
	bool insideMap() const;
	Point& moveDirection(unsigned char direction);
	Point& moveNorthwest() { --_row; --_col; return *this; }
	Point& moveNorth()     { --_row; return *this; }
	Point& moveNortheast() { --_row; ++_col; return *this; }
	Point& moveEast()      { ++_col; return *this; }
	Point& moveSoutheast() { ++_row; ++_col; return *this; }
	Point& moveSouth()     { ++_row; return *this; }
	Point& moveSouthwest() { ++_row; --_col; return *this; }
	Point& moveWest()      { --_col; return *this; }

	bool operator<(const Point& p) const;
	bool operator>(const Point& p) const;
	bool operator<=(const Point& p) const;
	bool operator>=(const Point& p) const;
	bool operator==(const Point& p) const { return _row == p._row && _col == p._col; }
	bool operator!=(const Point& p) const { return _row != p._row || _col != p._col; }

	static int gridDistance(const Point& p1, const Point& p2);

private:
	int _row;
	int _col;
};

std::ostream & operator<<(std::ostream& os, const Point& p);
#endif
