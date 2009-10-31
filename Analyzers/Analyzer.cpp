#include "Analyzer.h"

#include "Amulet.h"
#include "Beatitude.h"
#include "Elbereth.h"
#include "Enhance.h"
#include "Explore.h"
#include "Door.h"
#include "Fight.h"
#include "Food.h"
#include "Health.h"
#include "Lamp.h"
#include "Loot.h"
#include "MonsterInfo.h"
#include "Shop.h"
#include "Vault.h"
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
	World::registerAnalyzer(new Beatitude());
	World::registerAnalyzer(new Door());
	World::registerAnalyzer(new Elbereth());
	World::registerAnalyzer(new Enhance());
	World::registerAnalyzer(new Explore());
	World::registerAnalyzer(new Fight());
	World::registerAnalyzer(new Food());
	World::registerAnalyzer(new Health());
	World::registerAnalyzer(new Lamp());
	World::registerAnalyzer(new Loot());
	World::registerAnalyzer(new MonsterInfo());
	World::registerAnalyzer(new Shop());
	World::registerAnalyzer(new Vault());
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

void Analyzer::actionCompleted() {
}