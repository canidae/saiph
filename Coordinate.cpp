#include "Coordinate.h"

using namespace std;

ostream &operator<<(ostream &os, const Coordinate& c) {
	return os << "(" << c.level << ", " << c.row << ", " << c.col << ")";
}
