#include "Actions/Throw.h"

#include "Inventory.h"

using namespace action;

/* constructors/destructor */
Throw::Throw(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority) : Action(analyzer), _do_throw("t", priority), _throw_item(key, PRIORITY_CONTINUE_ACTION), _throw_direction(direction, PRIORITY_CONTINUE_ACTION) {
}

Throw::~Throw() {
}

/* methods */
int Throw::id() {
	return ID;
}

const Command& Throw::command() {
	switch (_sequence) {
	case 0:
		return _do_throw;

	case 1:
		return _throw_item;

	case 2:
		return _throw_direction;

	default:
		return Action::NOOP;
	}
}

void Throw::update(const std::string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_THROW) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
		_sequence = 2;
	} else if (_sequence == 2) {
		/* mark inventory dirty */
		Inventory::update();
		_sequence = 3;
	}
}
