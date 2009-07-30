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
int Action::id_counter = 0;
const Command Action::noop;
int Apply::id = NO_ACTION;
int Answer::id = NO_ACTION;
int ApplyInDirection::id = NO_ACTION;
int Call::id = NO_ACTION;
int Eat::id = NO_ACTION;
int EatCorpse::id = NO_ACTION;
int Engrave::id = NO_ACTION;
int Fight::id = NO_ACTION;
int Kick::id = NO_ACTION;
int ListInventory::id = NO_ACTION;
int Look::id = NO_ACTION;
int Loot::id = NO_ACTION;
int Move::id = NO_ACTION;
int Name::id = NO_ACTION;
int Open::id = NO_ACTION;
int Pray::id = NO_ACTION;
int PutOn::id = NO_ACTION;
int Remove::id = NO_ACTION;
int Rest::id = NO_ACTION;
int Search::id = NO_ACTION;
int Select::id = NO_ACTION;
int SelectMultiple::id = NO_ACTION;
int Throw::id = NO_ACTION;
int Unlock::id = NO_ACTION;

/* methods */
void Action::init() {
	if (id_counter > 0)
		return;

	/* set ID for actions */
	Apply::id = ++id_counter;
	Answer::id = ++id_counter;
	ApplyInDirection::id = ++id_counter;
	Call::id = ++id_counter;
	Eat::id = ++id_counter;
	EatCorpse::id = ++id_counter;
	Engrave::id = ++id_counter;
	Fight::id = ++id_counter;
	Kick::id = ++id_counter;
	ListInventory::id = ++id_counter;
	Look::id = ++id_counter;
	Loot::id = ++id_counter;
	Move::id = ++id_counter;
	Name::id = ++id_counter;
	Open::id = ++id_counter;
	Pray::id = ++id_counter;
	PutOn::id = ++id_counter;
	Remove::id = ++id_counter;
	Rest::id = ++id_counter;
	Search::id = ++id_counter;
	Select::id = ++id_counter;
	SelectMultiple::id = ++id_counter;
	Throw::id = ++id_counter;
	Unlock::id = ++id_counter;
}

void Action::destroy() {
}
