#include "Amulet.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Amulet::Amulet(Saiph *saiph) : Analyzer("Amulet"), saiph(saiph) {
}

/* methods */
void Amulet::analyze() {
	if (saiph->inventory_changed)
		wearAmulet();
}

void Amulet::parseMessages(const string &messages) {
	if (!command2.empty() && messages.find(MESSAGE_YOU_WERE_WEARING, 0) != string::npos) {
                /* request dirty inventory */
		/* TODO: something else should look for the "you were wearing" message and trigger dirty inventory */
                req.request = REQUEST_DIRTY_INVENTORY;
                saiph->request(req);
	}
}

/* private methods */
void Amulet::wearAmulet() {
	/* find out which amulets we should wear */
	unsigned char amulet_worn = 0;
	bool amulet_cursed = false;
	unsigned char best_key = 0;
	int best_amulet = INT_MAX;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		for (vector<WearAmulet>::size_type w = 0; w < amulets.size(); ++w) {
			if (amulets[w].name != i->second.name || (int) w >= best_amulet) {
				continue;
			} else if ((amulets[w].beatitude & i->second.beatitude) == 0) {
				continue;
			} else if (i->second.additional.find("being worn", 0) == 0) {
				amulet_worn = i->first;
				if (i->second.beatitude == CURSED)
					amulet_cursed = true;
				continue;
			}
			best_key = i->first;
			best_amulet = w;
		}
	}
	if (best_key == 0 || amulet_cursed)
		return; // no amulet we like or we're wearing a cursed one
	wear_amulet = true;
	if (amulet_worn != 0) {
		/* must take off amulet we're wearing first */
		command = REMOVE;
		priority = PRIORITY_AMULET_WEAR;
		command2 = amulet_worn;
	} else {
		/* put on best amulet */
		command = PUT_ON;
		priority = PRIORITY_AMULET_WEAR;
		command2 = best_key;
	}
}
