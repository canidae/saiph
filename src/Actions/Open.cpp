#include "Actions/Open.h"

using namespace action;

/* constructors/destructor */
Open::Open(analyzer::Analyzer* analyzer, unsigned char direction, int priority) : Action(analyzer), _open("o", priority), _open_direction(direction, PRIORITY_CONTINUE_ACTION) {
}

Open::~Open() {
}

/* methods */
int Open::id() {
	return ID;
}

const Command& Open::command() {
	switch (_sequence) {
	case 0:
		return _open;

	case 1:
		return _open_direction;

	default:
		return Action::NOOP;
	}
}

void Open::update(const std::string& messages) {
	if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1) {
		_sequence = 2;
	} else {
		failed();
		_sequence = 2;
	}
}
