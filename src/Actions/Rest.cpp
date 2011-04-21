#include "Actions/Rest.h"

using namespace action;

/* constructors/destructor */
Rest::Rest(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _rest("16s", priority) {
}

Rest::~Rest() {
}

/* methods */
int Rest::id() {
	return ID;
}

const Command& Rest::command() {
	switch (_sequence) {
	case 0:
		return _rest;

	default:
		return Action::NOOP;
	}
}

void Rest::update(const std::string&) {
	if (_sequence == 0)
		_sequence = 1;
}
