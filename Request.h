#ifndef REQUEST_H
/* defines */
#define REQUEST_H
/* requests */
#define ILLEGAL_REQUEST 0
#define REQUEST_DESCEND_DUNGEON 1
#define REQUEST_ASCEND_DUNGEON 2
#define REQUEST_ELBERETH_OR_REST 3

/* forward declare */
class Request;

/* includes */
#include <string>
#include "Globals.h"

/* namespace */
using namespace std;

/* Request */
class Request {
	public:
		/* variables */
		int request;
		int priority;
		int value1;
		string data;

		/* constructors */
		Request();
};
#endif
