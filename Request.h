#ifndef REQUEST_H
/* defines */
#define REQUEST_H
/* requests */
#define REQUEST_DESCEND_DUNGEON 1
#define REQUEST_ASCEND_DUNGEON 2

/* forward declare */
class Request;

/* includes */
#include <string>
#include "Coordinate.h"

/* namespace */
using namespace std;

/* Request */
class Request : public Coordinate {
	public:
		/* variables */
		int request;
		int priority;
		string data;

		/* constructors */
		Request(int request, int priority = 0, const string &data = "", const Coordinate &coordinate = Coordinate());
		Request(int request, int priority = 0, const string &data = "", const Point &point = Point());
		Request(int request, int priority = 0, const Coordinate &coordinate = Coordinate());
		Request(int request, int priority = 0, const Point &point = Point());
};
#endif
