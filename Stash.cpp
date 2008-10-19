#include "Stash.h"

/* constructors/destructor */
Stash::Stash(int turn_changed, unsigned char top_symbol, int top_color) : turn_changed(turn_changed), top_symbol(top_symbol), top_color(top_color) {
	items.clear();
}
