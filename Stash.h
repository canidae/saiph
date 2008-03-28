#ifndef STASH_H
#define STASH_H

/* forward declare */
class Stash;

#define MAX_ITEMS 128

using namespace std;

/* a Stash is a pile of 1 or more items */
class Stash {
	public:
		/* variables */
		int row;
		int col;
		int dungeon;
		Item items[MAX_ITEMS];
		int item_count;

		/* constructors */
		Stash();

		/* destructors */
		~Stash();

		/* methods */

	private:
};

#endif
