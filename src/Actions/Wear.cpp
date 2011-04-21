#include "Actions/Wear.h"

using namespace action;

/* constructors/destructor */
Wear::Wear(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _wear("W", priority), _wear_key(key, PRIORITY_CONTINUE_ACTION) {
}

Wear::~Wear() {
}

/* methods */
int Wear::id() {
	return ID;
}

const Command& Wear::command() {
	switch (_sequence) {
	case 0:
		return _wear;

	case 1:
		return _wear_key;

	default:
		return Action::NOOP;
	}
}

void Wear::update(const std::string& messages) {
	if (World::question() && messages.find(MESSAGE_WHAT_TO_WEAR) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1) {
		/* also mark the inventory dirty when we do this */
		Inventory::update();
		_sequence = 2;
	}
}
