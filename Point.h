#ifndef POINT_H
#define POINT_H

class Point {
	public:
		int row;
		int col;

		Point(int row = -1, int col = -1);

		bool operator<(const Point &point) const;
		bool operator>(const Point &point) const;
		bool operator<=(const Point &point) const;
		bool operator>=(const Point &point) const;
		bool operator==(const Point &point) const;
		bool operator!=(const Point &point) const;
};
#endif
