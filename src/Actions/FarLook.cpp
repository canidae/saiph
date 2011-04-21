#include "Actions/FarLook.h"

using namespace action;

/* constructors/destructor */
FarLook::FarLook(analyzer::Analyzer* analyzer, const Point& location) : Action(analyzer, false), _far_look(";" + World::cursorMoves(Saiph::position(), location) + ".", PRIORITY_LOOK) {
}

FarLook::~FarLook() {
}

/* methods */
int FarLook::id() {
	return ID;
}

const Command& FarLook::command() {
	switch (_sequence) {
	case 0:
		return _far_look;

	default:
		return Action::NOOP;
	}
}

void FarLook::update(const std::string&) {
	if (_sequence == 0)
		_sequence = 1;
}
