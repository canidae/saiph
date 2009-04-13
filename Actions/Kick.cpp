#include "Kick.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;
using namespace action;

Kick::Kick(unsigned char direction, int priority) : kick(KICK, priority), kick_direction(string(1, direction), PRIORITY_CONTINUE_ACTION) {
}

Kick::~Kick() {
}

const Command &Kick::getCommand() {
	switch (sequence) {
	case 0:
		return kick;
		
	case 1:
		return kick_direction;

	default:
		return Action::noop;
	}
}

void Kick::updateAction(const Saiph *saiph) {
	if (saiph->world->question && saiph->world->messages.find(MESSAGE_IN_WHAT_DIRECTION) != string::npos)
		sequence = 1;
	else if (sequence == 1)
		sequence = 2;
}
