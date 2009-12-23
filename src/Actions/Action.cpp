#include "Actions/Action.h"

#include "Actions/Answer.h"
#include "Actions/Apply.h"
#include "Actions/ApplyInDirection.h"
#include "Actions/Call.h"
#include "Actions/Drop.h"
#include "Actions/DropGold.h"
#include "Actions/Eat.h"
#include "Actions/EatCorpse.h"
#include "Actions/Engrave.h"
#include "Actions/Enhance.h"
#include "Actions/FarLook.h"
#include "Actions/Fight.h"
#include "Actions/Kick.h"
#include "Actions/ListInventory.h"
#include "Actions/ListPlayerAttributes.h"
#include "Actions/Look.h"
#include "Actions/Loot.h"
#include "Actions/Move.h"
#include "Actions/Name.h"
#include "Actions/Open.h"
#include "Actions/Pray.h"
#include "Actions/PutOn.h"
#include "Actions/Remove.h"
#include "Actions/Rest.h"
#include "Actions/Search.h"
#include "Actions/TakeOff.h"
#include "Actions/Throw.h"
#include "Actions/Travel.h"
#include "Actions/Unlock.h"
#include "Actions/Wear.h"
#include "Actions/Wield.h"

using namespace action;

/* define static variables */
const Command Action::NOOP;
const int Answer::ID = 1;
const int Apply::ID = 2;
const int ApplyInDirection::ID = 3;
const int Call::ID = 4;
const int Eat::ID = 5;
const int EatCorpse::ID = 6;
const int Engrave::ID = 7;
const int Fight::ID = 8;
const int Kick::ID = 9;
const int ListInventory::ID = 10;
const int Look::ID = 11;
const int Loot::ID = 12;
const int Move::ID = 13;
const int Name::ID = 14;
const int Open::ID = 15;
const int Pray::ID = 16;
const int PutOn::ID = 17;
const int Remove::ID = 18;
const int Rest::ID = 19;
const int Search::ID = 20;
const int Throw::ID = 21;
const int Unlock::ID = 22;
const int FarLook::ID = 23;
const int Enhance::ID = 24;
const int Drop::ID = 25;
const int DropGold::ID = 26;
const int ListPlayerAttributes::ID = 27;
const int Wield::ID = 28;
const int TakeOff::ID = 29;
const int Wear::ID = 30;
const int Travel::ID = 31;

/* constructors/destructor */
Action::Action(analyzer::Analyzer* analyzer, bool increase_turn_counter) : _sequence(0), _analyzer(analyzer), _increase_turn_counter(increase_turn_counter) {
}

Action::~Action() {
}

/* methods */
analyzer::Analyzer* Action::analyzer() {
	return _analyzer;
}

bool Action::increaseTurnCounter() {
	return _increase_turn_counter;
}
