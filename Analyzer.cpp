#include "Analyzer.h"

/* constructor */
Analyzer::Analyzer(string name) : name(name), priority(ILLEGAL_PRIORITY), command("") {
	/* methods will be called in this order:
	 * 1. parseMessages
	 * 2. inspect
	 * 3. analyze
	 * 3. complete (only called for analyzer that got its command through)
	 *
	 * additionally, requests may come at any time */
}

/* destructor */
Analyzer::~Analyzer() {
	/* we must have a virtual destructor.
	 * if we don't then the destructor of classes inheriting
	 * this class won't be called upon destruction */
}

/* methods */
void Analyzer::analyze() {
}

void Analyzer::complete() {
}

void Analyzer::inspect(const Point &point) {
}

void Analyzer::parseMessages(const string &messages) {
}

bool Analyzer::request(const Request &request) {
	return false;
}
