#include "Armor.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Armor::Armor(Saiph *saiph) : Analyzer("Armor"), saiph(saiph) {
}

/* methods */
void Armor::analyze() {
	if (saiph->inventory_changed)
		wearArmor();
}

void Armor::parseMessages(const string &messages) {
	if (sequence == 0 && saiph->world->question && (messages.find(MESSAGE_WHAT_TO_WEAR, 0) != string::npos || messages.find(MESSAGE_WHAT_TO_TAKE_OFF, 0) != string::npos)) {
		/* wear or take off something */
		++sequence;
	} else if (sequence > 0) {
		/* sequence above 0 got priority PRIORITY_CONTINUE,
		 * this means that when the above checks fail,
		 * we're done putting on/taking off armor */
		sequence = -1;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
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
	}
	return false;
}

/* private methods */
void Armor::wearArmor() {
	/* put on or change armor (which means we'll have to take something off) */
	/* check that we're (still) wearing our preferred armor */
	unsigned char worn[ARMOR_SLOTS] = {0};
	unsigned char best_key[ARMOR_SLOTS] = {0};
	int best_armor[ARMOR_SLOTS];
	bool is_cursed[ARMOR_SLOTS];
	for (int s = 0; s < ARMOR_SLOTS; ++s) {
		best_armor[s] = INT_MAX;
		is_cursed[s] = false;
	}
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		for (int s = 0; s < ARMOR_SLOTS; ++s) {
			for (vector<WearArmor>::size_type a = 0; a < armor[s].size(); ++a) {
				if (armor[s][a].name != i->second.name)
					continue;
				if (i->second.additional == "being worn") {
					worn[s] = i->first;
					is_cursed[s] = (i->second.beatitude == CURSED);
				}
				if ((int) a >= best_armor[s])
					continue;
				if ((armor[s][a].beatitude & i->second.beatitude) == 0)
					continue;
				best_key[s] = i->first;
				best_armor[s] = a;
			}
		}
	}
	for (int s = 0; s < ARMOR_SLOTS; ++s) {
		if (best_key[s] == 0 || (worn[s] != 0 && saiph->inventory[worn[s]].name == saiph->inventory[best_key[s]].name))
			continue; // wearing best armor or got no armor to wield
		if (is_cursed[s])
			continue; // the item we're wearing in this slot it cursed and cannot be taken off
		if (s == ARMOR_SUIT || s == ARMOR_SHIRT) {
			/* wish to put on shirt or suit.
			 * if cloak or suit is cursed, this can't be done */
			if (is_cursed[ARMOR_CLOAK] || (s == ARMOR_SHIRT && is_cursed[ARMOR_SUIT])) {
				/* cloak is cursed, or suit is cursed and we're putting on a shirt */
				continue;
			}
			/* are we wearing a cloak? */
			if (worn[ARMOR_CLOAK] != 0) {
				/* yes, we must take it off first */
				setCommand(0, ARMOR_WEAR_PRIORITY, TAKE_OFF);
				setCommand(1, PRIORITY_CONTINUE_ACTION, string(worn[ARMOR_CLOAK], 1));
				sequence = 0;
				return;
			}
			if (s == ARMOR_SHIRT) {
				/* are we wearing a suit? */
				if (worn[ARMOR_SUIT] != 0) {
					/* yes, we must take it off first */
					setCommand(0, ARMOR_WEAR_PRIORITY, TAKE_OFF);
					setCommand(1, PRIORITY_CONTINUE_ACTION, string(worn[ARMOR_SUIT], 1));
					sequence = 0;
					return;
				}
			}
		}
		if (worn[s] != 0) {
			/* we'll have to take this armor off first */
			setCommand(0, ARMOR_WEAR_PRIORITY, TAKE_OFF);
			setCommand(1, PRIORITY_CONTINUE_ACTION, string(worn[s], 1));
			sequence = 0;
			return;
		}
		/* we should put on this piece of armor */
		setCommand(0, ARMOR_WEAR_PRIORITY, WEAR);
		setCommand(1, PRIORITY_CONTINUE_ACTION, string(best_key[s], 1));
		sequence = 0;
		return;
	}
}
