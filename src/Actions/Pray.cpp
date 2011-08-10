#include "Actions/Pray.h"

#include "Saiph.h"
#include "World.h"

using namespace action;

/* constructors/destructor */
Pray::Pray(analyzer::Analyzer* analyzer, int priority) : Action(analyzer, -3), _pray("#pray\n", priority) {
}

Pray::~Pray() {
}

/* methods */
bool Pray::isSafeToPray() {
	return World::turn() - PRAY_PRAYER_TIMEOUT > Saiph::lastPrayed();
}

bool Pray::isSafeToPrayMajorTrouble() {
	return World::turn() - PRAY_MAJOR_TROUBLE > Saiph::lastPrayed();
}

int Pray::id() {
	return ID;
}

const Command& Pray::command() {
	switch (_sequence) {
	case 0:
		return _pray;

	default:
		return Action::NOOP;
	}
}

void Pray::update(const std::string& messages) {
	if (messages.find(MESSAGE_YOU_FINISH_YOUR_PRAYER) != std::string::npos) {
		Saiph::lastPrayed(World::turn());
		_sequence = 1;
	}
}
