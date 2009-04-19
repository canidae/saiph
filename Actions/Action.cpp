#include "Action.h"
#include "Kick.h"
#include "ListInventory.h"
#include "Look.h"
#include "Move.h"
#include "PutOn.h"
#include "Remove.h"
#include "Search.h"

using namespace action;

/* define static variables */
bool Action::initialized = false;
const Command Action::noop;
int Kick::id = ILLEGAL_ACTION_ID;
int ListInventory::id = ILLEGAL_ACTION_ID;
int Look::id = ILLEGAL_ACTION_ID;
int Move::id = ILLEGAL_ACTION_ID;
int PutOn::id = ILLEGAL_ACTION_ID;
int Remove::id = ILLEGAL_ACTION_ID;
int Search::id = ILLEGAL_ACTION_ID;

/* methods */
void Action::init() {
	if (initialized)
		return;
	initialized = true;

	/* init actions */
	int id = 0;
	Kick::id = ++id;
	ListInventory::id = ++id;
	Look::id = ++id;
	Move::id = ++id;
	PutOn::id = ++id;
	Remove::id = ++id;
	Search::id = ++id;
}

void Action::destroy() {
}
