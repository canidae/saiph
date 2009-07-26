#include "Action.h"
#include "Answer.h"
#include "Apply.h"
#include "ApplyInDirection.h"
#include "Call.h"
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
const Command Action::noop;
int Apply::id = 1;
int Answer::id = 2;
int ApplyInDirection::id = 3;
int Call::id = 4;
int Engrave::id = 5;
int Fight::id = 6;
int Kick::id = 7;
int ListInventory::id = 8;
int Look::id = 9;
int Loot::id = 10;
int Move::id = 11;
int Name::id = 12;
int Open::id = 13;
int Pray::id = 14;
int PutOn::id = 15;
int Remove::id = 16;
int Rest::id = 17;
int Search::id = 18;
int Select::id = 19;
int SelectMultiple::id = 20;
int Throw::id = 21;
int Unlock::id = 22;
