#ifndef ITEMTRACKER_H
/* defines */
#define ITEMTRACKER_H
/* parsing help */
#define GET_SINGLE_ITEM "%7s %127[^\t\n]"

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
		map<unsigned char, Item> pickup;
		list<Stash> stashes;

		/* constructors */
		ItemTracker(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;

		/* methods */
		void addItemToInventory(unsigned char key, const Item &item);
		void addItemToPickup(unsigned char key, const Item &item);
		void addItemToStash(const Coordinate &coordinate, const Item &item);
		void clearStash(const Coordinate &coordinate);
		Item parseItemText(const string &text);
		void removeItemFromInventory(unsigned char key, const Item &item);
		void removeItemFromPickup(unsigned char key, const Item &item);
		void removeItemFromStash(const Coordinate &coordinate, const Item &item);
};
#endif
