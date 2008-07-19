#include "Item.h"

/* constructors */
Item::Item(const string &name, int count) : name(name), count(count), buc_status(BUC_UNKNOWN) {
}

Item::Item(const Item &item) : name(item.name), count(item.count), buc_status(item.buc_status) {
}
