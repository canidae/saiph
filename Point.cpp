#include "Point.h"

using namespace std;

ostream& operator<<(ostream& os, const Point& p) {
	return os << "(" << p.row() << ", " << p.col() << ")";
}
