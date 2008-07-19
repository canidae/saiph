#ifndef ITEM_H
/* defines */
#define ITEM_H

/* forward declare */
class Item;

/* includes */
#include <string>
#include "Globals.h"

/* namespace */
using namespace std;

/* Item */
class Item {
	public:
		/* variables */
		string name;
		int count;
		int buc_status;

		/* constructors */
		Item(const string &name = "", int count = 1);
		Item(const Item &item);
};
#endif
