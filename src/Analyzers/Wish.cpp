#include "Analyzers/Wish.h"

#include <string>
#include "Actions/Answer.h"
#include "Inventory.h"
#include "Globals.h"
#include "Saiph.h"
#include "World.h"
#include "Debug.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Wish::Wish() : Analyzer("Wish"), wand_of_wishing_key(0), zapping_wand(false), extrinsics(0) {
}

/* methods */
void Wish::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_FOR_WHAT_DO_YOU_WISH, 0) != string::npos) {
		extrinsics = currentExtrinsics();
		string command = "blessed greased fixed +3 " + selectWish();
		Debug::custom(name()) << "Wishing for " << command << endl;
		command.append("\n");
		World::queueAction(static_cast<action::Action*> (new action::Answer(this, command)));
		return;
	} else if (zapping_wand && messages.find(MESSAGE_WHAT_TO_ZAP, 0) != string::npos) {
		zapping_wand = false;
		return;
	}
}

void Wish::analyze() {
	//if we have a wand of wishing, we need to decide if we should zap
	//TODO: get charging and recharge it; currently it'll wrest it before charging
	//since it has no idea of "this wand is empty"
	//if (wand_of_wishing_key != 0) {
		//if (!haveReflection || !haveMR || !wearing("speed boots") || !wearing("gauntlets of power")) {
			//zapping_wand = true;
			//command = ZAP_WAND;
			//priority = PRIORITY_WISH_ZAP_WAND;
		//}
	//}
}

string Wish::selectWish() {
	//if (wish_from_wand && !have_charging)
	//	return "3 scrolls of charging";
	if (!haveMR) {
		if (reflectionArmor)
			return "cloak of magic resistance";
		return "gray dragon scale mail";
	} else if (!haveReflection) {
		if (MRarmor)
			return "amulet of reflection";
		return "silver dragon scale mail";
	} else if (!wearing("speed boots"))
		return "speed boots";
	else if (!wearing("gauntlets of power"))
		return "gauntlets of power";
	//just some survival items
	//TODO: wait until we wear amulets
//	if (!reflectionAmulet)
//		return "amulet of life saving";
	return "potion of full healing";
}

bool Wish::wearing(const string& name) {
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i)
		if (i->second.name().find(name) != string::npos)
			return true;
	return false;
}

unsigned long long Wish::currentExtrinsics() {
	/* TODO: look at other sources of MR, artifacts, etc */
	unsigned long long extrinsics = 0;
	haveReflection = false;
	haveMR = false;
	reflectionArmor = false;
	MRarmor = false;
	MRcloak = false;
	reflectionAmulet = false;
	reflectionShield = false;
	if (wearing("silver dragon scale mail") || wearing("silver dragon scales")) {
		haveReflection = true;
		reflectionArmor = true;
	}
	if (wearing("amulet of reflection")) {
		haveReflection = true;
		reflectionAmulet = true;
	}
	if (wearing("shield of reflection")) {
		haveReflection = true;
		reflectionAmulet = true;
	}
	if (wearing("gray dragon scale mail") || wearing("gray dragon scales")) {
		haveMR = true;
		MRarmor = true;
	}
	if (wearing("cloak of magic resistance")) {
		haveMR = true;
		MRcloak = false;
	}
	if (haveMR)
		extrinsics |= PROPERTY_MAGICRES;
	if (haveReflection)
		extrinsics |= PROPERTY_REFLECTION;
	return extrinsics;
}
	
