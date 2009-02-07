#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
	public:
		int row;
		int col;

		Point(int row = -1, int col = -1);

		bool operator<(const Point &p) const;
		bool operator>(const Point &p) const;
		bool operator<=(const Point &p) const;
		bool operator>=(const Point &p) const;
		bool operator==(const Point &p) const;
		bool operator!=(const Point &p) const;
};

std::ostream &operator<<(std::ostream &os, const Point& p);
#endif
