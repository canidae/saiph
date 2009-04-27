#include "Action.h"
#include "Fight.h"
#include "Kick.h"
#include "ListInventory.h"
#include "Look.h"
#include "Move.h"
#include "PutOn.h"
#include "Remove.h"
#include "Search.h"

using namespace action;

/* define static variables */
const Command Action::noop;
int Fight::id = 1;
int Kick::id = 10;
int ListInventory::id = 20;
int Look::id = 30;
int Move::id = 40;
int PutOn::id = 50;
int Remove::id = 60;
int Search::id = 70;
