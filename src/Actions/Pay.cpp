#include "Actions/Pay.h"

using namespace action;

/* constructors/destructor */
Pay::Pay(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _pay("p", priority), _answer_no(NO, PRIORITY_CONTINUE_ACTION), _answer_yes(YES, PRIORITY_CONTINUE_ACTION) {
}

Pay::~Pay() {
}

/* methods */
int Pay::id() {
	return ID;
}

const Command& Pay::command() {
	switch (_sequence) {
	case 0:
		return _pay;

	case 1:
		return _answer_no;

	case 2:
		return _answer_yes;

	default:
		return Action::NOOP;
	}
}

void Pay::update(const std::string& messages) {
	if (messages.find(MESSAGE_ITEMIZED_BILLING) != std::string::npos) {
		/* asking if we want to use itemized billing */
		_sequence = 1;
	} else if (messages.find(MESSAGE_PAY) != std::string::npos) {
		/* we do want to pay for this item */
		_sequence = 2;
	} else {
		/* we finished paying */
		if (_sequence != 2)
			failed();
		_sequence = 3;
		Inventory::update();
	}
}
