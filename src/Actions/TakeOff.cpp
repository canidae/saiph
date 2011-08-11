#include "Actions/TakeOff.h"

#include "Inventory.h"
#include "World.h"

using namespace action;

/* constructors/destructor */
// technically it's STANDARD if oc_delay is 0, or NOMUL if positive.  But our spoilers don't have oc_delay...
TakeOff::TakeOff(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer, TIME_MULTIPLE), _take_off("T", priority), _take_off_key(key, PRIORITY_CONTINUE_ACTION) {
}

TakeOff::~TakeOff() {
}

/* methods */
int TakeOff::id() {
	return ID;
}

const Command& TakeOff::command() {
	switch (_sequence) {
	case 0:
		return _take_off;

	case 1:
		return _take_off_key;

	default:
		return Action::NOOP;
	}
}

void TakeOff::update(const std::string& messages) {
	if (World::question() && messages.find(MESSAGE_WHAT_TO_TAKE_OFF) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1 || messages.find(MESSAGE_YOU_FINISH_TAKING_OFF) != std::string::npos || messages.find(TAKE_OFF_ARMOR_CURSED) != std::string::npos || messages.find(MESSAGE_YOU_WERE_WEARING) != std::string::npos) {
		/* also mark the inventory dirty when we do this */
		Inventory::update();
		_sequence = 2;
	}
}
