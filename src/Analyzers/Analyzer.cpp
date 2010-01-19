#include "Analyzers/Analyzer.h"

#include "Analyzers/Amulet.h"
#include "Analyzers/Armor.h"
#include "Analyzers/Beatitude.h"
#include "Analyzers/Donate.h"
#include "Analyzers/Door.h"
#include "Analyzers/Elbereth.h"
#include "Analyzers/Enhance.h"
#include "Analyzers/Excalibur.h"
#include "Analyzers/Explore.h"
#include "Analyzers/Fight.h"
#include "Analyzers/Food.h"
#include "Analyzers/Health.h"
#include "Analyzers/Lamp.h"
#include "Analyzers/Loot.h"
#include "Analyzers/MonsterInfo.h"
#include "Analyzers/Shop.h"
#include "Analyzers/Sokoban.h"
#include "Analyzers/Vault.h"
#include "Analyzers/Weapon.h"
#include "World.h"

using namespace analyzer;

/* static variables */
std::vector<Analyzer*> Analyzer::_analyzers;

/* constructors/destructor */
Analyzer::Analyzer(const std::string& name) : _name(name) {
}

Analyzer::~Analyzer() {
}

/* static methods */
void Analyzer::init() {
	/* init analyzers */
	_analyzers.push_back(new Amulet());
	_analyzers.push_back(new Armor());
	_analyzers.push_back(new Beatitude());
	_analyzers.push_back(new Donate());
	_analyzers.push_back(new Door());
	_analyzers.push_back(new Elbereth());
	_analyzers.push_back(new Enhance());
	_analyzers.push_back(new Excalibur());
	_analyzers.push_back(new Explore());
	_analyzers.push_back(new Fight());
	_analyzers.push_back(new Food());
	_analyzers.push_back(new Health());
	_analyzers.push_back(new Lamp());
	_analyzers.push_back(new Loot());
	_analyzers.push_back(new MonsterInfo());
	_analyzers.push_back(new Shop());
	_analyzers.push_back(new Sokoban());
	_analyzers.push_back(new Vault());
	_analyzers.push_back(new Weapon());

	for (std::vector<Analyzer*>::iterator a = _analyzers.begin(); a != _analyzers.end(); ++a)
		World::registerAnalyzer(*a);
}

void Analyzer::destroy() {
	for (std::vector<Analyzer*>::iterator a = _analyzers.begin(); a != _analyzers.end(); ++a)
		delete *a;
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

void Analyzer::actionCompleted(const std::string&) {
	//default to the parameter-less overload
	actionCompleted();
}

