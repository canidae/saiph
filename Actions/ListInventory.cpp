#include "ListInventory.h"

using namespace action;
using namespace std;

const int ListInventory::id = Action::createID();

ListInventory::ListInventory() : list_inventory("i", PRIORITY_LOOK) {
}

const Command &ListInventory::getCommand() {
	switch (sequence) {
	case 0:
		return list_inventory;

	default:
		return Action::noop;
	}
}

void ListInventory::updateAction(const string &) {
	++sequence;
}
