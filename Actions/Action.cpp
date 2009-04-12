#include "Action.h"

using namespace std;
using namespace action;

const Command Action::noop;

Action::Action() : sequence(0) {
}

Action::~Action() {
}

const Command &Action::execute(Saiph *) {
	return noop;
}
