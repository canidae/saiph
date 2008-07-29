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

	/* corpses */
	inedible_corpses["abbot corpse"] = true;
	inedible_corpses["abbot corpses"] = true;
	inedible_corpses["bat corpse"] = true;
	inedible_corpses["bat corpses"] = true;
	inedible_corpses["chameleon corpse"] = true;
	inedible_corpses["chameleon corpses"] = true;
	inedible_corpses["chickatrice corpse"] = true;
	inedible_corpses["chickatrice corpses"] = true;
	inedible_corpses["cockatrice corpse"] = true;
	inedible_corpses["cockatrice corpses"] = true;
	inedible_corpses["Death's corpse"] = true;
	inedible_corpses["dog corpse"] = true;
	inedible_corpses["dog corpses"] = true;
	inedible_corpses["doppelganger corpse"] = true;
	inedible_corpses["doppelganger corpses"] = true;
	inedible_corpses["Famine's corpse"] = true;
	inedible_corpses["giant bat corpse"] = true;
	inedible_corpses["giant bat corpses"] = true;
	inedible_corpses["giant mimic corpse"] = true;
	inedible_corpses["giant mimic corpses"] = true;
	inedible_corpses["green slime corpse"] = true;
	inedible_corpses["green slime corpses"] = true;
	inedible_corpses["housecat corpse"] = true;
	inedible_corpses["housecat corpses"] = true;
	inedible_corpses["kitten corpse"] = true;
	inedible_corpses["kitten corpses"] = true;
	inedible_corpses["large cat corpse"] = true;
	inedible_corpses["large cat corpses"] = true;
	inedible_corpses["large dog corpse"] = true;
	inedible_corpses["large dog corpses"] = true;
	inedible_corpses["large mimic corpse"] = true;
	inedible_corpses["large mimic corpses"] = true;
	inedible_corpses["lichen corpse"] = true;
	inedible_corpses["lichen corpses"] = true;
	inedible_corpses["lizard corpse"] = true;
	inedible_corpses["lizard corpses"] = true;
	inedible_corpses["little dog corpse"] = true;
	inedible_corpses["little dog corpses"] = true;
	inedible_corpses["Medusa's corpse"] = true;
	inedible_corpses["Pestilence's corpse"] = true;
	inedible_corpses["small mimic corpse"] = true;
	inedible_corpses["small mimic corpses"] = true;
	inedible_corpses["werejackal corpse"] = true;
	inedible_corpses["werejackal corpses"] = true;
	inedible_corpses["wererat corpse"] = true;
	inedible_corpses["wererat corpses"] = true;
	inedible_corpses["werewolf corpse"] = true;
	inedible_corpses["werewolf corpses"] = true;
}

/* methods */
void Food::finish() {
	/* update safe_monster with seen monsters */
	safe_monster.clear();
	for (map<Point, Monster>::iterator m = saiph->monsters[saiph->position.branch][saiph->position.level].begin(); m != saiph->monsters[saiph->position.branch][saiph->position.level].end(); ++m) {
		if (m->second.symbol == 'Z' || m->second.symbol == 'M' || m->second.symbol == 'V')
			continue; // these leave tainted corpses
		map<Point, Stash>::iterator s = saiph->stashes[saiph->position.branch][saiph->position.level].find(m->first);
		if (s != saiph->stashes[saiph->position.branch][saiph->position.level].end())
			continue; // there's a stash here, might be an old corpse, don't gamble
		safe_monster[m->first] = true;
	}
	/* are we hungry? */
	if (saiph->world->player.hunger <= WEAK) {
		/* yes, we are */
		/* make sure inventory is updated */
		req.request = REQUEST_UPDATED_INVENTORY;
		req.priority = PRIORITY_LOOK;
		if (!saiph->request(req))
			return; // hmm, we can't say if inventory is updated?
		for (vector<string>::iterator f = eat_order.begin(); f != eat_order.end(); ++f) {
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
		if (saiph->world->player.hunger <= WEAK) {
			/* bad enough to pray for help.
			 * if this doesn't work... help! */
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
				command = EAT;
				command2 = i->first;
				priority = FOOD_EAT_HUNGRY_PRIORITY;
				return;
			}
		}
	}
	if (saiph->on_ground != NULL && saiph->world->player.hunger < SATIATED) {
		map<Point, int>::iterator s = safe_to_eat.find(saiph->position);
		if (s != safe_to_eat.end() && s->second + FOOD_CORPSE_EAT_TIME > saiph->world->player.turn) {
			/* it's safe to eat corpses here */
			for (map<Point, Monster>::iterator m = saiph->monsters[saiph->position.branch][saiph->position.level].begin(); m != saiph->monsters[saiph->position.branch][saiph->position.level].end(); ++m) {
				if (m->second.symbol == '@' && m->second.color == WHITE && m->second.visible)
					return; // we see a white '@', don't eat (nor loot)
			}
			/* there are items here, we should look for corpses to eat */
			for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
				string::size_type pos;
				if (((pos = i->name.find(FOOD_CORPSES)) != string::npos && pos == i->name.size() - sizeof (FOOD_CORPSES) + 1) || ((pos = i->name.find(FOOD_CORPSE)) != string::npos && pos == i->name.size() - sizeof (FOOD_CORPSE) + 1)) {
					/* there's a corpse in the stash, is it edible? */
					if (inedible_corpses.find(i->name) == inedible_corpses.end()) {
						/* it is, and we know we can eat corpses on this position */
						command = EAT;
						command2 = i->name;
						priority = FOOD_EAT_HUNGRY_PRIORITY;
						return;
					}
				}
			}
		}
	}
	if (saiph->on_ground != NULL) {
		/* there are items here, we should look for food to pick up */
		req.request = REQUEST_LOOT_STASH;
		req.priority = FOOD_PICKUP_PRIORITY;
		req.coordinate = saiph->position;
		for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
			for (vector<string>::iterator f = eat_order.begin(); f != eat_order.end(); ++f) {
				if (i->name == *f) {
					/* wooo, food!
					 * request that someone loot this stash */
					saiph->request(req);
					return;
				}
			}
		}
	}
}

