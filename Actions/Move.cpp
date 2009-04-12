#include "Move.h"

using namespace std;
using namespace action;

Move::Move(unsigned char direction, int priority) : move(string(1, direction), priority) {
	/* XXX:
	 * should we check that the user gave a valid direction?
	 * i say no, it would be an unnecessary cost. this class will not check
	 * if we can move, analyzer should do that before it does its heavy
	 * calculation that figures out where to move, and then the analyzer
	 * should check if the move is legal at the same time (if necessary) */
}

Move::~Move() {
}

const Command &Move::execute(Saiph *) {
	if (sequence == 0)
		return move;
	return Action::noop;
}
