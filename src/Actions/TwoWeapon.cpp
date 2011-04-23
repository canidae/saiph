#include "Actions/TwoWeapon.h"
#include "Inventory.h"

using namespace action;

/* constructors/destructor */
TwoWeapon::TwoWeapon(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _two_weap("#twoweapon\n", priority) {
}

TwoWeapon::~TwoWeapon() {
}

/* methods */
int TwoWeapon::id() {
	return ID;
}

const Command& TwoWeapon::command() {
	switch (_sequence) {
	case 0:
		return _two_weap;

	default:
		return Action::NOOP;
	}
}

void TwoWeapon::update(const std::string&) {
	_sequence = 1;
	Inventory::update();
}
