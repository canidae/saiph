#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
	class adjacent_iterator;

	Point(int row = -1, int col = -1);

	int col() const;
	int col(int col);
	int row() const;
	int row(int row);
	bool insideMap() const;
	Point& moveDirection(unsigned char direction);
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

	static int gridDistance(const Point& p1, const Point& p2);

private:
	int _row;
	int _col;
};

class Point::adjacent_iterator {
public:
	adjacent_iterator(const Point& from, bool include_self = false);
	adjacent_iterator(const Point& from, const char* priority);

	operator bool() const;
	adjacent_iterator& operator++();
	const Point& operator*() const;
private:
	const char* _agenda;
	Point _from;
	Point _cur;
	void find();
};

std::ostream& operator<<(std::ostream& os, const Point& p);
#endif
