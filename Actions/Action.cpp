#include "Action.h"

using namespace std;
using namespace action;

const Command Action::noop;

Action::Action() {
}

Action::~Action() {
}

const Command &Action::execute(Saiph *) {
	return noop;
}
