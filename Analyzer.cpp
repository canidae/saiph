#include "Analyzer.h"
#include "Debug.h"
#include "Globals.h"

using namespace std;

/* constructors/destructor */
Analyzer::Analyzer(string name) : name(name), commands(1), sequence(-1), last_sequence(-1) {
	/* methods will be called in this order:
	 * 0. init (once, after all analyzers have been created)
	 * 1. parseMessages
	 * 2. inspect (unless question, menu or engulfed)
	 * 3. analyze (unless question or menu)
	 * 3. complete/fail (only called for analyzer that got its command through)
	 *
	 * additionally, requests may come at any time */
}

Analyzer::~Analyzer() {
	/* we must have a virtual destructor.
	 * if we don't then the destructor of classes inheriting
	 * this class won't be called upon destruction */
}

/* methods */
void Analyzer::analyze() {
}

void Analyzer::clearCommands() {
	Debug::notice() << "Clearing commands for analyzer '" << name << "'" << endl;
	sequence = -1;
	last_sequence = -1;
}

void Analyzer::complete() {
}

void Analyzer::fail() {
}

string &Analyzer::getCommand() {
	if (sequence >= 0 && sequence <= last_sequence)
		return commands[sequence].data;
	/* this shouldn't happen */
	setCommand(0, ILLEGAL_PRIORITY, SEARCH);
	return commands[0].data;
}

int Analyzer::getPriority() {
	Debug::notice() << name << " getPriority(): " << last_sequence << " - " << sequence << endl;
	if (sequence >= 0 && sequence <= last_sequence)
		return commands[sequence].priority;
	return ILLEGAL_PRIORITY;
}

void Analyzer::init() {
}

void Analyzer::inspect(const Point &point) {
}

void Analyzer::parseMessages(const string &messages) {
}

bool Analyzer::rememberCommand() {
	if (sequence < 0 || sequence > last_sequence)
		return true; // we got no command, we'll remember that :)
	return commands[sequence].remember;
}

bool Analyzer::request(const Request &request) {
	return false;
}

/* protected methods */
bool Analyzer::setCommand(int index, int priority, const string &data, bool remember) {
	/* if "remember" is true then the analyzer must itself find out if
	 * the command was called or not. preferably by looking at the
	 * messages we received, or using "complete()" if we don't get a
	 * helpful message that can tell us if the command succeeded or not */
	Debug::notice() << "'" << name << "' setting command: " << index << " - " << priority << " - " << data << " - " << remember << endl;
	if (index < 0 || index > (int) commands.size())
		return false;
	if (index > last_sequence)
		last_sequence = index;
	if (index < (int) commands.size())
		commands[index].setCommand(priority, data, remember);
	else
		commands.push_back(Command(priority, data, remember));
	return true;
}
