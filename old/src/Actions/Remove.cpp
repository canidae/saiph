#include "Actions/Remove.h"

#include "Inventory.h"
#include "World.h"

using namespace action;

/* constructors/destructor */
Remove::Remove(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _remove("R", priority), _remove_key(key, PRIORITY_CONTINUE_ACTION) {
}

Remove::~Remove() {
}

/* methods */
int Remove::id() {
	return ID;
}

const Command& Remove::command() {
	switch (_sequence) {
	case 0:
		return _remove;

	case 1:
		return _remove_key;

	default:
		return Action::NOOP;
	}
}

void Remove::update(const std::string& messages) {
	if (World::question() && messages.find(MESSAGE_WHAT_TO_REMOVE) != std::string::npos) {
		_sequence = 1;
	} else { // Maybe we only removed one thing?
		/* also mark the inventory dirty when we do this */
		Inventory::update();
		_sequence = 2;
	}
}
