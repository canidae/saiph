#include "Analyzer.h"
#include "Amulet.h"
#include "Explore.h"
#include "../World.h"

using namespace analyzer;

/* define static variables */
bool Analyzer::initialized = false;

/* methods */
void Analyzer::init() {
	if (initialized)
		return;
	initialized = true;

	/* init analyzers */
	World::registerAnalyzer(new Amulet());
	World::registerAnalyzer(new Explore());
}

void Analyzer::destroy() {
}
