#include "Actions/Unlock.h"

#include "Globals.h"

using namespace action;

/* constructors/destructor */
Unlock::Unlock(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority) : Action(analyzer, TIME_MULTIPLE), _do_apply("a", priority), _apply_item(key, PRIORITY_CONTINUE_ACTION), _apply_direction(direction, PRIORITY_CONTINUE_ACTION), _do_unlock("y", PRIORITY_CONTINUE_ACTION) {
}

Unlock::~Unlock() {
}

/* methods */
int Unlock::id() {
	return ID;
}

const Command& Unlock::command() {
	switch (_sequence) {
	case 0:
		return _do_apply;

	case 1:
		return _apply_item;

	case 2:
		return _apply_direction;

	case 3:
		return _do_unlock;

	default:
		return Action::NOOP;
	}
}

void Unlock::update(const std::string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
		_sequence = 2;
	} else if (messages.find(MESSAGE_UNLOCK_IT) != std::string::npos) {
		_sequence = 3;
	} else if (_sequence == 3) {
		_sequence = 4;
	} else {
		failed();
		_sequence = 4;
	}
}
