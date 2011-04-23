#include "Actions/SwapWeapon.h"
#include "Inventory.h"

using namespace action;

/* constructors/destructor */
SwapWeapon::SwapWeapon(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _swap_weap("x", priority) {
}

SwapWeapon::~SwapWeapon() {
}

/* methods */
int SwapWeapon::id() {
	return ID;
}

const Command& SwapWeapon::command() {
	switch (_sequence) {
	case 0:
		return _swap_weap;

	default:
		return Action::NOOP;
	}
}

void SwapWeapon::update(const std::string&) {
	_sequence = 1;
	Inventory::update();
}
