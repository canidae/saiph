#include "Actions/DropGold.h"

#include "Globals.h"

using namespace action;

/* constructors/destructor */
DropGold::DropGold(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _drop("d", priority), _gold("$", PRIORITY_CONTINUE_ACTION) {
}

DropGold::~DropGold() {
}

/* methods */
int DropGold::id() {
	return ID;
}

const Command& DropGold::command() {
	switch (_sequence) {
	case 0:
		return _drop;

	case 1:
		return _gold;

	default:
		return Action::NOOP;
	}
}

void DropGold::update(const std::string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_DROP) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1) {
		_sequence = 2;
	} else {
		failed();
		_sequence = 2;
	}
}
