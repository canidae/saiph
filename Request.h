#ifndef REQUEST_H
/* defines */
#define REQUEST_H
/* requests */
#define REQUEST_DESCEND_DUNGEON 1
#define REQUEST_ASCEND_DUNGEON 2
#define REQUEST_ELBERETH_OR_REST 3

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
		Request();
};
#endif
