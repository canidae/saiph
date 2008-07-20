#ifndef POINT_H
/* defines */
#define POINT_H

/* Point */
class Point {
	public:
		/* variables */
		int row;
		int col;

		/* constructors */
		Point(int row = 0, int col = 0);

		/* operator overloading */
		bool operator<(const Point &point) const;
		bool operator>(const Point &point) const;
		bool operator<=(const Point &point) const;
		bool operator>=(const Point &point) const;
		bool operator==(const Point &point) const;
		bool operator!=(const Point &point) const;
};
#endif
