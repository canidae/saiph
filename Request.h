#ifndef REQUEST_H
/* defines */
#define REQUEST_H
/* requests */
#define ILLEGAL_REQUEST 0
#define REQUEST_ELBERETH_OR_REST 2
#define REQUEST_VISIT_STASH 3
#define REQUEST_LOOT_STASH 4
#define REQUEST_DIRTY_INVENTORY 5
#define REQUEST_PRAY 7
#define REQUEST_VISIT_NEAREST_FOUNTAIN 8
#define REQUEST_WEAPON_GROUP_CREATE 9
#define REQUEST_WEAPON_GROUP_ADD 10
#define REQUEST_WEAPON_WIELD 11
#define REQUEST_WEAPON_PICKUP 12
#define REQUEST_ADD_THROWN_WEAPON 13

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
		int status;
		string data;
		Coordinate coordinate;

		/* constructors */
		Request();
};
#endif
