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

/* constructors/destructor */
Analyzer::Analyzer(const std::string& name) : _name(name) {
}

Analyzer::~Analyzer() {
}

/* static methods */
void Analyzer::init() {
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

/* methods */
const std::string& Analyzer::name() {
	return _name;
}

void Analyzer::parseMessages(const std::string&) {
}

void Analyzer::analyze() {
}

void Analyzer::onEvent(event::Event * const) {
}
