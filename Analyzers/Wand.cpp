#include "Wand.h"

/* constructors */
Wand::Wand(Saiph *saiph) : Analyzer("Wand"), saiph(saiph) {
}

/* methods */
void Wand::analyze() {
	if (saiph->on_ground != NULL) {
		/* if there's a wand here, pick it up */
		for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
			if (i->name.size() < 4 || (i->name.substr(0, 4) != "wand" && i->name.substr(i->name.size() - 4) != "wand"))
				continue; // not a wand
			/* request that someone loot this stash */
			req.request = REQUEST_LOOT_STASH;
			req.priority = WAND_PICKUP_PRIORITY;
			saiph->request(req);
			return;
		}
	}
}

void Wand::parseMessages(const string &messages) {
	if (saiph->pickup.size() > 0) {
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			if (p->second.name.size() < 4 || (p->second.name.substr(0, 4) != "wand" && p->second.name.substr(p->second.name.size() - 4) != "wand"))
				continue; // not a wand
			command = p->first;
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
	}
}
