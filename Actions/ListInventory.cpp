#include "ListInventory.h"

using namespace std;
using namespace action;

ListInventory::ListInventory() : list_inventory("i", PRIORITY_LOOK) {
}

ListInventory::~ListInventory() {
}

const Command &ListInventory::getCommand() {
	switch (sequence) {
	case 0:
		return list_inventory;

	default:
		return Action::noop;
	}
}

void ListInventory::updateAction() {
	++sequence;
}
