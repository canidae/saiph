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
const Command Action::noop;
int Apply::id = 100;
int Answer::id = 200;
int ApplyInDirection::id = 300;
int Call::id = 400;
int Eat::id = 500;
int EatCorpse::id = 550;
int Engrave::id = 600;
int Fight::id = 700;
int Kick::id = 800;
int ListInventory::id = 900;
int Look::id = 1000;
int Loot::id = 1100;
int Move::id = 1200;
int Name::id = 1300;
int Open::id = 1400;
int Pray::id = 1500;
int PutOn::id = 1600;
int Remove::id = 1700;
int Rest::id = 1800;
int Search::id = 1900;
int Select::id = 2000;
int SelectMultiple::id = 2100;
int Throw::id = 2200;
int Unlock::id = 2300;
