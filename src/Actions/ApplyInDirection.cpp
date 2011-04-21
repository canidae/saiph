#include "Actions/ApplyInDirection.h"

#include "Inventory.h"

using namespace action;

/* constructors/destructor */
ApplyInDirection::ApplyInDirection(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority, bool update_inventory) : Action(analyzer), _do_apply("a", priority), _apply_item(key, PRIORITY_CONTINUE_ACTION), _apply_direction(direction, PRIORITY_CONTINUE_ACTION), _update_inventory(update_inventory) {
}

ApplyInDirection::~ApplyInDirection() {
}

/* methods */
int ApplyInDirection::id() {
	return ID;
}

const Command& ApplyInDirection::command() {
	switch (_sequence) {
	case 0:
		return _do_apply;

	case 1:
		return _apply_item;

	case 2:
		return _apply_direction;

	default:
		return Action::NOOP;
	}
}

void ApplyInDirection::update(const std::string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_DIG_DIRECTION) != std::string::npos) {
		_sequence = 2;
	} else {
		/* mark inventory dirty if update_inventory is true */
		if (_update_inventory)
			Inventory::update();
		if (_sequence != 2)
			failed();
		_sequence = 3;
	}
}
