#include "Actions/Call.h"

#include "Inventory.h"
#include "World.h"

using namespace action;

/* constructors/destructor */
Call::Call(analyzer::Analyzer* analyzer, unsigned char item, const std::string& call) : Action(analyzer, 0), _do_call("#name\n", PRIORITY_NAME_ITEM), _call_individual("n", PRIORITY_CONTINUE_ACTION), _call_item(item, PRIORITY_CONTINUE_ACTION), _set_call(call + "\n", PRIORITY_CONTINUE_ACTION) {
}

Call::~Call() {
}

/* methods */
int Call::id() {
	return ID;
}

const Command& Call::command() {
	switch (_sequence) {
	case 0:
		return _do_call;

	case 1:
		return _call_individual;

	case 2:
		return _call_item;

	case 3:
		Inventory::update();
		return _set_call;

	default:
		return Action::NOOP;
	}
}

void Call::update(const std::string& messages) {
	if (messages.find(MESSAGE_NAME_INDIVIDUAL_OBECT) != std::string::npos || (World::menu() && messages.find(MESSAGE_WHAT_TO_NAME_TYPE) != std::string::npos)) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_ITEM_TO_CALL) != std::string::npos) {
		_sequence = 2;
	} else if (messages.find(MESSAGE_WHAT_TO_CALL_ITEM) != std::string::npos) {
		_sequence = 3;
	} else if (_sequence == 3) {
		/* we need to mark inventory as not updated */
		Inventory::update();
		_sequence = 4;
	}
}
