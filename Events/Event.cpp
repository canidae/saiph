#include "Event.h"
#include "ChangedInventoryItems.h"
#include "EatItem.h"
#include "ElberethQuery.h"
#include "ItemsOnGround.h"
#include "ReceivedItems.h"
#include "StashChanged.h"
#include "TakeMeThere.h"
#include "WantItems.h"
#include "../EventBus.h"

using namespace event;

/* define static variables */
const int ChangedInventoryItems::ID = 0;
const int EatItem::ID = 1;
const int ElberethQuery::ID = 2;
const int ItemsOnGround::ID = 3;
const int ReceivedItems::ID = 4;
const int StashChanged::ID = 5;
const int TakeMeThere::ID = 6;
const int WantItems::ID = 7;