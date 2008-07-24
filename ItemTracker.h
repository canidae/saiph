#ifndef ITEMTRACKER_H
/* defines */
#define ITEMTRACKER_H
/* debug */
#define ITEMTRACKER_DEBUG_NAME "[ItemTracker] "
/* parsing help */
#define GET_SINGLE_ITEM "%7s %127[^\t\n]"

/* forward declare */
class ItemTracker;

/* includes */
#include <list>
#include <map>
#include <vector>
#include "Globals.h"
#include "Item.h"
#include "Point.h"
#include "Saiph.h"
#include "Stash.h"

/* namespaces */
using namespace std;

/* ItemTracker */
class ItemTracker {
	public:
		/* variables */
		bool item[UCHAR_MAX + 1];
		map<unsigned char, Item> inventory;
		map<unsigned char, Item> pickup;
		map<int, map<int, map<Point, Stash> > > stashes;
		Stash *on_ground;

		/* constructors */
		ItemTracker(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);
		void removeItemFromInventory(unsigned char key, const Item &item);
		void updateStash(const Point &point);

	private:
		/* variables */
		Saiph *saiph;

		/* methods */
		void addItemToInventory(unsigned char key, const Item &item);
		void addItemToPickup(unsigned char key, const Item &item);
		void addItemToStash(const Point &point, const Item &item);
		void clearStash(const Point &point);
		void removeItemFromPickup(const Item &item);
		void removeItemFromStash(const Point &point, const Item &item);
};
#endif
