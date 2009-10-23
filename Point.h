#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
	Point(const int& row = -1, const int& col = -1);

	const int& col() const;
	const int& col(const int& col);
	const int& row() const;
	const int& row(const int& row);
	bool insideMap() const;
	Point& moveNorthwest();
	Point& moveNorth();
	Point& moveNortheast();
	Point& moveEast();
	Point& moveSoutheast();
	Point& moveSouth();
	Point& moveSouthwest();
	Point& moveWest();

	bool operator<(const Point& p) const;
	bool operator>(const Point& p) const;
	bool operator<=(const Point& p) const;
	bool operator>=(const Point& p) const;
	bool operator==(const Point& p) const;
	bool operator!=(const Point& p) const;

private:
	int _row;
	int _col;
};

std::ostream & operator<<(std::ostream& os, const Point& p);
#endif
