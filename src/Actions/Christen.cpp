#include "Actions/Christen.h"

#include "World.h"
#include "Level.h"
#include "Debug.h"

#include <vector>
#include <string>

using namespace action;
using namespace std;

/* static data */
int Christen::_nao_christen = 0;

/* constructors/destructor */
Christen::Christen(analyzer::Analyzer* analyzer, const vector<pair<Point, string> >& requests) : Action(analyzer, 0), _requests(requests) {
}

Christen::~Christen() {
}

/* methods */
int Christen::id() {
	return ID;
}

const Command& Christen::command() {
	if (_requests.empty())
		return Action::NOOP;

	// probe for incompatible NAO patch
	if (_nao_christen == 0)
		return _command = Command("C ", PRIORITY_LOOK);

	string cmdbuf;
	for (vector<pair<Point, string> >::iterator iter = _requests.begin(); iter != _requests.end(); ++iter) {
		cmdbuf.push_back('C');
		if (_nao_christen > 0)
			cmdbuf.push_back('a');
		string moves = World::cursorMoves(Saiph::position(), iter->first);
		cmdbuf.insert(cmdbuf.end(), moves.begin(), moves.end());
		cmdbuf.push_back('.');
		cmdbuf.append(iter->second);
		cmdbuf.push_back('\n');
	}
	_command = Command(cmdbuf, PRIORITY_LOOK);
	return _command;
}

void Christen::update(const std::string&) {
	string buf(World::lastData());

	if (_nao_christen == 0) {
		// we just probed
		_nao_christen = (buf.find("What do you wish to name?") != string::npos) ? +1 : -1;
	} else {
		_requests.clear();
		Level::clearFarlookData(); // TODO: could be optimized
	}
}
