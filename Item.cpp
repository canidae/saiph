#include "Item.h"

/* constructors */
Item::Item(string name, int count) : name(name) {
	this->count = count;
	buc_status = BUC_UNKNOWN;
}
