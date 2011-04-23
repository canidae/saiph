#include "Actions/FarLook.h"

#include "World.h"

#include <vector>
#include <string>

using namespace action;
using namespace std;

/* constructors/destructor */
FarLook::Request::Request(const Point& where) : where(where) {
}

FarLook::FarLook(analyzer::Analyzer* analyzer, vector<FarLook::Request>& requests) : Action(analyzer, false), _current(requests.begin()), _end(requests.end()) {
}

FarLook::~FarLook() {
}

/* methods */
int FarLook::id() {
	return ID;
}

const Command& FarLook::command() {
	if (_current == _end)
		return Action::NOOP;

	_command = Command(";" + World::cursorMoves(Saiph::position(), _current->where) + ".", PRIORITY_LOOK);
	return _command;
}

void FarLook::update(const std::string& output) {
	_current->result = output;
	++_current;
}
