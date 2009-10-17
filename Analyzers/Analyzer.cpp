#include "Analyzer.h"
#include "Amulet.h"
#include "Elbereth.h"
#include "Explore.h"
#include "Door.h"
#include "Fight.h"
#include "Food.h"
#include "Health.h"
#include "Loot.h"
#include "../World.h"

using namespace analyzer;

/* define static variables */
bool Analyzer::_initialized = false;

/* methods */
void Analyzer::init() {
	if (_initialized)
		return;
	_initialized = true;

	/* init analyzers */
	World::registerAnalyzer(new Amulet());
	World::registerAnalyzer(new Elbereth());
	World::registerAnalyzer(new Explore());
	World::registerAnalyzer(new Door());
	World::registerAnalyzer(new Fight());
	World::registerAnalyzer(new Food());
	World::registerAnalyzer(new Health());
	World::registerAnalyzer(new Loot());
}

void Analyzer::destroy() {
}
