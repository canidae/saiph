#include "PutOn.h"
#include "../World.h"

using namespace action;
using namespace std;

const int PutOn::id = Action::createID();

PutOn::PutOn(unsigned char key, int priority) : put_on(PUT_ON, priority), put_on_key(string(1, key), PRIORITY_CONTINUE_ACTION) {
}

const Command &PutOn::getCommand() {
	switch (sequence) {
	case 0:
		return put_on;
		
	case 1:
		return put_on_key;

	default:
		return Action::noop;
	}
}

void PutOn::updateAction(const string &messages) {
	if (World::question && messages.find(MESSAGE_WHAT_TO_PUT_ON) != string::npos)
		sequence = 1;
	else if (sequence == 1)
		sequence = 2;
}
