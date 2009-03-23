#include "Armor.h"
#include "../Item.h"
#include "../Player.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Data/ArmorData.h"

using namespace std;

map<unsigned long long int, int> Armor::scores;

/* constructors/destructor */
Armor::Armor(Saiph *saiph) : Analyzer("Armor"), saiph(saiph), wear_armor(false), last_armor_type(0), last_polymorphed(false) {
	resetCanWear();
	scores[PROPERTY_MAGICRES] = 100;
	scores[PROPERTY_REFLECTION] = 95;
	scores[PROPERTY_VERYFAST] = 50; //speed is good
	scores[PROPERTY_POISON] = 40;
	scores[PROPERTY_FIRE] = 30;
	scores[PROPERTY_COLD] = 30;
	scores[PROPERTY_SLEEP] = 30;
	scores[PROPERTY_SHOCK] = 15;
	scores[PROPERTY_ACID] = 10;
	scores[PROPERTY_DISINT] = 5;
	scores[PROPERTY_INVISIBLE] = 15;
	scores[PROPERTY_DISPLACEMENT] = 10;
	scores[PROPERTY_SLIPPERY] = 5;
	scores[PROPERTY_STRENGTH] = 30;
	scores[PROPERTY_STEALTH] = 15;
	scores[PROPERTY_FUMBLING] = -100;
	scores[PROPERTY_STUPIDITY] = -100;
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
	if (request.request == REQUEST_ARMOR_WEAR) {
		/* player wish to wear this armor */
		if (request.key >= ARMOR_SLOTS)
			return false;
		carry_amount[request.key] = request.value;
		OldArmorData ad;
		ad.beatitude = request.beatitude;
		ad.priority = request.priority;
		ad.keep = request.sustain;
		ad.name = request.data;
		armor[request.key].push_back(ad);
		/* tell the loot analyzer to pick up this armor too */
		req.request = REQUEST_ITEM_PICKUP;
		req.value = request.value;
		req.beatitude = request.beatitude | BEATITUDE_UNKNOWN;
		req.data = request.data;
		saiph->request(req);
		return true;
	} else if (request.request == REQUEST_UPDATED_INVENTORY) {
		/* need this due to foocubi stripping us */
		wearArmor();
		return true;
	}
	return false;
}

/* private methods */
bool Armor::isCursed(int armor_slot) {
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional != "being worn")
			continue;
		for (vector<OldArmorData>::size_type a = 0; a < armor[armor_slot].size(); ++a) {
			if (armor[armor_slot][a].name != i->second.name)
				continue;
			return i->second.beatitude == CURSED;
		}
	}

	/* Couldn't find something on that slot? */
	return false;
}

