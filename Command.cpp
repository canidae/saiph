#include "Command.h"

using namespace std;

/* constructors/destructor */
Command::Command(int priority, const string &data) : priority(priority), data(data) {
}

Command::~Command() {
}

/* methods */
void Command::setCommand(int priority, const string &data) {
	this->priority = priority;
	this->data = data;
}
