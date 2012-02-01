#include "Actions/Donate.h"

#include <sstream>
#include "Globals.h"

using namespace action;

/* constructors/destructor */
Donate::Donate(analyzer::Analyzer* analyzer, unsigned char direction, int amount, int priority) : Action(analyzer), _do_donate("#chat\n", priority), _donate_direction(direction, PRIORITY_CONTINUE_ACTION) {
	std::ostringstream a;
	a << amount << std::endl;
	_donate_amount = Command(a.str(), PRIORITY_CONTINUE_ACTION);
}

Donate::~Donate() {
}

/* methods */
int Donate::id() {
	return ID;
}

const Command& Donate::command() {
	switch (_sequence) {
	case 0:
		return _do_donate;

	case 1:
		return _donate_direction;

	case 2:
		return _donate_amount;

	default:
		return Action::NOOP;
	}
}

void Donate::update(const std::string& messages) {
	if (messages.find(MESSAGE_TALK_TO_WHOM) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_HOW_MUCH_OFFER) != std::string::npos) {
		_sequence = 2;
	} else if (_sequence == 2) {
		_sequence = 3;
	} else {
		failed();
		_sequence = 3;
	}
}
