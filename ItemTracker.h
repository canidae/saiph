#ifndef ITEMTRACKER_H
/* defines */
#define ITEMTRACKER_H

/* forward declare */
class ItemTracker;

/* includes */
#include <list>
#include <map>
#include "Coordinate.h"
#include "Globals.h"
#include "Item.h"
#include "Saiph.h"
#include "Stash.h"

/* namespaces */
using namespace std;

/* ItemTracker */
class ItemTracker {
	public:
		/* variables */
		map<unsigned char, Item> inventory;
		list<Stash> stashes;

		/* constructors */
		ItemTracker(Saiph *saiph);

		/* methods */
		void addItemToInventory(const Item &item);
		void addItemToStash(const Item &item);
		void moveItemFromStashToInventory(const Item &item);
		void moveItemFromInventoryToStash(const Item &item);
		void parseMessages(string *messages);
		void removeItemFromInventory(const Item &item);
		void removeItemFromStash(const Item &item);

	private:
		/* variables */
		Saiph *saiph;

		/* methods */
		void addItemToStash(const Coordinate &coordinate, const Item &item);
		void removeItemFromStash(const Coordinate &coordinate, const Item &item);
};
#endif
