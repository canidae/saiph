#include "Armor.h"

/* constructors */
Armor::Armor(Saiph *saiph) : Analyzer("Armor"), saiph(saiph) {
}

/* methods */
void Armor::parseMessages(const string &messages) {
	if (saiph->world->question && !command2.empty() && messages.find(ARMOR_WHAT_TO_WEAR, 0) != string::npos) {
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (command == WEAR) {
		/* in case we didn't get to wear the armor */
		priority = ARMOR_WEAR_PRIORITY;
	}
}

bool Armor::request(const Request &request) {
	if (request.request == REQUEST_ARMOR_WEAR) {
		/* player wish to wear this armor */
		if (request.value < 0 || request.value >= ARMOR_SLOTS)
			return false;
		WearArmor wa;
		wa.beatitude = request.status;
		wa.name = request.data;
		armor[request.value].push_back(wa);
		return true;
	} else if (request.request == REQUEST_UPDATED_INVENTORY) {
		/* check that we're (still) wearing our preferred armor */
		unsigned char worn[ARMOR_SLOTS];
		memset(worn, 0, sizeof (worn));
		unsigned char best_key[ARMOR_SLOTS];
		memset(best_key, 0, sizeof (best_key));
		int best_armor[ARMOR_SLOTS];
		memset(best_armor, 0, sizeof (best_armor));
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.additional == "being worn") {
				/* we're wearing this item, which slot does it belong to? */
				for (int s = 0; s < ARMOR_SLOTS; ++s) {
					for (vector<WearArmor>::size_type a = 0; a < armor[s].size(); ++a) {
						if (armor[s][a].name != i->second.name)
							continue;
						worn[s] = i->first;
						/* break loops */
						s = ARMOR_SLOTS;
						break;
					}
				}
			} else {
				/* we're not wearing this item, is it armor? */
				for (int s = 0; s < ARMOR_SLOTS; ++s) {
					for (vector<WearArmor>::size_type a = 0; a < armor[s].size(); ++a) {
						if (armor[s][a].name != i->second.name)
							continue;
						if (armor[s][a].name != i->second.name || (int) a >= best_armor[s])
							continue;
						best_key[s] = i->first;
						best_armor[s] = a;
					}
				}
			}
		}
		for (int s = 0; s < ARMOR_SLOTS; ++s) {
			if (worn[s] != 0 && worn[s] == best_armor[s])
				continue; // wearing best armor
			if (best_key[s] == 0)
				continue; // we have no armor for this slot
			/* we should put on this piece of armor */
			command = WEAR;
			command2 = best_key[s];
			priority = ARMOR_WEAR_PRIORITY;
			return true;
		}
	}
	return false;
}

/* private methods */
void Armor::wearArmor() {
	/* put on or change armor (which means we'll have to take something off) */
}
