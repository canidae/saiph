#include "Armor.h"
#include "../Item.h"
#include "../Player.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Debug.h"

using namespace std;

/* constructors/destructor */
Armor::Armor(Saiph *saiph) : Analyzer("Armor"), saiph(saiph), wear_armor(false), last_armor_type(0), last_polymorphed(false) {
	resetCanWear();
}

/* methods */
void Armor::parseMessages(const string &messages) {
	string::size_type pos;

	if (saiph->world->player.polymorphed != last_polymorphed) {
		// We changed forms, so assume we can wear all types of armor again.
		resetCanWear();
		last_polymorphed = saiph->world->player.polymorphed;
	}
	if (!command2.empty() && (messages.find(MESSAGE_DONT_EVEN_BOTHER, 0) != string::npos ||
			messages.find(ARMOR_HAVE_NO_FEET, 0) != string::npos ||
			messages.find(ARMOR_WONT_FIT_HORN, 0) != string::npos ||
			messages.find(ARMOR_TOO_MANY_HOOVES, 0) != string::npos)) {
		/* We tried to wear something and it didn't work. Polymorphed? */
		can_wear[last_armor_type] = false;
		wear_armor = false;
		command2.clear();
	} else if (saiph->world->question && !command2.empty() && (messages.find(MESSAGE_WHAT_TO_WEAR, 0) != string::npos || messages.find(MESSAGE_WHAT_TO_TAKE_OFF, 0) != string::npos)) {
		/* take off or wear something */
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (!command2.empty() && messages.find(MESSAGE_YOU_WERE_WEARING, 0) != string::npos) {
		/* took off last piece of armor (no "what do you want to take off?" question then) */
		command2.clear();
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (saiph->world->question && (pos = messages.find(MESSAGE_FOOCUBUS_QUESTION, 0)) != string::npos) {
		/* A foocubus wants to remove something. Check the beatitude of
		 * that piece of armor! */
		command = NO;
		priority = PRIORITY_CONTINUE_ACTION;
		if (messages.find(" cloak, ", pos) == pos
				|| messages.find(" wrapping, ", pos) == pos
				|| messages.find(" robe, ", pos) == pos
				|| messages.find(" apron, ", pos) == pos
				|| messages.find(" smock, ", pos) == pos) {
			/* Remove the cloak? (yes, these are all the cases) */
			/* also take off cloak if suit or shirt is cursed */
			if (isCursed(ARMOR_CLOAK) || isCursed(ARMOR_SUIT) || isCursed(ARMOR_SHIRT))
				command = YES;
		} else if (messages.find(" suit, ", pos) == pos) {
			/* Remove the suit? */
			/* also take off suit if shirt is cursed */
			if (isCursed(ARMOR_SUIT) || isCursed(ARMOR_SHIRT))
				command = YES;
		} else if (messages.find(" boots, ", pos) == pos) {
			/* Remove the boots? */
			if (isCursed(ARMOR_BOOTS))
				command = YES;
		} else if (messages.find(" gloves, ", pos) == pos) {
			/* Remove the gloves? */
			if (isCursed(ARMOR_GLOVES))
				command = YES;
		} else if (messages.find(" shield, ", pos) == pos) {
			/* Remove the shield? */
			if (isCursed(ARMOR_SHIELD))
				command = YES;
		} else if (messages.find(" helmet, ", pos) == pos) {
			/* Remove the helmet? */
			if (isCursed(ARMOR_HELMET))
				command = YES;
		} else if (messages.find(" shirt, ", pos) == pos) {
			/* Remove the shirt? */
			if (isCursed(ARMOR_SHIRT))
				command = YES;
		}
		/* Request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (messages.find(MESSAGE_FOOCUBUS_REMOVE, 0) != string::npos) {
		/* A foocubus removed something without asking us. */
		/* Request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (messages.find(MESSAGE_YOU_FINISH_TAKING_OFF, 0) != string::npos) {
		/* took of last armor, mark inventory dirty */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (saiph->inventory_changed || wear_armor) {
		wearArmor();
	}
}

bool Armor::request(const Request &request) {
//	if (request.request == REQUEST_ARMOR_WEAR) {
//		/* player wish to wear this armor */
//		if (request.key >= ARMOR_SLOTS)
//			return false;
//		carry_amount[request.key] = request.value;
//		ArmorData ad;
//		ad.beatitude = request.beatitude;
//		ad.priority = request.priority;
//		ad.keep = request.sustain;
//		ad.name = request.data;
//		armor[request.key].push_back(ad);
//		/* tell the loot analyzer to pick up this armor too */
//		req.request = REQUEST_ITEM_PICKUP;
//		req.value = request.value;
//		req.beatitude = request.beatitude | BEATITUDE_UNKNOWN;
//		req.data = request.data;
//		saiph->request(req);
//		return true;
//	} else if (request.request == REQUEST_UPDATED_INVENTORY) {
//		/* need this due to foocubi stripping us */
//		planWear();
//		return true;
//	}
	return false;
}

/* private methods */
Item* Armor::wearingInSlot(int armor_slot) {
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional != "being worn")
			continue;
		if (ArmorData::armors.find(i->second.name) != ArmorData::armors.end() &&
				ArmorData::armors[i->second.name]->slot == armor_slot)
			return &(i->second);
		Debug::warning() << "Item named " << i->second.name << " being worn, but in unknown slot" << endl;
	}

	/* Couldn't find something on that slot? */
	return 0;
}

bool Armor::isCursed(int armor_slot) {
	Item *foo = wearingInSlot(armor_slot);
	return foo != 0 && foo->beatitude == CURSED;
}

void Armor::wearArmor() {
	bool slotCursed[ARMOR_SLOTS] = {false, false, false, false, false, false, false};
	for (int i = 0; i < ARMOR_SLOTS; i++) {
		Item *wearing = wearingInSlot(i);
		if (wearing != 0 && wearing->beatitude == CURSED)
			slotCursed[i] = true;
	}
	//cursed wielded weapon means no adjusting gloves
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i)
		if (i->second.additional.find("weapon in ", 0) == 0 || i->second.additional == "wielded")
			if (i->second.beatitude == CURSED) {
				slotCursed[ARMOR_GLOVES] = true;
				break;
			}
	if (slotCursed[ARMOR_CLOAK])
		slotCursed[ARMOR_SUIT] = true;
	if (slotCursed[ARMOR_SUIT])
		slotCursed[ARMOR_SHIRT] = true;
	vector<Item> armorChoices[ARMOR_SLOTS];
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		map<string, ArmorData *>::iterator armorIter = ArmorData::armors.find(i->second.name);
		if (armorIter == ArmorData::armors.end()) //it's not armor.
			continue;
		if (armorIter != ArmorData::armors.end() && i->second.beatitude == BEATITUDE_UNKNOWN) {
			req.request = REQUEST_BEATIFY_ITEMS;
			saiph->request(req);
		} else if (!slotCursed[armorIter->second->slot])
			armorChoices[armorIter->second->slot].push_back(i->second);
	}
	for (int i = 0; i < ARMOR_SLOTS; i++)
		if (slotCursed[i])
			armorChoices[i].push_back(*wearingInSlot(i));
	//choose zero or one element(s) from each vector in the armorChoices array
	//if we are wearing cursed armor in a slot, we must choose the cursed armor
	vector<vector<Item> > listOfCombos;
	vector<Item> curVec;
	makeArmorCombinations(armorChoices, slotCursed, 0, curVec, listOfCombos);
	int bestRank = INT_MIN;
	vector<Item> bestSet;
	for (int i = 0; i < listOfCombos.size(); i++) {
		int rank_ = rank(listOfCombos[i]);
		if (rank_ > bestRank) {
			bestSet = listOfCombos[i];
			bestRank = rank_;
		}
	}
	//now that we know what to wear, let's work out a "plan" to wear it
	unsigned char worn[ARMOR_SLOTS] = {0};
	unsigned char best_key[ARMOR_SLOTS] = {0};
	for (int i = 0; i < bestSet.size(); i++)
		for (map<unsigned char, Item>::iterator iter = saiph->inventory.begin(); iter != saiph->inventory.end(); iter++)
			if (iter->second == bestSet[i])
				best_key[ArmorData::armors[bestSet[i].name]->slot] = iter->first;
	for (int i = 0; i < ARMOR_SLOTS; i++) {
		Item *slot = wearingInSlot(i);
		if (slot != 0)
			for (map<unsigned char, Item>::iterator iter = saiph->inventory.begin(); iter != saiph->inventory.end(); iter++)
				if (iter->second == *slot)
					worn[i] = iter->first;
	}
	bool last_wear_armor = wear_armor;
	wear_armor = false;
	for (int s = 0; s < ARMOR_SLOTS; ++s) {
		if (worn[s] == best_key[s])
			continue; // wearing best armor or got no armor to wield
		if (isCursed(s))
			continue; // the item we're wearing in this slot it cursed and cannot be taken off
		if (!can_wear[s])
			continue; // we can't wear this in our current form
		if (s == ARMOR_SUIT || s == ARMOR_SHIRT) {
			/* wish to put on shirt or suit.
			 * if cloak or suit is cursed, this can't be done */
			if (isCursed(ARMOR_CLOAK) || (s == ARMOR_SHIRT && isCursed(ARMOR_SUIT))) {
				/* cloak is cursed, or suit is cursed and we're putting on a shirt */
				continue;
			}
			/* are we wearing a cloak? */
			if (worn[ARMOR_CLOAK] != 0) {
				/* yes, we must take it off first */
				command = TAKE_OFF;
				command2 = worn[ARMOR_CLOAK];
				wear_armor = true;
				break;
			}
			if (s == ARMOR_SHIRT) {
				/* are we wearing a suit? */
				if (worn[ARMOR_SUIT] != 0) {
					/* yes, we must take it off first */
					command = TAKE_OFF;
					command2 = worn[ARMOR_SUIT];
					wear_armor = true;
					break;
				}
			}
		} else if (s == ARMOR_GLOVES) {
			bool weapon_cursed = false;
			for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i)
				if (i->second.additional.find("weapon in ", 0) == 0 || i->second.additional == "wielded")
					if (i->second.beatitude == CURSED) {
						weapon_cursed = true;
						break;
					}

			/* weapon is cursed, we can't wear or take off gloves */
			if (weapon_cursed)
				continue;
		}
		if (worn[s] != 0) {
			/* we'll have to take this armor off first */
			command = TAKE_OFF;
			command2 = worn[s];
			wear_armor = true;
			break;
		}
		/* we should put on this piece of armor */
		command = WEAR;
		command2 = best_key[s];
		last_armor_type = s;
		wear_armor = true;
		break;
	}
	if (wear_armor)
		priority = PRIORITY_ARMOR_WEAR;
}

