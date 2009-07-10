#include "Stash.h"

/* constructors/destructor */
Stash::Stash(unsigned char top_symbol, int top_color) : top_symbol(top_symbol), top_color(top_color) {
	items.clear();
}
