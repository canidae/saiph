#include "Amulet.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Amulet::Amulet(Saiph *saiph) : Analyzer("Amulet"), saiph(saiph), command2(""), wear_amulet(false), sequence(-1) {
}

/* methods */
void Amulet::analyze() {
	sequence = -1;
	if (saiph->inventory_changed || wear_amulet)
		wearAmulet();
}

void Amulet::complete() {
	sequence = 1;
}

void Amulet::parseMessages(const string &messages) {
	if (sequence == 1 && saiph->world->question && (messages.find(MESSAGE_WHAT_TO_PUT_ON, 0) != string::npos || messages.find(MESSAGE_WHAT_TO_REMOVE, 0) != string::npos)) {
		/* put on or remove a amulet */
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		wear_amulet = false;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	}
}

bool Amulet::request(const Request &request) {
	if (request.request == REQUEST_AMULET_WEAR) {
		/* player wish to wear this amulet */
		WearAmulet wr;
		wr.beatitude = request.status;
		wr.name = request.data;
		amulets.push_back(wr);
		return true;
	}
	return false;
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
		priority = AMULET_WEAR_PRIORITY;
		command2 = amulet_worn;
	} else {
		/* put on best amulet */
		command = PUT_ON;
		priority = AMULET_WEAR_PRIORITY;
		command2 = best_key;
	}
}
