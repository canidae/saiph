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
void Analyzer::command(string *command) {
}

void Analyzer::finish() {
}

void Analyzer::inspect(const Point &point) {
}

void Analyzer::parseMessages(const string &messages) {
}

bool Analyzer::request(const Request &request) {
	return false;
}

void Analyzer::start() {
}
