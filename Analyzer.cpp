#include "Analyzer.h"

/* destructor */
Analyzer::~Analyzer() {
	/* we must have a virtual destructor.
	 * if we don't then the destructor of classes inheriting
	 * this class won't be called upon destruction */
}

/* methods */
void Analyzer::command(string *command) {
}

int Analyzer::finish() {
	return 0;
}

void Analyzer::inspect(const Point &point, unsigned char symbol) {
}

int Analyzer::parseMessages(string *messages) {
	return 0;
}

bool Analyzer::requestAction(const Request &request) {
	return false;
}

int Analyzer::start() {
	return 0;
}
