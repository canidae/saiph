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
#include "PutOn.h"
#include "Remove.h"
#include "Search.h"
#include "Select.h"
#include "Throw.h"
#include "Unlock.h"

using namespace action;

/* define static variables */
const Command Action::noop;
int Apply::id = 1;
int Answer::id = 101;
int ApplyInDirection::id = 201;
int Call::id = 301;
int Engrave::id = 401;
int Fight::id = 501;
int Kick::id = 601;
int ListInventory::id = 701;
int Look::id = 801;
int Loot::id = 901;
int Move::id = 1001;
int Name::id = 1101;
int Open::id = 1201;
int PutOn::id = 1301;
int Remove::id = 1401;
int Search::id = 1501;
int Select::id = 1601;
int Throw::id = 1701;
int Unlock::id = 1801;
