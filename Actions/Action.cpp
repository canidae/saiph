#include "Action.h"

using namespace std;
using namespace action;

const Command Action::noop;

Action::Action() : sequence(0) {
}

Action::~Action() {
}

const Command &Action::getCommand() {
	return noop;
}

void Action::updateAction(const Saiph *) {
	/* called on the action object that was executed */
	return;
}
