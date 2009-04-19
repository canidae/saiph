#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
	int row;
	int col;

	Point(int row = -1, int col = -1) : row(row), col(col) {}

	bool operator<(const Point &p) const {return (row > p.row || (row == p.row && col > p.col));}
	bool operator>(const Point &p) const {return (row > p.row || (row == p.row && col > p.col));}
	bool operator<=(const Point &p) const {return (row < p.row || (row == p.row && col <= p.col));}
	bool operator>=(const Point &p) const {return (row > p.row || (row == p.row && col >= p.col));}
	bool operator==(const Point &p) const {return (row == p.row && col == p.col);}
	bool operator!=(const Point &p) const {return (row != p.row || col != p.col);}
};

std::ostream &operator<<(std::ostream &os, const Point &p);
#endif
