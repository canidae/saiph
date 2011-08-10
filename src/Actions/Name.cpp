#include "Actions/Name.h"

#include "Inventory.h"
#include "World.h"

using namespace action;

/* constructors/destructor */
Name::Name(analyzer::Analyzer* analyzer, unsigned char item, const std::string& name) : Action(analyzer, 0), _do_name("#name\n", PRIORITY_NAME_ITEM), _name_individual(YES, PRIORITY_CONTINUE_ACTION), _name_item(item, PRIORITY_CONTINUE_ACTION), _set_name(name + "\n", PRIORITY_CONTINUE_ACTION) {
}

Name::~Name() {
}

/* methods */
int Name::id() {
	return ID;
}

const Command& Name::command() {
	switch (_sequence) {
	case 0:
		return _do_name;

	case 1:
		return _name_individual;

	case 2:
		return _name_item;

	case 3:
		Inventory::update();
		return _set_name;

	default:
		return Action::NOOP;
	}
}

void Name::update(const std::string& messages) {
	if (messages.find(MESSAGE_NAME_INDIVIDUAL_OBECT) != std::string::npos || (World::menu() && messages.find(MESSAGE_WHAT_TO_NAME_TYPE) != std::string::npos)) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_ITEM_TO_NAME) != std::string::npos) {
		_sequence = 2;
	} else if (messages.find(MESSAGE_WHAT_TO_NAME_ITEM) != std::string::npos) {
		_sequence = 3;
	} else if (_sequence == 3) {
		/* we need to mark inventory as not updated */
		Inventory::update();
		_sequence = 4;
	}
}
