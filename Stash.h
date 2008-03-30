#ifndef STASH_H
/* defines */
#define STASH_H
#define MAX_ITEMS 128

/* forward declare */
class Stash;

/* includes */

/* namespace */
using namespace std;

/* a stash is a pile of 1 or more items
 * TODO? */
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
