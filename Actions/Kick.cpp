#include "Kick.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;
using namespace action;

Kick::Kick(unsigned char direction, int priority) : kick(KICK, priority), kick_direction(string(1, direction), PRIORITY_CONTINUE_ACTION) {
}

Kick::~Kick() {
}

const Command &Kick::execute(Saiph *saiph) {
	if (sequence == 0)
		return kick;
	else if (sequence == 1 && saiph->world->question && saiph->world->messages.find(MESSAGE_IN_WHAT_DIRECTION))
		return kick_direction;
	return Action::noop;
}
