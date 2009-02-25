#include "Analyzer.h"
#include "Globals.h"

using namespace std;

/* constructors/destructor */
Analyzer::Analyzer(string name) : name(name), priority(ILLEGAL_PRIORITY), command(""), expired(false) {
}

Analyzer::~Analyzer() {
}

/* methods */
void Analyzer::analyze(const string &) {
	/* called each "frame" */
}

void Analyzer::complete() {
	/* called when this analyzer "won" the priority race */
}

void Analyzer::fail() {
	/* called if command from this analyzer repeatedly fail */
}

void Analyzer::init() {
	/* called once at startup */
}

bool Analyzer::request(const Request &) {
	/* called each time an analyzer requests something */
	return false;
}
