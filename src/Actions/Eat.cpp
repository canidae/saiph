#include "Actions/Eat.h"

using namespace action;

/* constructors/destructor */
Eat::Eat(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _eat("e", priority), _item(key, PRIORITY_CONTINUE_ACTION), _answer_no(NO, PRIORITY_CONTINUE_ACTION), _answer_yes(YES, PRIORITY_CONTINUE_ACTION) {
}

Eat::~Eat() {
}

/* methods */
int Eat::id() {
	return ID;
}

const Command& Eat::command() {
	switch (_sequence) {
	case 0:
		return _eat;

	case 1:
		return _item;

	case 2:
		return _answer_no;

	case 3:
		return _answer_yes;

	default:
		return Action::NOOP;
	}
}

void Eat::update(const std::string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_EAT) != std::string::npos) {
		/* tell the game which item to eat */
		_sequence = 1;
	} else if (messages.find(MESSAGE_EAT_IT_2) != std::string::npos || messages.find(MESSAGE_EAT_ONE_2) != std::string::npos) {
		/* we're not eating corpses/items on ground, answer no */
		_sequence = 2;
	} else if (messages.find(MESSAGE_STOP_EATING) != std::string::npos) {
		/* we're about to choke, abort eating */
		_sequence = 3;
	} else if (_sequence < 4) {
		/* ate item, inventory is no longer updated */
		Inventory::update();
		_sequence = 4;
	}
}
