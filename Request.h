#ifndef REQUEST_H
/* defines */
#define REQUEST_H
/* requests */
#define ILLEGAL_REQUEST 0
#define REQUEST_APPLY_UNIHORN 1
#define REQUEST_ELBERETH_OR_REST 2
#define REQUEST_UPDATED_INVENTORY 3
#define REQUEST_DIRTY_INVENTORY 4
#define REQUEST_DIRTY_STASH 5
#define REQUEST_PRAY 7
#define REQUEST_ARMOR_WEAR 8
#define REQUEST_ITEM_GROUP_SET_AMOUNT 9
#define REQUEST_ITEM_GROUP_ADD 10
#define REQUEST_ITEM_PICKUP 11
#define REQUEST_WEAPON_WIELD 12
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
		unsigned char key;
		string data;
		Coordinate coordinate;

		/* constructors */
		Request();
};
#endif
