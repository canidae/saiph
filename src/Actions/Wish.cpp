#include "Actions/Wish.h"
#include "Actions/Name.h"
#include "Analyzers/Wish.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Globals.h"

using namespace action;

/* constructors/destructor */
Wish::Wish(analyzer::Analyzer* analyzer, unsigned char wand, int priority) : Action(analyzer), _wished(false), _wish("E", priority), _wish_wand(wand, PRIORITY_CONTINUE_ACTION), _append(YES, PRIORITY_CONTINUE_ACTION), _elbereth("Elbereth\r\n", PRIORITY_CONTINUE_ACTION) {
}

Wish::~Wish() {
}

/* methods */
int Wish::id() {
	return ID;
}

bool Wish::canEngrave() {
	return !(Saiph::engulfed() || Saiph::blind() || Saiph::hallucinating() || Saiph::confused() || Saiph::stunned() || (Saiph::extrinsics() & PROPERTY_LEVITATION) != 0 || World::level().tile().symbol() == GRAVE || World::level().tile().symbol() == ALTAR || World::level().tile().symbol() == FOUNTAIN || World::level().tile().symbol() == WATER || World::level().tile().symbol() == LAVA);
}

const Command& Wish::command() {
	switch (_sequence) {
	case 0:
		return _wish;

	case 1:
		return _wish_wand;
		
	case 2:
		return _append;
	
	case 3:
		return _elbereth;
		
	default:
		return Action::NOOP;
	}
}

void Wish::update(const std::string& messages) {

	if (messages.find(MESSAGE_ENGRAVE_WITH) != std::string::npos) {
		_sequence = 1;
	} else if (messages.find(MESSAGE_ENGRAVE_ADD) != std::string::npos) {
		_sequence = 2;
	} else if (messages.find(MESSAGE_ENGRAVE_WHAT) != std::string::npos) {
		_sequence = 3;
	} else if (messages.find(MESSAGE_FOR_WHAT_DO_YOU_WISH) != std::string::npos) {
		_wished = true;
		_sequence = 4;
	} else if (_sequence == 3) {
		_sequence = 4;
	} else if (_sequence < 3) {
		_sequence = 3;
	} else {
		_sequence = 4;
	}
	if (messages.find(MESSAGE_ENGRAVE_ADD) != std::string::npos && !_wished) {
		// aww, empty!
		/* it's interesting that we have to check if we've already made
		 * a wish in this action due to the way we handle actually
		 * making the wish -- it's done in the middle of the action!
		 */
		/*
		 * we need to rethink our strategy of making a new action for every specialized task,
		 * we're getting too many actions, this one could fall under "Zap" or "Engrave".
		 */
		std::string oname = Inventory::items()[_wish_wand.command()[0]].name();
		if (oname.find("EMPTY") == std::string::npos)
			World::queueAction(new action::Name(analyzer(), _wish_wand.command()[0], oname + " EMPTY"));
		_sequence = 4;
	}
}