void Armor::wearArmor() {
	/* put on or change armor (which means we'll have to take something off) */
	/* check that we're (still) wearing our preferred armor */
	unsigned char worn[ARMOR_SLOTS] = {0, 0, 0, 0, 0, 0, 0};
	unsigned char best_key[ARMOR_SLOTS] = {0, 0, 0, 0, 0, 0, 0};
	int best_armor[ARMOR_SLOTS];
	for (int s = 0; s < ARMOR_SLOTS; ++s)
		best_armor[s] = INT_MIN;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		for (int s = 0; s < ARMOR_SLOTS; ++s) {
			for (vector<OldArmorData>::iterator a = armor[s].begin(); a != armor[s].end(); ++a) {
				if (a->name != i->second.name)
					continue;
				else if (i->second.additional == "being worn")
					worn[s] = i->first;
				if (i->second.beatitude == BEATITUDE_UNKNOWN) {
					/* armor with unknown beatitude, request it beatified */
					req.request = REQUEST_BEATIFY_ITEMS;
					saiph->request(req);
				}
				if ((a->beatitude & i->second.beatitude) == 0)
					continue;
			}
		}
	}

	bool cursed[ARMOR_SLOTS];
	for (int i = 0; i < ARMOR_SLOTS; i++)
		cursed[i] = isCursed(i);
	vector< vector<Item> > choices(ARMOR_SLOTS);
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		map<string, ArmorData *>::iterator a = ArmorData::armors.find(i->second.name);
		//we don't wear cursed armor, but if we're wearing something cursed we need to add it anyway
		if (a != ArmorData::armors.end() && ((cursed[a->second->slot] && i->second.beatitude == CURSED) ||
				(!cursed[a->second->slot] && !(i->second.beatitude == CURSED || i->second.beatitude == BEATITUDE_UNKNOWN))))
			choices[a->second->slot].push_back(i->second);
	}
	//represent leaving a slot empty (sometimes this is better)
	for (int i = 0; i < ARMOR_SLOTS; i++)
		if (!cursed[i]) //we have to wear this if it's cursed
			choices[i].push_back(Item());

	ArmorSet cur_set;
	for (int i = 0; i < ARMOR_SLOTS; i++)
		if (worn[i] != 0)
			cur_set[i] = saiph->inventory[worn[i]];
	Debug::notice() << "Armor] " << "currently wearing " << cur_set << endl;

	ArmorSet best_set = cur_set;
	int best_score = rank(best_set);
	for (vector<Item>::size_type a = 0; a < choices[ARMOR_SHIRT].size(); a++)
		for (vector<Item>::size_type b = 0; b < choices[ARMOR_SUIT].size(); b++)
			for (vector<Item>::size_type c = 0; c < choices[ARMOR_CLOAK].size(); c++)
				for (vector<Item>::size_type d = 0; d < choices[ARMOR_BOOTS].size(); d++)
					for (vector<Item>::size_type e = 0; e < choices[ARMOR_GLOVES].size(); e++)
						for (vector<Item>::size_type f = 0; f < choices[ARMOR_HELMET].size(); f++)
							for (vector<Item>::size_type g = 0; g < choices[ARMOR_SHIELD].size(); g++) {
								ArmorSet as = {
									choices[ARMOR_SHIRT][a],
									choices[ARMOR_SUIT][b],
									choices[ARMOR_CLOAK][c],
									choices[ARMOR_BOOTS][d],
									choices[ARMOR_GLOVES][e],
									choices[ARMOR_HELMET][f],
									choices[ARMOR_SHIELD][g],
								};
								int score = rank(as);
								if (score > best_score) {
									best_set = as;
									best_score = score;
								}
							}

	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); i++)
		for (int j = 0; j < ARMOR_SLOTS; j++)
			if (i->second == best_set[j])
				best_key[j] = i->first;

	bool last_wear_armor = wear_armor;
	wear_armor = false;
	for (int s = 0; s < ARMOR_SLOTS; ++s) {
		if (worn[s] == best_key[s])
			continue; // wearing best armor or got no armor to wield
		Debug::notice() << "Armor] Going to wear armor in slot " << s << endl;
		Debug::notice() << "Armor] best: " << best_key[s] << " cur: " << worn[s] << endl;
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
	if (wear_armor) {
		priority = PRIORITY_ARMOR_WEAR;
		Debug::notice() << "Armor]" << "Going to wear " << best_set << " with score"
				<< best_score << endl;
	} else {
		/* nothing to wear */
		command.clear();
		if (!last_wear_armor)
			return;
		/* tell Loot what armor we [still] want */
		req.request = REQUEST_ITEM_PICKUP;
		for (int s = 0; s < ARMOR_SLOTS; ++s) {
			map<unsigned char, Item>::iterator i = saiph->inventory.find(worn[s]);
			for (vector<OldArmorData>::iterator a = armor[s].begin(); a != armor[s].end(); ++a) {
				if (a->keep || a->priority + ARMOR_UNKNOWN_ENCHANTMENT_BONUS > best_armor[s]) {
					/* we [still] want this armor */
					req.value = carry_amount[s];
					if (i != saiph->inventory.end() && a->name == i->second.name) {
						/* allow carrying 1 more of what we're wearing,
						 * but don't carry things we know enchantment of */
						req.value++;
						req.only_unknown_enchantment = true;
					} else {
						/* only carry armor which may be better than what we got */
						req.only_unknown_enchantment = (a->priority <= best_armor[s]);
					}
				} else {
					/* we don't want to keep this armor as it'll never
					 * be better than the armor we currently got */
					req.value = 0;
					req.only_unknown_enchantment = false;
				}
				req.beatitude = a->beatitude | BEATITUDE_UNKNOWN;
				req.data = a->name;
				saiph->request(req);
			}
		}
	}
}

