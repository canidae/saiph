#include "Actions/PutOn.h"

#include "Inventory.h"
#include "World.h"

using namespace action;

/* constructors/destructor */
PutOn::PutOn(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _put_on("P", priority), _put_on_key(key, PRIORITY_CONTINUE_ACTION) {
}

PutOn::~PutOn() {
}

/* methods */
int PutOn::id() {
	return ID;
}

const Command& PutOn::command() {
	switch (_sequence) {
	case 0:
		return _put_on;

	case 1:
		return _put_on_key;

	default:
		return Action::NOOP;
	}
}

void PutOn::update(const std::string& messages) {
	if (World::question() && messages.find(MESSAGE_WHAT_TO_PUT_ON) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1) {
		/* also mark the inventory dirty when we do this */
		Inventory::update();
		_sequence = 2;
	}
}
