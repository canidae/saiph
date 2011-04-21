#include "Actions/Kick.h"

using namespace action;

/* constructors/destructor */
Kick::Kick(analyzer::Analyzer* analyzer, unsigned char direction, int priority) : Action(analyzer), _kick("", priority), _kick_direction(direction, PRIORITY_CONTINUE_ACTION) {
}

Kick::~Kick() {
}

/* methods */
int Kick::id() {
	return ID;
}

const Command& Kick::command() {
	switch (_sequence) {
	case 0:
		return _kick;

	case 1:
		return _kick_direction;

	default:
		return Action::NOOP;
	}
}

void Kick::update(const std::string& messages) {
	if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1) {
		_sequence = 2;
	} else {
		failed();
		_sequence = 2;
	}
}
