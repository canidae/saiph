#include "Actions/FarLook.h"

#include "World.h"
#include "Level.h"
#include "Debug.h"

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

	string cmdbuf;
	for (vector<FarLook::Request>::iterator iter = _current; iter != _end; ++iter) {
		cmdbuf.push_back(';');
		string moves = World::cursorMoves(Saiph::position(), iter->where);
		cmdbuf.insert(cmdbuf.end(), moves.begin(), moves.end());
		cmdbuf.push_back('.');
	}
	_command = Command(cmdbuf, PRIORITY_LOOK);
	return _command;
}

void FarLook::update(const std::string&) {
	string buf(World::lastData());
	map<Point,string> aout;
	for (vector<FarLook::Request>::iterator iter = _current; iter != _end; ++iter) {
		size_t ix = buf.rfind("Pick an object");
		if (ix == string::npos)
			ix = buf.rfind("Please move the cursor to an unknown object");
		if (ix == string::npos) {
			iter->result.clear();
			Debug::custom("FarLook") << "Oops! We didn't get enough replies." << endl;
		} else {
			iter->result.assign(buf.begin() + ix, buf.end());
			buf.erase(ix);
			if ((ix = iter->result.find("\033[H\033[K")) != string::npos)
				iter->result.erase(0, ix+6);
			if ((ix = iter->result.find("\033[H")) != string::npos)
				iter->result.erase(0, ix+3);
			if ((ix = iter->result.find("\033[K")) != string::npos)
				iter->result.erase(ix);
			// XXX: Is it possible for this to wrap?
			iter->result.insert(0, 2, ' ');
			iter->result.append("  ");
			aout[iter->where] = iter->result;
			Debug::custom("FarLook") << iter->where << " => " << iter->result << endl;
		}
	}
	Level::setFarlookResults(aout);

	_current = _end;
}