//choices is a pointer to the first element of an array of vector<Item>
//listOfCombos is intentionally mutable
void Armor::makeArmorCombinations(const vector<Item> choices[], bool slotCursed[], int curSlot, vector<Item> &curVec, vector<vector<Item> > &listOfCombos) {
	if (curSlot == ARMOR_SLOTS) {
		listOfCombos.push_back(curVec);
		return;
	}
	if (choices[curSlot].empty())
		makeArmorCombinations(choices, slotCursed, curSlot+1, curVec, listOfCombos);
	else if (slotCursed[curSlot]) { //we must wear the item in this slot
		curVec.push_back(choices[curSlot][0]);
		makeArmorCombinations(choices, slotCursed, curSlot+1, curVec, listOfCombos);
	} else { //we can choose to wear nothing or one of the possible items
		//wear nothing
		makeArmorCombinations(choices, slotCursed, curSlot+1, curVec, listOfCombos);
		//wear something; copy curVec to not mess up other recursive calls
		for (int i = 0; i < choices[curSlot].size(); i++) {
			curVec.push_back(choices[curSlot][i]);
			makeArmorCombinations(choices, slotCursed, curSlot+1, curVec, listOfCombos);
		}
	}
}

void Armor::resetCanWear() {
	for (int a = 0; a < ARMOR_SLOTS; ++a)
		can_wear[a] = true;
}

