#include "Food.h"

/* constructors */
Food::Food(Saiph *saiph) : Analyzer("Food"), saiph(saiph) {
	command2 = "";
	priority = ILLEGAL_PRIORITY;
	//eat_order.push_back("tin");
	eat_order.push_back("meatball");
	eat_order.push_back("meat ring");
	eat_order.push_back("meat stick");
	eat_order.push_back("partly eaten huge chunk of meat");
	eat_order.push_back("huge chunk of meat");
	eat_order.push_back("cream pie");
	eat_order.push_back("partly eaten lichen corpse");
	eat_order.push_back("lichen corpse");
	eat_order.push_back("partly eaten tripe ration");
	eat_order.push_back("tripe ration");
	eat_order.push_back("melon");
	eat_order.push_back("apple");
	eat_order.push_back("pear");
	eat_order.push_back("kelp frond");
	eat_order.push_back("C-ration");
	eat_order.push_back("partly eaten cram ration");
	eat_order.push_back("cram ration");
	eat_order.push_back("partly eaten food ration");
	eat_order.push_back("food ration");
	eat_order.push_back("partly eaten gunyoki");
	eat_order.push_back("gunyoki");
	eat_order.push_back("K-ration");
	eat_order.push_back("clove of garlic");
	eat_order.push_back("fortune cookie");
	eat_order.push_back("banana");
	eat_order.push_back("orange");
	eat_order.push_back("candy bar");
	eat_order.push_back("byte");
	eat_order.push_back("lump of royal jelly");
	eat_order.push_back("partly eaten pancake");
	eat_order.push_back("pancake");
	eat_order.push_back("partly eaten lembas wafer");
	eat_order.push_back("lembas wafer");
	eat_order.push_back("carrot"); // will cure blindness
	eat_order.push_back("eucalyptus leaf"); // will cure sickness
	eat_order.push_back("sprig of wolfsbane"); // will cure lycanthropy
	eat_order.push_back("partly eaten lizard corpse"); // wil cure stoning?
	eat_order.push_back("lizard corpse"); // wil cure stoning

	/* corpses */
	corpse_data["abbot corpse"] = FOOD_HUMAN | FOOD_HALLUCINOGENIC;
	corpse_data["acid blob corpse"] = FOOD_ACIDIC | FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["acolyte corpse"] = FOOD_HUMAN;
	corpse_data["aligned priest corpse"] = FOOD_HUMAN;
	corpse_data["apprentice corpse"] = FOOD_HUMAN;
	corpse_data["Arch Priest's corpse"] = FOOD_HUMAN;
	corpse_data["archeologist corpse"] = FOOD_HUMAN;
	corpse_data["attendant corpse"] = FOOD_HUMAN;
	corpse_data["baby green dragon corpse"] = FOOD_POISONOUS;
	corpse_data["baby yellow dragon corpse"] = FOOD_ACIDIC;
	corpse_data["barbarian corpse"] = FOOD_HUMAN;
	corpse_data["bat corpse"] = FOOD_STUN;
	corpse_data["black naga corpse"] = FOOD_ACIDIC;
	corpse_data["black naga hatchling corpse"] = FOOD_ACIDIC;
	corpse_data["black pudding corpse"] = FOOD_ACIDIC;
	corpse_data["blue jelly corpse"] = FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["brown mold corpse"] = FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["brown pudding corpse"] = FOOD_ACIDIC | FOOD_VEGETARIAN;
	corpse_data["captain corpse"] = FOOD_HUMAN;
	corpse_data["caveman corpse"] = FOOD_HUMAN;
	corpse_data["cavewoman corpse"] = FOOD_HUMAN;
	corpse_data["chameleon corpse"] = FOOD_POLYMORPH;
	corpse_data["chickatrice corpse"] = FOOD_PETRIFY;
	corpse_data["chieftain corpse"] = FOOD_HUMAN;
	corpse_data["Chromatic Dragon's corpse"] = FOOD_POISONOUS;
	corpse_data["cobra corpse"] = FOOD_POISONOUS;
	corpse_data["cockatrice corpse"] = FOOD_PETRIFY;
	corpse_data["Croesus' corpse"] = FOOD_HUMAN;
	corpse_data["Death's corpse"] = FOOD_DIE;
	corpse_data["dog corpse"] = FOOD_AGGRAVATE;
	corpse_data["doppelganger corpse"] = FOOD_HUMAN | FOOD_POLYMORPH;
	corpse_data["dwarf corpse"] = FOOD_DWARF;
	corpse_data["dwarf king corpse"] = FOOD_DWARF;
	corpse_data["dwarf lord corpse"] = FOOD_DWARF;
	corpse_data["elf corpse"] = FOOD_ELF;
	corpse_data["elf-lord corpse"] = FOOD_ELF;
	corpse_data["Elvenking corpse"] = FOOD_ELF;
	corpse_data["Famine's corpse"] = FOOD_DIE;
	corpse_data["gelatinous corpse"] = FOOD_ACIDIC | FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["giant bat corpse"] = FOOD_STUN;
	corpse_data["giant beetle corpse"] = FOOD_POISONOUS;
	corpse_data["giant mimic corpse"] = FOOD_MIMIC;
	corpse_data["giant spider corpse"] = FOOD_POISONOUS;
	corpse_data["gnome corpse"] = FOOD_GNOME;
	corpse_data["gnome king corpse"] = FOOD_GNOME;
	corpse_data["gnome lord corpse"] = FOOD_GNOME;
	corpse_data["gnomish wizard corpse"] = FOOD_GNOME;
	corpse_data["Grand Master's corpse"] = FOOD_HUMAN;
	corpse_data["gray ooze corpse"] = FOOD_ACIDIC | FOOD_VEGETARIAN;
	corpse_data["green dragon corpse"] = FOOD_POISONOUS;
	corpse_data["green mold corpse"] = FOOD_ACIDIC | FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["green slime corpse"] = FOOD_ACIDIC | FOOD_POISONOUS | FOOD_SLIME | FOOD_VEGETARIAN;
	corpse_data["Green-elf corpse"] = FOOD_ELF;
	corpse_data["gremlin corpse"] = FOOD_POISONOUS;
	corpse_data["Grey-elf corpse"] = FOOD_ELF;
	corpse_data["guard corpse"] = FOOD_HUMAN;
	corpse_data["guardian naga corpse"] = FOOD_POISONOUS;
	corpse_data["guide corpse"] = FOOD_HUMAN;
	corpse_data["healer corpse"] = FOOD_HUMAN;
	corpse_data["high priest corpse"] = FOOD_HUMAN;
	corpse_data["Hippocrates' corpse"] = FOOD_HUMAN;
	corpse_data["homunculus corpse"] = FOOD_POISONOUS;
	corpse_data["housecat corpse"] = FOOD_AGGRAVATE;
	corpse_data["human corpse"] = FOOD_HUMAN;
	corpse_data["hunter corpse"] = FOOD_HUMAN;
	corpse_data["jellyfish corpse"] = FOOD_POISONOUS;
	corpse_data["Keystone Kop corpse"] = FOOD_HUMAN;
	corpse_data["killer bee corpse"] = FOOD_POISONOUS;
	corpse_data["King Arthur's corpse"] = FOOD_HUMAN;
	corpse_data["kitten corpse"] = FOOD_AGGRAVATE;
	corpse_data["knight corpse"] = FOOD_HUMAN;
	corpse_data["kobold corpse"] = FOOD_POISONOUS;
	corpse_data["kobold lord corpse"] = FOOD_POISONOUS;
	corpse_data["kobold shaman corpse"] = FOOD_POISONOUS;
	corpse_data["Kop Kaptain corpse"] = FOOD_HUMAN;
	corpse_data["Kop Lieutenant corpse"] = FOOD_HUMAN;
	corpse_data["Kop Sergeant corpse"] = FOOD_HUMAN;
	corpse_data["large cat corpse"] = FOOD_AGGRAVATE;
	corpse_data["large dog corpse"] = FOOD_AGGRAVATE;
	corpse_data["large kobold corpse"] = FOOD_POISONOUS;
	corpse_data["large mimic corpse"] = FOOD_MIMIC;
	corpse_data["leprechaun corpse"] = FOOD_TELEPORTITIS;
	corpse_data["lichen corpse"] = FOOD_PICKUP_INSTEAD | FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["lieutenant corpse"] = FOOD_HUMAN;
	corpse_data["little dog corpse"] = FOOD_AGGRAVATE;
	corpse_data["lizard corpse"] = FOOD_PICKUP_INSTEAD;
	corpse_data["Lord Carnarvon's corpse"] = FOOD_HUMAN;
	corpse_data["Lord Sato's corpse"] = FOOD_HUMAN;
	corpse_data["Master Assassin's corpse"] = FOOD_HUMAN;
	corpse_data["Master Kaen's corpse"] = FOOD_HUMAN;
	corpse_data["Master of Thieves' corpse"] = FOOD_HUMAN;
	corpse_data["Medusa's corpse"] = FOOD_PETRIFY | FOOD_POISONOUS;
	corpse_data["monk corpse"] = FOOD_HUMAN;
	corpse_data["mountain nymph corpse"] = FOOD_TELEPORTITIS;
	corpse_data["neanderthal corpse"] = FOOD_HUMAN;
	corpse_data["Neferet the Green's corpse"] = FOOD_HUMAN;
	corpse_data["ninja corpse"] = FOOD_HUMAN;
	corpse_data["Norn's corpse"] = FOOD_HUMAN;
	corpse_data["nurse corpse"] = FOOD_HUMAN;
	corpse_data["ochre jelly corpse"] = FOOD_ACIDIC | FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["Oracle's corpse"] = FOOD_HUMAN;
	corpse_data["Orion's corpse"] = FOOD_HUMAN;
	corpse_data["page corpse"] = FOOD_HUMAN;
	corpse_data["Pelias' corpse"] = FOOD_HUMAN;
	corpse_data["Pestilence's corpse"] = FOOD_DIE;
	corpse_data["pit viper corpse"] = FOOD_POISONOUS;
	corpse_data["priest corpse"] = FOOD_HUMAN;
	corpse_data["priestess corpse"] = FOOD_HUMAN;
	corpse_data["prisoner corpse"] = FOOD_HUMAN;
	corpse_data["quantum mechanic corpse"] = FOOD_POISONOUS;
	corpse_data["queen bee corpse"] = FOOD_POISONOUS;
	corpse_data["quivering blob corpse"] = FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["rabid rat corpse"] = FOOD_POISONOUS;
	corpse_data["ranger corpse"] = FOOD_HUMAN;
	corpse_data["red mold corpse"] = FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["rogue corpse"] = FOOD_HUMAN;
	corpse_data["roshi corpse"] = FOOD_HUMAN;
	corpse_data["salamander corpse"] = FOOD_POISONOUS;
	corpse_data["samurai corpse"] = FOOD_HUMAN;
	corpse_data["scorpion corpse"] = FOOD_POISONOUS;
	corpse_data["Scorpius' corpse"] = FOOD_POISONOUS;
	corpse_data["sergeant corpse"] = FOOD_HUMAN;
	corpse_data["Shaman Karnov's corpse"] = FOOD_HUMAN;
	corpse_data["shopkeeper corpse"] = FOOD_HUMAN;
	corpse_data["shrieker corpse"] = FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["small mimic corpse"] = FOOD_MIMIC;
	corpse_data["snake corpse"] = FOOD_POISONOUS;
	corpse_data["soldier corpse"] = FOOD_HUMAN;
	corpse_data["soldier ant corpse"] = FOOD_POISONOUS;
	corpse_data["spotted jelly corpse"] = FOOD_ACIDIC | FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["stalker corpse"] = FOOD_STUN;
	corpse_data["student corpse"] = FOOD_HUMAN;
	corpse_data["tengu corpse"] = FOOD_TELEPORTITIS;
	corpse_data["thug corpse"] = FOOD_HUMAN;
	corpse_data["tourist corpse"] = FOOD_HUMAN;
	corpse_data["Twoflower's corpse"] = FOOD_HUMAN;
	corpse_data["valkyrie corpse"] = FOOD_HUMAN;
	corpse_data["vampire bat corpse"] = FOOD_POISONOUS;
	corpse_data["violet fungus corpse"] = FOOD_HALLUCINOGENIC | FOOD_VEGAN | FOOD_VEGETARIAN;
	corpse_data["warrior corpse"] = FOOD_HUMAN;
	corpse_data["watch captain corpse"] = FOOD_HUMAN;
	corpse_data["watchman corpse"] = FOOD_HUMAN;
	corpse_data["water moccasin corpse"] = FOOD_POISONOUS;
	corpse_data["water nymph corpse"] = FOOD_TELEPORTITIS;
	corpse_data["werejackal corpse"] = FOOD_HUMAN | FOOD_LYCANTHROPY | FOOD_POISONOUS;
	corpse_data["wererat corpse"] = FOOD_HUMAN | FOOD_LYCANTHROPY | FOOD_POISONOUS;
	corpse_data["werewolf corpse"] = FOOD_HUMAN | FOOD_LYCANTHROPY | FOOD_POISONOUS;
	corpse_data["wizard corpse"] = FOOD_HUMAN;
	corpse_data["Wizard of Yendor's corpse"] = FOOD_HUMAN | FOOD_TELEPORTITIS;
	corpse_data["wood nymph corpse"] = FOOD_TELEPORTITIS;
	corpse_data["Woodland-elf corpse"] = FOOD_ELF;
	corpse_data["xan corpse"] = FOOD_POISONOUS;
	corpse_data["yellow dragon corpse"] = FOOD_ACIDIC;
	corpse_data["yellow mold corpse"] = FOOD_HALLUCINOGENIC | FOOD_POISONOUS | FOOD_VEGAN | FOOD_VEGETARIAN;
}

