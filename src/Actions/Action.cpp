#include "Actions/Action.h"

#include "Analyzers/Analyzer.h"
#include "Actions/Answer.h"
#include "Actions/Apply.h"
#include "Actions/ApplyInDirection.h"
#include "Actions/Call.h"
#include "Actions/Charge.h"
#include "Actions/CallMonster.h"
#include "Actions/Donate.h"
#include "Actions/Dip.h"
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
#include "Actions/MergeStack.h"
#include "Actions/Move.h"
#include "Actions/Name.h"
#include "Actions/Open.h"
#include "Actions/Pay.h"
#include "Actions/Pray.h"
#include "Actions/PutOn.h"
#include "Actions/Remove.h"
#include "Actions/Rest.h"
#include "Actions/Search.h"
#include "Actions/TakeOff.h"
#include "Actions/Throw.h"
#include "Actions/Unlock.h"
#include "Actions/Wear.h"
#include "Actions/Wield.h"
#include "Actions/Wish.h"

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
const int Dip::ID = 31;
const int Donate::ID = 32;
const int Pay::ID = 33;
const int Wish::ID = 34;
const int Charge::ID = 35;
const int CallMonster::ID = 36;
const int MergeStack::ID = 37;

/** compilation of info on nethack nomul(<0) call sites
 *  Digesting mobs		(1 + (cwt >> 8)) * (1 + !!Slow_digestion)  "You digest"
 *  Floating eye		2d70 or 127
 *  Gelatinous cube		1d4  "You are frozen by"
 *  Chest trap			5d6  "Suddenly you are frozen in place!"
 *  Stoning			3
 *  Fumbling			2  "Egads!  %s bite%s your %s!" "You trip over %s."  "%s %s%s on the ice."  "You trip over your own %s."  "You slip %s."  "Your %s slip out of the stirrups."  "You let go of the reins."  "You bang into the saddle-horn."  "You slide to one side of the saddle."
 *  Clouds			3  "Kaboom!!!  Boom!!  Boom!!"
 *  Sleeping			varies  "You wake up."
 *  Nymphs			varies  "%s seduces you and %s off your %s."  "%s charms you.  You gladly %s your %s."
 *  Failing to learn a spell	varies
 *  Chatting with a skeleton	2
 *  Temple ghost		3  "You regain your composure."
 *  Demon summoning		3  "You are terrified, and unable to move."
 *  Praying			3  "You finish your prayer."
 *  Turn undead			5
 *  Potion ghost		3  "You regain your composure."
 *  Potion of paralysis		varies
 *  Paralysis (thrown)		1d5  "Something seems to be holding you." / "You can move again."
 *  Sleeping (thrown)		1d5  "You can move again."
 *  Trapped chest		1  "You set it off!" / "You trigger a trap!"
 *  Monster hit			1d10  "You are frozen
 *  CLC_PARALYZE		1 (Antimagic | Free action) "You stiffen briefly." / varies "You are frozen in place!"
 *  Dragging an iron ball	2
 *  Eating a mimic		variest
 *  Rotten food (possible)	1d10 "The world spins and
 *  Fainting			>10 "You faint from lack of food"
 *  Vomiting			2 "You stuff yourself and then vomit voluminously." / "The water is foul!  You gag and vomit." / "Gaggg... this tastes like sewage!  You vomit." / "You suddenly vomit!"
 *  Hurtling 3rd law		varies no message with !verbose
 *  Wearing stuff		varies
 *  Pick axe on web		2d2
 *  Crystal ball		1d10
 *  "The magic-absorbing blade scares you!"	3
 *  Cursed bell summoning	possible 2
 *  Jumping			1
 */

/* used for the travel command "_" */
Point Move::_last_target;

/* constructors/destructor */
Action::Action(analyzer::Analyzer* analyzer, int time_taken) : _sequence(0), _time_taken(time_taken), _analyzer(analyzer) {
}

Action::~Action() {
}

/* methods */
analyzer::Analyzer* Action::analyzer() {
	return _analyzer;
}

int Action::timeTaken() {
	return _time_taken;
}

void Action::failed() {
	_analyzer->actionFailed();
}
