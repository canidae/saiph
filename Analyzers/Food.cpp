#include "Food.h"

/* constructors */
Food::Food(Saiph *saiph) : Analyzer("Food"), saiph(saiph) {
	command2 = "";
	priority = ILLEGAL_PRIORITY;
	//eat_order.push_back("tin");
	eat_order.push_back("meatball");
	eat_order.push_back("meatballs");
	eat_order.push_back("meat ring");
	eat_order.push_back("meat stick");
	eat_order.push_back("meat sticks");
	eat_order.push_back("partly eaten huge chunk of meat");
	eat_order.push_back("partly eaten huge chunks of meat");
	eat_order.push_back("huge chunk of meat");
	eat_order.push_back("huge chunks of meat");
	eat_order.push_back("cream pie");
	eat_order.push_back("cream pies");
	eat_order.push_back("partly eaten lichen corpse");
	eat_order.push_back("partly eaten lichen corpses");
	eat_order.push_back("lichen corpse");
	eat_order.push_back("lichen corpses");
	eat_order.push_back("partly eaten tripe ration");
	eat_order.push_back("partly eaten tripe rations");
	eat_order.push_back("tripe ration");
	eat_order.push_back("tripe rations");
	eat_order.push_back("melon");
	eat_order.push_back("melons");
	eat_order.push_back("apple");
	eat_order.push_back("apples");
	eat_order.push_back("pear");
	eat_order.push_back("pears");
	eat_order.push_back("kelp frond");
	eat_order.push_back("kelp fronds");
	eat_order.push_back("C-ration");
	eat_order.push_back("C-rations");
	eat_order.push_back("partly eaten cram ration");
	eat_order.push_back("partly eaten cram rations");
	eat_order.push_back("cram ration");
	eat_order.push_back("cram rations");
	eat_order.push_back("partly eaten food ration");
	eat_order.push_back("partly eaten food rations");
	eat_order.push_back("food ration");
	eat_order.push_back("food rations");
	eat_order.push_back("partly eaten gunyoki");
	eat_order.push_back("gunyoki");
	eat_order.push_back("K-ration");
	eat_order.push_back("K-rations");
	eat_order.push_back("clove of garlic");
	eat_order.push_back("cloves of garlic");
	eat_order.push_back("fortune cookie");
	eat_order.push_back("fortune cookies");
	eat_order.push_back("banana");
	eat_order.push_back("bananas");
	eat_order.push_back("orange");
	eat_order.push_back("oranges");
	eat_order.push_back("candy bar");
	eat_order.push_back("candy bars");
	eat_order.push_back("byte");
	eat_order.push_back("bytes");
	//eat_order.push_back("egg"); // just makes her sick
	//eat_order.push_back("eggs"); // just makes her sick
	eat_order.push_back("lump of royal jelly");
	eat_order.push_back("lumps of royal jelly");
	eat_order.push_back("partly eaten pancake");
	eat_order.push_back("partly eaten pancakes");
	eat_order.push_back("pancake");
	eat_order.push_back("pancakes");
	eat_order.push_back("partly eaten lembas wafer");
	eat_order.push_back("partly eaten lembas wafers");
	eat_order.push_back("lembas wafer");
	eat_order.push_back("lembas wafers");
	eat_order.push_back("carrot"); // will cure blindness
	eat_order.push_back("carrots"); // will cure blindness
	eat_order.push_back("eucalyptus leaf"); // will cure sickness
	eat_order.push_back("eucalyptus leaves"); // will cure sickness
	eat_order.push_back("sprig of wolfsbane"); // will cure lycanthropy
	eat_order.push_back("sprigs of wolfsbane"); // will cure lycanthropy
	eat_order.push_back("partly eaten lizard corpse"); // wil cure stoning?
	eat_order.push_back("partly eaten lizard corpses"); // wil cure stoning?
	eat_order.push_back("lizard corpse"); // wil cure stoning
	eat_order.push_back("lizard corpses"); // wil cure stoning
}

/* methods */
void Food::complete() {
}

void Food::finish() {
	/* are we hungry? */
	if (saiph->world->player.hunger <= HUNGRY) {
		/* yes, we are */
		/* make sure inventory is updated */
		req.request = REQUEST_UPDATED_INVENTORY;
		req.priority = PRIORITY_LOOK;
		if (!saiph->request(req))
			return; // hmm, we can't say if inventory is updated?
		for (list<string>::iterator f = eat_order.begin(); f != eat_order.end(); ++f) {
			for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
				if (i->second.name == *f) {
					/* and we got something to eat */
					command = EAT;
					command2 = i->first;
					switch (saiph->world->player.hunger) {
						case HUNGRY:
							priority = FOOD_EAT_HUNGRY_PRIORITY;
							break;

						case WEAK:
							priority = FOOD_EAT_WEAK_PRIORITY;
							break;

						default:
							priority = FOOD_EAT_FAINTING_PRIORITY;
							break;
					}
					return;
				}
			}
		}
		/* hmm, nothing to eat, how bad is it? */
		if (saiph->world->player.hunger <= FAINTING) {
			/* quite, quite bad.
			 * pray for food, but if this doesn't work... help! */
			req.request = REQUEST_PRAY;
			req.priority = FOOD_PRAY_FOR_FOOD;
			saiph->request(req);
		}
	} else if (saiph->world->player.hunger > CONTENT) {
		/* make sure inventory is updated */
		req.request = REQUEST_UPDATED_INVENTORY;
		req.priority = PRIORITY_LOOK;
		if (!saiph->request(req))
			return; // hmm, we can't say if inventory is updated?
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name == "byte" || i->second.name == "bytes") {
				/* easter egg: eat bytes when [over]satiated */
				command2 = i->first;
				command = EAT;
				priority = FOOD_EAT_HUNGRY_PRIORITY;
				return;
			}
		}
	} else if (saiph->on_ground != NULL) {
		/* there are items here, we should look for food */
		req.request = REQUEST_LOOT_STASH;
		req.priority = FOOD_PICKUP_PRIORITY;
		req.coordinate = saiph->position;
		bool die = false;
		for (list<Item>::iterator i = saiph->on_ground->items.begin(); !die && i != saiph->on_ground->items.end(); ++i) {
			for (list<string>::iterator f = eat_order.begin(); f != eat_order.end(); ++f) {
				if (i->name == *f) {
					/* wooo, foood!
					 * request that someone loot this stash */
					saiph->request(req);
					/* and break loops */
					die = true;
					break;
				}
			}
		}
	}
}

void Food::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_EAT, 0) != string::npos) {
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		/* food gone, make inventory dirty */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		return;
	} else if (saiph->world->menu && messages.find(MESSAGE_PICK_UP_WHAT, 0) != string::npos) {
		/* select what to pick up */
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			for (list<string>::iterator f = eat_order.begin(); f != eat_order.end(); ++f) {
				if (p->second.name.find(*f, 0) != string::npos) {
					/* we should pick up this */
					command = p->first;
					priority = PRIORITY_PICKUP_ITEM;
					continue;
				}
			}
		}
		return;
	} else if (messages.find(MESSAGE_EAT_IT, 0) != string::npos) {
		/* we don't eat items on the floor yet */
		command = NO;
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	}
}
