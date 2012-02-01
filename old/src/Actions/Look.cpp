#include "Actions/Look.h"

using namespace action;

/* constructors/destructor */
Look::Look(analyzer::Analyzer* analyzer) : Action(analyzer, TIME_NONE), _look(":", PRIORITY_LOOK) {
}

Look::~Look() {
}

/* methods */
int Look::id() {
	return ID;
}

const Command& Look::command() {
	switch (_sequence) {
	case 0:
		return _look;

	default:
		return Action::NOOP;
	}
}

void Look::update(const std::string&) {
	if (_sequence == 0)
		_sequence = 1;
}
