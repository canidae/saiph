#include "Actions/Dip.h"

#include "Inventory.h"

using namespace action;

/* constructors/destructor */
Dip::Dip(analyzer::Analyzer* analyzer, unsigned char key, unsigned char dip_into, const int& priority, bool update_inventory) : Action(analyzer), _do_dip("#dip\n", priority), _dip_item(key, PRIORITY_CONTINUE_ACTION), _dip_in_water(dip_into == ILLEGAL_ITEM ? 'y' : 'n', PRIORITY_CONTINUE_ACTION), _dip_in_item(dip_into, PRIORITY_CONTINUE_ACTION), _update_inventory(update_inventory) {
}

Dip::~Dip() {
}

/* methods */
int Dip::id() {
	return ID;
}

const Command& Dip::command() {
	switch (_sequence) {
	case 0:
		return _do_dip;

	case 1:
		return _dip_item;

	case 2:
		return _dip_in_water;

	case 3:
		return _dip_in_item;

	default:
		return Action::NOOP;
	}
}

void Dip::update(const std::string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_DIP) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_DIP_IN_FOUNTAIN) != std::string::npos || messages.find(MESSAGE_DIP_IN_WATER) != std::string::npos) {
		_sequence = 2;
	} else if (messages.find(MESSAGE_WHAT_TO_DIP_INTO) != std::string::npos) {
		_sequence = 3;
	} else if (_sequence == 2 || _sequence == 3) {
		/* mark inventory dirty if _update_inventory is true */
		if (_update_inventory)
			Inventory::update();
		_sequence = 4;
	} else {
		failed();
		_sequence = 4;
	}
}
