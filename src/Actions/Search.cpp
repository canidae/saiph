#include "Actions/Search.h"

using namespace action;

/* constructors/destructor */
Search::Search(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _turn_before_search(World::turn()), _search("16s", priority) {
}

Search::~Search() {
}

/* methods */
int Search::id() {
	return ID;
}

const Command& Search::command() {
	switch (_sequence) {
	case 0:
		return _search;

	default:
		return Action::NOOP;
	}
}

void Search::update(const std::string&) {
	if (_sequence == 0) {
		/* increase search counter on level */
		World::level().increaseAdjacentSearchCount(Saiph::position(), World::turn() - _turn_before_search);
		_sequence = 1;
	}
}