void Food::parseMessages(const string &messages) {
	string::size_type pos;
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
			for (vector<string>::iterator f = eat_order.begin(); f != eat_order.end(); ++f) {
				if (p->second.name == *f) {
					/* we should pick up this */
					command = p->first;
					priority = PRIORITY_PICKUP_ITEM;
					continue;
				}
			}
		}
		return;
	} else if ((pos = messages.find(FOOD_EAT_IT_2, 0)) != string::npos || (pos = messages.find(FOOD_EAT_ONE_2, 0)) != string::npos) {
		/* asks if we should eat the stuff on the floor */
		priority = PRIORITY_CONTINUE_ACTION;
		map<Point, int>::iterator s = safe_to_eat.find(saiph->position);
		if (s == safe_to_eat.end() || s->second + FOOD_CORPSE_EAT_TIME <= saiph->world->player.turn) {
			/* this corpse is rotten */
			command = NO;
			return;
		}
		string::size_type pos2 = pos;
		pos = messages.find(FOOD_EAT_IT_1, 0);
		if (pos == string::npos) {
			pos = messages.find(FOOD_EAT_ONE_1, 0);
			if (pos == string::npos) {
				/* this shouldn't happen */
				command = NO;
				return;
			} else {
				pos += sizeof (FOOD_EAT_ONE_1) - 1;
			}
		} else {
			pos += sizeof (FOOD_EAT_IT_1) - 1;
		}
		Item item(messages.substr(pos, pos2 - pos));
		if (command2 == item.name) {
			command = YES;
			/* since we already got the "what do you want to eat?" prompt,
			 * it's safe to remove it from stash here.
			 * the corpse will be "partially eaten" if we're interrupted */
			saiph->removeItemFromStash(saiph->position, item);
		} else {
			command = NO;
		}
		return;
	} else if ((pos = messages.find(FOOD_YOU_KILL, 0)) != string::npos) {
		/* we killed a monster.
		 * look for a spot there used to be a "safe_monster",
		 * but now is a stash instead */
		string::size_type pos2 = messages.find("!  ", pos);
		if (pos2 != string::npos) {
			for (map<Point, bool>::iterator s = safe_monster.begin(); s != safe_monster.end(); ++s) {
				if (saiph->stashes[saiph->position.branch][saiph->position.level].find(s->first) != saiph->stashes[saiph->position.branch][saiph->position.level].end()) {
					/* there's a stash where we last saw the monster.
					 * since it's a "safe_monster", we can eat any corpse here.
					 * we've already checked that it wasn't a stash here before the monster went there */
					safe_to_eat[s->first] = saiph->world->player.turn;
					return;
				}
			}
		}
	} else if ((pos = messages.find(FOOD_IS_KILLED, 0)) != string::npos) {
		/* we saw a monster die.
		 * look for a spot there used to be a "safe_monster",
		 * but now is a stash instead */
		string::size_type pos2 = pos;
		pos = messages.rfind("  The ", pos2);
		if (pos != string::npos) {
			for (map<Point, bool>::iterator s = safe_monster.begin(); s != safe_monster.end(); ++s) {
				if (saiph->stashes[saiph->position.branch][saiph->position.level].find(s->first) != saiph->stashes[saiph->position.branch][saiph->position.level].end()) {
					/* there's a stash where we last saw the monster.
					 * since it's a "safe_monster", we can eat any corpse here.
					 * we've already checked that it wasn't a stash here before the monster went there */
					safe_to_eat[s->first] = saiph->world->player.turn;
					return;
				}
			}
		}
	}
}
