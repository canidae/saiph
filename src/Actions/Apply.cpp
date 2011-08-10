#include "Actions/Apply.h"

#include "Inventory.h"

using namespace action;

/* constructors/destructor */
// note, trapped chests, mirrors, crystal balls, cursed bells, can use more time; none are done
Apply::Apply(analyzer::Analyzer* analyzer, unsigned char key, const int& priority, bool update_inventory) : Action(analyzer), _do_apply("a", priority), _apply_item(key, PRIORITY_CONTINUE_ACTION), _update_inventory(update_inventory) {
}

Apply::~Apply() {
}

/* methods */
int Apply::id() {
	return ID;
}

const Command& Apply::command() {
	switch (_sequence) {
	case 0:
		return _do_apply;

	case 1:
		return _apply_item;

	default:
		return Action::NOOP;
	}
}

void Apply::update(const std::string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
		_sequence = 1;
	} else if (_sequence == 1) {
		/* mark inventory dirty if _update_inventory is true */
		if (_update_inventory)
			Inventory::update();
		_sequence = 2;
	} else {
		failed();
		_sequence = 2;
	}
}