/* methods */
void Food::analyze() {
	/* update prev_monster_loc with seen monsters (not standing on a stash) */
	prev_monster_loc.clear();
	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.find(m->first);
		if (s != saiph->levels[saiph->position.level].stashes.end()) {
			/* there's a stash here, might be an old corpse, don't gamble */
			safe_eat_loc.erase(s->first); // erase in case we kill edible, see stash, make it safe_eat_loc, then kill inedible before eating there
			continue;
		}
		prev_monster_loc[m->first] = m->second.symbol;
	}
	/* are we hungry? */
	if (saiph->world->player.hunger <= WEAK) {
		/* yes, we are */
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
		/* easter egg: eat bytes when [over]satiated */
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name == "byte" || i->second.name == "bytes") {
				command = EAT;
				command2 = i->first;
				priority = FOOD_EAT_HUNGRY_PRIORITY;
				return;
			}
		}
	}
	if (saiph->on_ground != NULL && saiph->world->player.hunger < SATIATED && priority < FOOD_EAT_CORPSE_PRIORITY) {
		map<Point, int>::iterator s = safe_eat_loc.find(saiph->position);
		if (s != safe_eat_loc.end() && s->second + FOOD_CORPSE_EAT_TIME > saiph->world->player.turn) {
			/* it's safe to eat corpses here */
			for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
				if (m->second.symbol == '@' && m->second.color == WHITE && m->second.visible)
					return; // we see a white '@', don't eat (nor loot)
			}
			/* there are items here, we should look for corpses to eat */
			for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
				if (i->name.find(FOOD_CORPSES, i->name.size() - sizeof (FOOD_CORPSES) + 1) != string::npos || i->name.find(FOOD_CORPSE, i->name.size() - sizeof (FOOD_CORPSE) + 1) != string::npos) {
					/* there's a corpse in the stash, is it edible? */
					if (safeToEat(i->name)) {
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
}

void Food::parseMessages(const string &messages) {
	if (command2 == "ate corpse") {
		/* just ate a corpse, we should look at ground */
		priority = PRIORITY_LOOK;
		command = LOOK;
		command2.clear();
		return;
	}
	string::size_type pos;
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_EAT, 0) != string::npos) {
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		/* food gone, make inventory dirty */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		return;
	} else if ((pos = messages.find(FOOD_EAT_IT_2, 0)) != string::npos || (pos = messages.find(FOOD_EAT_ONE_2, 0)) != string::npos) {
		/* asks if we should eat the stuff on the floor */
		priority = PRIORITY_CONTINUE_ACTION;
		map<Point, int>::iterator s = safe_eat_loc.find(saiph->position);
		if (s == safe_eat_loc.end() || s->second + FOOD_CORPSE_EAT_TIME <= saiph->world->player.turn) {
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
			command2 = "ate corpse";
		} else {
			command = NO;
		}
		return;
	} else if ((pos = messages.find(FOOD_YOU_KILL, 0)) != string::npos) {
		/* we killed a monster.
		 * look for a spot where there used to be a monster,
		 * but now is a stash instead */
		string::size_type pos2 = messages.find("!  ", pos);
		if (pos2 != string::npos) {
			for (map<Point, unsigned char>::iterator s = prev_monster_loc.begin(); s != prev_monster_loc.end(); ++s) {
				if (saiph->levels[saiph->position.level].stashes.find(s->first) != saiph->levels[saiph->position.level].stashes.end()) {
					/* there's a stash where we last saw the monster.
					 * unless the monster symbol is 'Z', 'M' or 'V', it's probably not tainted.
					 * we've already checked that it wasn't a stash here before the monster went there */
					if (s->second == 'Z' || s->second == 'M' || s->second == 'V')
						safe_eat_loc.erase(s->first); // tainted corpse
					else
						safe_eat_loc[s->first] = saiph->world->player.turn;
					return;
				}
			}
		}
	} else if ((pos = messages.find(FOOD_IS_KILLED, 0)) != string::npos) {
		/* we saw a monster die.
		 * look for a spot where there used to be a monster,
		 * but now is a stash instead */
		string::size_type pos2 = pos;
		pos = messages.rfind("  The ", pos2);
		if (pos != string::npos) {
			for (map<Point, unsigned char>::iterator s = prev_monster_loc.begin(); s != prev_monster_loc.end(); ++s) {
				if (saiph->levels[saiph->position.level].stashes.find(s->first) != saiph->levels[saiph->position.level].stashes.end()) {
					/* there's a stash where we last saw the monster.
					 * unless the monster symbol is 'Z', 'M' or 'V', it's probably not tainted.
					 * we've already checked that it wasn't a stash here before the monster went there */
					if (s->second == 'Z' || s->second == 'M' || s->second == 'V')
						safe_eat_loc.erase(s->first); // tainted corpse
					else
						safe_eat_loc[s->first] = saiph->world->player.turn;
					return;
				}
			}
		}
	} else if (saiph->world->question && messages.find(FOOD_STOP_EATING, 0) != string::npos) {
		/* we should stop eating when we get this message */
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

bool Food::request(const Request &request) {
	if (request.request == REQUEST_EAT) {
		if (request.priority < priority)
			return false;
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name != request.data)
				continue;
			command = EAT;
			command2 = i->first;
			priority = request.priority;
			return true;
		}
	}
	return false;
}

