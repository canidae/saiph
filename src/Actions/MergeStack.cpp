#include "Actions/MergeStack.h"

#include "Inventory.h"
#include "World.h"

using namespace action;

#define MESSAGE_ITEM_TO_ADJUST "  What do you want to adjust?"
#define MESSAGE_ADJUST_TO "  Adjust letter to what "

/* constructors/destructor */
MergeStack::MergeStack(analyzer::Analyzer* analyzer, unsigned char item) : Action(analyzer, TIME_NONE), _do_adjust("#adjust\n", PRIORITY_NAME_ITEM), _name_item(item, PRIORITY_CONTINUE_ACTION) {
}

MergeStack::~MergeStack() {
}

/* methods */
int MergeStack::id() {
	return ID;
}

const Command& MergeStack::command() {
	switch (_sequence) {
	case 0:
		return _do_adjust;

	case 1:
	case 2:
		return _name_item;

	default:
		return Action::NOOP;
	}
}

void MergeStack::update(const std::string& messages) {
	if (messages.find(MESSAGE_ITEM_TO_ADJUST) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_ADJUST_TO) != std::string::npos) {
		_sequence = 2;
	} else if (_sequence == 2) {
		/* we need to mark inventory as not updated */
		Inventory::update();
		_sequence = 3;
	}
}
