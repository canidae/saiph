#include "Analyzers/Analyzer.h"

#include "Analyzers/Amulet.h"
#include "Analyzers/Armor.h"
#include "Analyzers/Beatitude.h"
#include "Analyzers/Elbereth.h"
#include "Analyzers/Enhance.h"
#include "Analyzers/Excalibur.h"
#include "Analyzers/Explore.h"
#include "Analyzers/Door.h"
#include "Analyzers/Fight.h"
#include "Analyzers/Food.h"
#include "Analyzers/Health.h"
#include "Analyzers/Lamp.h"
#include "Analyzers/Loot.h"
#include "Analyzers/MonsterInfo.h"
#include "Analyzers/Shop.h"
#include "Analyzers/Vault.h"
#include "Analyzers/Weapon.h"
#include "World.h"

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
	World::registerAnalyzer(new Armor());
	World::registerAnalyzer(new Beatitude());
	World::registerAnalyzer(new Door());
	World::registerAnalyzer(new Elbereth());
	World::registerAnalyzer(new Enhance());
	World::registerAnalyzer(new Excalibur());
	World::registerAnalyzer(new Explore());
	World::registerAnalyzer(new Fight());
	World::registerAnalyzer(new Food());
	World::registerAnalyzer(new Health());
	World::registerAnalyzer(new Lamp());
	World::registerAnalyzer(new Loot());
	World::registerAnalyzer(new MonsterInfo());
	World::registerAnalyzer(new Shop());
	World::registerAnalyzer(new Vault());
	World::registerAnalyzer(new Weapon());
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

void Analyzer::actionFailed() {
}