/* private methods */
bool Food::safeToEat(const string &corpse) {
	/* this method returns true if it's safe to eat given corpse */
	map<string, int>::iterator c = corpse_data.find(corpse);
	if (c == corpse_data.end())
		return true;
	else if ((c->second & FOOD_PICKUP_INSTEAD) != 0)
		return false;
	/* acidic ain't so bad
	else if ((c->second & FOOD_ACIDIC) != 0)
		return false;
	*/
	else if ((c->second & FOOD_AGGRAVATE) != 0)
		return false;
	else if ((c->second & FOOD_DIE) != 0)
		return false;
	/* eat dwarves for now
	else if ((c->second & FOOD_DWARF) != 0)
		return false;
	*/
	/* eat elves for now
	else if ((c->second & FOOD_ELF) != 0)
		return false;
	*/
	/* eat gnomes for now
	else if ((c->second & FOOD_GNOME) != 0)
		return false;
	*/
	else if ((c->second & FOOD_HALLUCINOGENIC) != 0)
		return false;
	else if ((c->second & FOOD_HUMAN) != 0)
		return false;
	else if ((c->second & FOOD_LYCANTHROPY) != 0)
		return false;
	else if ((c->second & FOOD_MIMIC) != 0)
		return false;
	else if ((c->second & FOOD_PETRIFY) != 0)
		return false;
	else if ((c->second & FOOD_POISONOUS) != 0 && !saiph->world->player.poison_resistance)
		return false;
	else if ((c->second & FOOD_POLYMORPH) != 0)
		return false;
	else if ((c->second & FOOD_SLIME) != 0)
		return false;
	else if ((c->second & FOOD_STUN) != 0)
		return false;
	/* teleportitis might be fun for a bot
	else if ((c->second & FOOD_TELEPORTITIS) != 0)
		return false;
	*/
	/* we're not vegan
	else if ((c->second & FOOD_VEGAN) != 0)
		return false;
	*/
	/* nor vegetarian
	else if ((c->second & FOOD_VEGETARIAN) != 0)
		return false;
	*/
	return true;
}
