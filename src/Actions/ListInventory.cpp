#include "Actions/ListInventory.h"

#include "World.h"

using namespace action;

/* constructors/destructor */
ListInventory::ListInventory(analyzer::Analyzer* analyzer) : Action(analyzer, false), _list_inventory("i", PRIORITY_LIST_INVENTORY), _close_page(CLOSE_PAGE, PRIORITY_CONTINUE_ACTION) {
}

ListInventory::~ListInventory() {
}

/* methods */
int ListInventory::id() {
	return ID;
}

const Command& ListInventory::command() {
	switch (_sequence) {
	case 0:
		return _list_inventory;

	case 1:
		return _close_page;

	default:
		return Action::NOOP;
	}
}

void ListInventory::update(const std::string&) {
	if (_sequence == 0)
		_sequence = 1;
	else if (_sequence == 1 && !World::menu())
		_sequence = 2;
}
