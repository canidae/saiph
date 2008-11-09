#include "Command.h"

using namespace std;

/* constructors/destructor */
Command::Command(int priority, const string &data, bool remember) : priority(priority), data(data), remember(remember) {
}

Command::~Command() {
}

/* methods */
void Command::setCommand(int priority, const string &data, bool remember) {
	this->priority = priority;
	this->data = data;
	this->remember = remember;
}
