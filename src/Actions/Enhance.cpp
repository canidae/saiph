#include "Actions/Enhance.h"

using namespace action;

/* constructors/destructor */
Enhance::Enhance(analyzer::Analyzer* analyzer) : Action(analyzer, false), _do_enhance("#enhance\n", PRIORITY_CONTINUE_ACTION), _enhance_a("a", PRIORITY_CONTINUE_ACTION) {
}

Enhance::~Enhance() {
}

/* methods */
int Enhance::id() {
	return ID;
}

const Command& Enhance::command() {
	switch (_sequence) {
	case 0:
		return _do_enhance;

	case 1:
		return _enhance_a;

	default:
		return Action::NOOP;
	}
}

void Enhance::update(const std::string&) {
	if (_sequence == 0)
		_sequence = 1;
	else if (_sequence == 1)
		_sequence = 2;
}
