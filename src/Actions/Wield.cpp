#include "Actions/Wield.h"

#include "Inventory.h"
#include "World.h"

using namespace action;

/* constructors/destructor */
Wield::Wield(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _wield("w", priority), _wield_key(key, PRIORITY_CONTINUE_ACTION) {
}

Wield::~Wield() {
}

/* methods */
int Wield::id() {
	return ID;
}

const Command& Wield::command() {
	switch (_sequence) {
	case 0:
		return _wield;

	case 1:
		return _wield_key;

	default:
		return Action::NOOP;
	}
}

void Wield::update(const std::string& messages) {
	if (World::question() && messages.find(MESSAGE_WHAT_TO_WIELD) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1 || messages.find(WIELD_WEAPON_CURSED) != std::string::npos) {
		/* also mark the inventory dirty when we do this */
		Inventory::update();
		_sequence = 2;
	}
}
