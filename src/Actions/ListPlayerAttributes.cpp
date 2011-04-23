#include "Actions/ListPlayerAttributes.h"

#include "World.h"

using namespace action;

/* constructors/destructor */
ListPlayerAttributes::ListPlayerAttributes(analyzer::Analyzer* analyzer) : Action(analyzer, false), _list_attributes("", PRIORITY_LIST_PLAYER_ATTRIBUTES), _close_page(CLOSE_PAGE, PRIORITY_CONTINUE_ACTION) {
}

ListPlayerAttributes::~ListPlayerAttributes() {
}

/* methods */
int ListPlayerAttributes::id() {
	return ID;
}

const Command& ListPlayerAttributes::command() {
	switch (_sequence) {
	case 0:
		return _list_attributes;

	case 1:
		return _close_page;

	default:
		return Action::NOOP;
	}
}

void ListPlayerAttributes::update(const std::string&) {
	if (World::menu())
		_sequence = 1;
	else if (_sequence == 1)
		_sequence = 2;
}
