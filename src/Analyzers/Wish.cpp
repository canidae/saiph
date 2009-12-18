#include "Analyzers/Wish.h"

#include <string>
#include "Globals.h"
#include "Saiph.h"
#include "World.h"
#include "Debug.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Wish::Wish(Saiph* saiph) : Analyzer("Wish"), saiph(saiph), wand_of_wishing_key(0), zapping_wand(false) {
}

/* methods */
void Wish::parseMessages(const string& messages) {
	if (saiph->inventory_changed) {
		wand_of_wishing_key = 0;
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name == "wand of wishing") {
				wand_of_wishing_key = i->first;
			}
		}
	}
	if (messages.find(MESSAGE_FOR_WHAT_DO_YOU_WISH, 0) != string::npos) {
		haveMR = saiph->world->player.extrinsics & PROPERTY_MAGICRES;
		if (haveMR) {
			MRarmor = wearing("gray dragon scale mail") || wearing("gray dragon scales");
			MRcloak = wearing("cloak of magic resistance");
		} else
			MRarmor = MRcloak = false;
		haveReflection = saiph->world->player.extrinsics & PROPERTY_REFLECTION;
		if (haveReflection) {
			reflectionArmor = wearing("silver dragon scale mail") || wearing("silver dragon scales");
			reflectionAmulet = wearing("amulet of reflection");
			reflectionShield = wearing("shield of reflection");
		} else
			reflectionArmor = reflectionAmulet = reflectionShield = false;
		command = "3 blessed greased fixed +3 " + selectWish();
		Debug::notice(saiph->last_turn) << WISH_DEBUG_NAME << "Wishing for " << command << endl;
		command.append("\n");
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	} else if (zapping_wand && messages.find(MESSAGE_WHAT_TO_ZAP, 0) != string::npos) {
		zapping_wand = false;
		command = wand_of_wishing_key;
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	}
}

void Wish::analyze() {
	//if we have a wand of wishing, we need to decide if we should zap
	//TODO: get charging and recharge it; currently it'll wrest it before charging
	//since it has no idea of "this wand is empty"
	if (wand_of_wishing_key != 0) {
		if (!haveReflection || !haveMR || !wearing("speed boots") || !wearing("gauntlets of power")) {
			zapping_wand = true;
			command = ZAP_WAND;
			priority = PRIORITY_WISH_ZAP_WAND;
		}
	}
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
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i)
		if (i->second.name.find(name) != string::npos)
			return true;
	return false;
}
