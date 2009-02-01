#include "Wish.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../Debug.h"
#include <string>

using namespace std;

/* constructors/destructor */
Wish::Wish(Saiph *saiph) : Analyzer("Wish"), saiph(saiph), wand_of_wishing_key(0) {
}

/* methods */
void Wish::parseMessages(const string &messages) {
	if (messages.find(WISH_MESSAGE, 0) != string::npos) {
		command = "3 blessed greased fixed +3 " + selectWish();
		Debug::notice() << "[Wish       ] Wishing for " << command << endl;
		command.append("\n");
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	}
	MRarmor = wearing("gray dragon scale mail") || wearing("gray dragon scales");
	MRcloak = wearing("cloak of magic resistance");
	haveMR = MRarmor || MRcloak;
	reflectionArmor = wearing("silver dragon scale mail") || wearing("silver dragon scales");
	reflectionAmulet = wearing("amulet of reflection");
	reflectionShield = wearing("shield of reflection");
	haveReflection = reflectionArmor || reflectionAmulet || reflectionShield;
	if (saiph->inventory_changed) {
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i)
			if (i->second.name == "wand of wishing") {
				wand_of_wishing_key = i->first;
			}
	}
}

void Wish::analyze() {
	//if we have a wand of wishing, we need to decide if we should zap
	//TODO: get charging and recharge it; currently it'll wrest it before charging
	//since it has no idea of "this wand is empty"
	if (wand_of_wishing_key != 0) {
		if (!haveReflection || !haveMR || !wearing("speed boots") || !wearing("gauntlets of power")) {
			command = ZAP_WAND;
			priority = PRIORITY_WISH_ZAP_WAND;
		}
	}
}

string Wish::selectWish() {
	//if (wish_from_wand && !have_charging)
	//	return "3 scrolls of charging";
	if (!haveReflection) {
		if (MRarmor)
			return "amulet of reflection";
		return "silver dragon scale mail";
	} else if (!haveMR) {
		if (reflectionArmor)
			return "cloak of magic resistance";
		return "gray dragon scale mail";
	} else if (!wearing("speed boots"))
		return "speed boots";
	else if (!wearing("gauntlets of power"))
		return "gauntlets of power";
	//just some survival items
	if (!reflectionAmulet)
		return "amulet of life saving";
	return "potion of full healing";
}

bool Wish::wearing(const string& name) {
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i)
		if (i->second.name.find(name) != string::npos)
			return true;
	return false;
}
