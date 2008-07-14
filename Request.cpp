#include "Request.h"

/* constructors */
Request::Request(int request, int priority, const Coordinate &coordinate) : Coordinate(coordinate) {
	this->request = request;
	this->priority = priority;
}

Request::Request(int request, int priority, const Point &point) : Coordinate(point) {
	this->request = request;
	this->priority = priority;
}
