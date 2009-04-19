#include "Command.h"

using namespace std;

ostream &operator<<(ostream &os, const Command &c) {
	return os << "(" << c.priority << "): " << c.command;
}
