#include "Coordinate.h"

/* constructors */
Coordinate::Coordinate(int branch, int level, int row, int col) : Point(row, col), branch(branch), level(level) {
}

Coordinate::Coordinate(int branch, int level, const Point &point) : Point(point), branch(branch), level(level) {
}

Coordinate::Coordinate(const Point &point) : Point(point), branch(0), level(0) {
}
