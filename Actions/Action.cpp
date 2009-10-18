#include "Action.h"
#include "Answer.h"
#include "Apply.h"
#include "ApplyInDirection.h"
#include "Call.h"
#include "Eat.h"
#include "EatCorpse.h"
#include "Engrave.h"
#include "Fight.h"
#include "Kick.h"
#include "ListInventory.h"
#include "Look.h"
#include "Loot.h"
#include "Move.h"
#include "Name.h"
#include "Open.h"
#include "Pray.h"
#include "PutOn.h"
#include "Remove.h"
#include "Rest.h"
#include "Search.h"
#include "Select.h"
#include "SelectMultiple.h"
#include "Throw.h"
#include "Unlock.h"

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
const int Select::ID = 21;
const int SelectMultiple::ID = 22;
const int Throw::ID = 23;
const int Unlock::ID = 24;

/* constructors/destructor */
Action::Action(analyzer::Analyzer* analyzer) : _sequence(0), _analyzer(analyzer) {
}

Action::~Action() {
}

/* methods */
analyzer::Analyzer* Action::analyzer() {
	return _analyzer;
}
