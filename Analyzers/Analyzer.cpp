#include "Analyzer.h"
#include "Amulet.h"
#include "Elbereth.h"
#include "Explore.h"
#include "Door.h"
#include "Fight.h"
#include "Loot.h"
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
	World::registerAnalyzer(new Elbereth());
	World::registerAnalyzer(new Explore());
	World::registerAnalyzer(new Door());
	World::registerAnalyzer(new Fight());
	World::registerAnalyzer(new Loot());
}

void Analyzer::destroy() {
}
