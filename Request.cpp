#include "Request.h"

/* constructors */
Request::Request(int request, int priority, const Coordinate &coordinate) : Coordinate(coordinate), request(request), priority(priority) {
}

Request::Request(int request, int priority, const Point &point) : Coordinate(point), request(request), priority(priority) {
}
