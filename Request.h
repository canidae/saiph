#ifndef REQUEST_H
/* defines */
#define REQUEST_H
/* requests */
#define ILLEGAL_REQUEST 0
#define REQUEST_DESCEND_DUNGEON 1
#define REQUEST_ASCEND_DUNGEON 2
#define REQUEST_ELBERETH_OR_REST 3
#define REQUEST_LOOT_STASH 4
#define REQUEST_DIRTY_INVENTORY 5
#define REQUEST_UPDATED_INVENTORY 6
#define REQUEST_PRAY 7
#define REQUEST_VISIT_NEAREST_FOUNTAIN 8

/* forward declare */
class Request;

/* includes */
#include <string>
#include "Coordinate.h"
#include "Globals.h"

/* namespace */
using namespace std;

/* Request */
class Request {
	public:
		/* variables */
		int request;
		int priority;
		int value;
		string data;
		Coordinate coordinate;

		/* constructors */
		Request();
};
#endif