//return a value representing how desirable this armor is
#define MR_VALUE 100
#define REFLECTION_VALUE 100
#define VERYFAST_VALUE 50
#define AC_MULTIPLIER 10
#define MC_MULTIPLIER 5

int Armor::rank(const vector<Item> &items) {
	vector<ArmorData *> armor;
	for (int i = 0; i < items.size(); i++) {
		if (items[i].beatitude == 0)
			return -100; //we don't wear cursed armor.
		map<string, ArmorData *>::iterator iter = ArmorData::armors.find(items[i].name);
		if (iter != ArmorData::armors.end()) {
			armor.push_back(iter->second);
		} else
			Debug::warning() << "No ArmorData for item named \"" << items[i].name << "\"" << endl;
	}
	int value = 0;
	int maxMC = 0;
	int baseAC = 0;
	unsigned long long int properties = 0;
	for (int i = 0; i < armor.size(); i++) {
		baseAC += armor[i]->ac;
		if (armor[i]->mc > maxMC)
			maxMC = armor[i]->mc;
		properties |= armor[i]->properties;
	}
	if (properties & PROPERTY_MAGICRES)
		value += MR_VALUE;
	if (properties & PROPERTY_REFLECTION)
		value += REFLECTION_VALUE;
	if (properties & PROPERTY_VERYFAST)
		value += VERYFAST_VALUE;
	value += AC_MULTIPLIER*baseAC;
	value += MC_MULTIPLIER*maxMC;
	//value is cacheable here because no item-instance-specific things are included
	int enchantAC = 0; //also includes damage
	int fixedBonus = 0;
	int greasedBonus = 0;
	int blessedBonus = 0;
	for (int i = 0; i < items.size(); i++) {
		enchantAC += items[i].unknown_enchantment ? ARMOR_UNKNOWN_ENCHANTMENT_BONUS : items[i].enchantment;
		enchantAC -= items[i].damage;
		fixedBonus += items[i].fixed;
		blessedBonus += items[i].beatitude == BLESSED;
	}
	value += enchantAC*AC_MULTIPLIER;
	value += fixedBonus;
	value += greasedBonus;
	value += blessedBonus;
	return value;
}
