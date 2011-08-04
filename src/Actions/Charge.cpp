#include "Actions/Charge.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Globals.h"

using namespace action;

/* constructors/destructor */
Charge::Charge(analyzer::Analyzer* analyzer, unsigned char scroll, unsigned char item, int priority) : Action(analyzer), _read("r", priority), _charge_scroll(scroll, PRIORITY_CONTINUE_ACTION), _charge_item(item, PRIORITY_CONTINUE_ACTION) {
}

Charge::~Charge() {
}

/* methods */
int Charge::id() {
	return ID;
}

bool Charge::canRead() {
	return !(Saiph::blind() || Saiph::hallucinating() || Saiph::confused() || Saiph::stunned());
}

const Command& Charge::command() {
	switch (_sequence) {
	case 0:
		return _read;

	case 1:
		return _charge_scroll;
		
	case 2:
		return Action::NOOP;
		
	case 3:
		return _charge_item;
		
	default:
		return Action::NOOP;
	}
}

void Charge::update(const std::string& messages) {

	if (messages.find(MESSAGE_READ_WHAT) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_CHARGING_READ) != std::string::npos) {
		_sequence = 2;
	} else if (messages.find(MESSAGE_CHARGE_WHAT) != std::string::npos) {
		_sequence = 3;
	} else if (messages.find(MESSAGE_CHARGED_SOMETHING) != std::string::npos) {
		_sequence = 4;
	}

}
