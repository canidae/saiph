#include "Actions/Fight.h"

#include "Globals.h"

using namespace action;

/* constructors/destructor */
Fight::Fight(analyzer::Analyzer* analyzer, unsigned char direction, int priority, bool attack_friendly) : Action(analyzer), _fight(std::string("F").append(std::string(1, direction)), priority), _attack_friendly(attack_friendly ? YES : NO, PRIORITY_CONTINUE_ACTION) {
}

Fight::~Fight() {
}

/* methods */
int Fight::id() {
	return ID;
}

const Command& Fight::command() {
	switch (_sequence) {
	case 0:
		return _fight;

	case 1:
		return _attack_friendly;

	default:
		return Action::NOOP;
	}
}

void Fight::update(const std::string& messages) {
	if (_sequence == 0) {
		if (messages.find(MESSAGE_REALLY_ATTACK) != std::string::npos)
			_sequence = 1;
		else
			_sequence = 2;
	} else if (_sequence == 1) {
		_sequence = 2;
	}
}
