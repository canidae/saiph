#include "Actions/Answer.h"

using namespace action;

/* constructors/destructor */
Answer::Answer(analyzer::Analyzer* analyzer, const std::string& answer) : Action(analyzer, 0), _answer(answer, PRIORITY_CONTINUE_ACTION) {
}

Answer::Answer(analyzer::Analyzer* analyzer, unsigned char answer) : Action(analyzer, 0), _answer(answer, PRIORITY_CONTINUE_ACTION) {
}

Answer::~Answer() {
}

/* methods */
int Answer::id() {
	return ID;
}

const Command& Answer::command() {
	switch (_sequence) {
	case 0:
		return _answer;

	default:
		return Action::NOOP;
	}
}

void Answer::update(const std::string&) {
	if (_sequence == 0)
		_sequence = 1;
}

