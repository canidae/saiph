#include "Remove.h"
#include "../World.h"

using namespace std;
using namespace action;

const int Remove::id = Action::createID();

Remove::Remove(unsigned char key, int priority) : remove(REMOVE, priority), remove_key(string(1, key), PRIORITY_CONTINUE_ACTION) {
}

const Command &Remove::getCommand() {
	switch (sequence) {
	case 0:
		return remove;
		
	case 1:
		return remove_key;

	default:
		return Action::noop;
	}
}

void Remove::updateAction() {
	if (World::question && World::messages.find(MESSAGE_WHAT_TO_REMOVE) != string::npos)
		sequence = 1;
	else if (sequence == 1)
		sequence = 2;
}
