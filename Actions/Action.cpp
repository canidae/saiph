#include "Action.h"
#include "Answer.h"
#include "Apply.h"
#include "ApplyInDirection.h"
#include "Fight.h"
#include "Kick.h"
#include "ListInventory.h"
#include "Look.h"
#include "Loot.h"
#include "Move.h"
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
int Answer::id = 2;
int ApplyInDirection::id = 3;
int Fight::id = 5;
int Kick::id = 10;
int ListInventory::id = 20;
int Look::id = 30;
int Loot::id = 35;
int Move::id = 40;
int Open::id = 45;
int PutOn::id = 50;
int Remove::id = 60;
int Search::id = 70;
int Select::id = 80;
int Throw::id = 90;
int Unlock::id = 100;
