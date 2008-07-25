#include "Analyzer.h"

/* constructor */
Analyzer::Analyzer(string name) : name(name) {
}

/* destructor */
Analyzer::~Analyzer() {
	/* we must have a virtual destructor.
	 * if we don't then the destructor of classes inheriting
	 * this class won't be called upon destruction */
}

/* methods */
void Analyzer::begin() {
	/* decisions should be made here. */
}

void Analyzer::command(string *command) {
	/* this one should be fixed.
	 * it should return pointer to a string instead of this */
}

void Analyzer::finish() {
	/* here we should only set up action depending decisions made earlier.
	 * any requests sent here may be delayed 1 turn! */
}

void Analyzer::inspect(const Point &point) {
	/* this might not be needed in the future */
}

void Analyzer::parseMessages(const string &messages) {
	/* simply parse messages */
}

bool Analyzer::request(const Request &request) {
	/* respond to requests from other analyzers.
	 * this is the reason we shouldn't make decisions in finish,
	 * because when we do, another analyzer may already have done
	 * its finish method */
	return false;
}
