#include "Actions/Engrave.h"

using namespace action;

/* constructors/destructor */
Engrave::Engrave(analyzer::Analyzer* analyzer, const std::string& engrave_msg, unsigned char engrave_with, int priority, bool should_append) : Action(analyzer), _engrave("E", priority), _message(engrave_msg, PRIORITY_CONTINUE_ACTION), _item(engrave_with, PRIORITY_CONTINUE_ACTION), _append(should_append ? YES : NO, PRIORITY_CONTINUE_ACTION) {
}

Engrave::~Engrave() {
}

/* methods */
bool Engrave::canEngrave() {
	return !(Saiph::engulfed() || Saiph::blind() || Saiph::hallucinating() || Saiph::confused() || Saiph::stunned() || (Saiph::extrinsics() & PROPERTY_LEVITATION) != 0 || World::level().tile().symbol() == GRAVE || World::level().tile().symbol() == ALTAR || World::level().tile().symbol() == FOUNTAIN || World::level().tile().symbol() == WATER || World::level().tile().symbol() == LAVA);
}

int Engrave::id() {
	return ID;
}

const Command& Engrave::command() {
	switch (_sequence) {
	case 0:
		return _engrave;

	case 1:
		return _item;

	case 2:
		return _append;

	case 3:
		return _message;

	default:
		return Action::NOOP;
	}
}

void Engrave::update(const std::string& messages) {
	if (messages.find(MESSAGE_ENGRAVE_WITH) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_ENGRAVE_ADD) != std::string::npos) {
		_sequence = 2;
	} else if (messages.find(MESSAGE_ENGRAVE_WHAT) != std::string::npos) {
		_sequence = 3;
	} else if (messages.find(MESSAGE_FOR_WHAT_DO_YOU_WISH) != std::string::npos) {
		// oh boy!
		Item& wandWishing = Inventory::items()[_item.command()[0]];
		wandWishing.name("wand of wishing"); // update name
		_sequence = 4;
	} else if (messages.find(MESSAGE_WISHING_NO_CHARGES) != std::string::npos) {
		Item& wandWishing = Inventory::items()[_item.command()[0]];
		wandWishing.name("wand of wishing named EMPTY"); // update name
		_sequence = 4;
	} else if (_sequence < 3) {
		_sequence = 3;
	} else if (_sequence == 3) {
		_sequence = 4;
	} else {
		_sequence = 4;
	}
}
