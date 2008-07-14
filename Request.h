#ifndef REQUEST_H
/* defines */
#define REQUEST_H
/* requests */
#define DESCEND_DUNGEON 1
#define ASCEND_DUNGEON 2

/* forward declare */
class Request;

/* includes */
#include "Coordinate.h"

/* namespace */
using namespace std;

/* Request */
class Request : public Coordinate {
	public:
		/* variables */
		int request;
		int priority;

		/* constructors */
		Request(int request, int priority, const Coordinate &coordinate = Coordinate());
		Request(int request, int priority, const Point &point = Point());
};
#endif