void Armor::resetCanWear() {
	for (int a = 0; a < ARMOR_SLOTS; ++a)
		can_wear[a] = true;
}

#define AC_MULTIPLIER 10
#define MC_MULTIPLIER 5
#define BLESSED_BONUS 3
#define FIXED_BONUS 2
#define GREASED_BONUS 2
#define HARD_HAT_BONUS 2 /* good vs. falling rocks */
#define ANY_ARMOR_BONUS 1 /* better than nothing? */
#define BEING_WORN_BONUS 1 /* a hack to prevent oscillations */
#define WEIGHT_LIMIT 150
#define WEIGHT_PENALTY 50

int Armor::rank(const ArmorSet &armor) {
	ArmorData *nulldata = 0;  //vector's constructor didn't like const 0 for some reason
	vector<ArmorData *> data(ARMOR_SLOTS, nulldata);
	for (int i = 0; i < ARMOR_SLOTS; i++)
		if (armor[i] != Item()) {
			map<string, ArmorData *>::iterator iter = ArmorData::armors.find(armor[i].name);
			if (iter != ArmorData::armors.end())
				data[i] = iter->second;
		}
	unsigned long long armor_properties = 0ULL;
	for (int i = 0; i < ARMOR_SLOTS; i++)
		if (data[i] != 0)
			armor_properties |= data[i]->properties;
	//don't consider the things we have intrinsically, unless the extrinsic is different
	armor_properties &= (~saiph->world->player.intrinsics | PROPERTY_ESP);
	int property_score = 0;
	for (map<unsigned long long int, int>::iterator i = scores.begin(); i != scores.end(); i++)
		if (armor_properties & i->first)
			property_score += i->second;
	int total_ac = 0, max_mc = 0, misc_bonuses = 0;
	for (int i = 0; i < ARMOR_SLOTS; i++) {
		if (data[i] == 0)
			continue;
		total_ac += data[i]->ac;
		total_ac -= armor[i].damage > data[i]->ac ? data[i]->ac : armor[i].damage;
		total_ac += armor[i].unknown_enchantment ? ARMOR_UNKNOWN_ENCHANTMENT_BONUS : armor[i].enchantment;
		max_mc = max_mc > data[i]->mc ? max_mc : data[i]->mc;
		if (armor[i].beatitude == BLESSED)
			misc_bonuses += BLESSED_BONUS;
		if (armor[i].fixed || (data[i]->material & ~MATERIALS_THAT_ERODE))
			misc_bonuses += FIXED_BONUS;
		if (armor[i].greased)
			misc_bonuses += GREASED_BONUS;
		if (armor[i].additional == "being worn")
			misc_bonuses += BEING_WORN_BONUS;
		if (data[i]->weight > WEIGHT_LIMIT)
			misc_bonuses -= WEIGHT_PENALTY;
		misc_bonuses += ANY_ARMOR_BONUS;
	}
	if (data[ARMOR_HELMET] != 0 && data[ARMOR_HELMET]->material & MATERIALS_METALLIC)
		misc_bonuses += HARD_HAT_BONUS;
	return property_score + total_ac * AC_MULTIPLIER + max_mc * MC_MULTIPLIER + misc_bonuses;
}
