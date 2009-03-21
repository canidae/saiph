#include "Food.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Data/CorpseData.h"

using namespace std;

/* constructors/destructor */
Food::Food(Saiph *saiph) : Analyzer("Food"), saiph(saiph) {
	command2 = "";
	priority = ILLEGAL_PRIORITY;
	eat_order.push_back("partly eaten meatball");
	eat_order.push_back("meatball");
	eat_order.push_back("partly eaten meat ring");
	eat_order.push_back("meat ring");
	eat_order.push_back("partly eaten meat stick");
	eat_order.push_back("meat stick");
	eat_order.push_back("partly eaten huge chunk of meat");
	eat_order.push_back("huge chunk of meat");
	eat_order.push_back("partly eaten cream pie");
	eat_order.push_back("cream pie");
	eat_order.push_back("partly eaten lichen corpse");
	eat_order.push_back("lichen corpse");
	eat_order.push_back("partly eaten tripe ration");
	eat_order.push_back("tripe ration");
	eat_order.push_back("partly eaten melon");
	eat_order.push_back("melon");
	eat_order.push_back("partly eaten apple");
	eat_order.push_back("apple");
	eat_order.push_back("partly eaten pear");
	eat_order.push_back("pear");
	eat_order.push_back("partly eaten kelp frond");
	eat_order.push_back("kelp frond");
	eat_order.push_back("partly eaten C-ration");
	eat_order.push_back("C-ration");
	eat_order.push_back("partly eaten cram ration");
	eat_order.push_back("cram ration");
	eat_order.push_back("partly eaten food ration");
	eat_order.push_back("food ration");
	eat_order.push_back("partly eaten gunyoki");
	eat_order.push_back("gunyoki");
	eat_order.push_back("partly eaten K-ration");
	eat_order.push_back("K-ration");
	eat_order.push_back("partly eaten clove of garlic");
	eat_order.push_back("clove of garlic");
	eat_order.push_back("partly eaten fortune cookie");
	eat_order.push_back("fortune cookie");
	eat_order.push_back("partly eaten banana");
	eat_order.push_back("banana");
	eat_order.push_back("partly eaten orange");
	eat_order.push_back("orange");
	eat_order.push_back("partly eaten candy bar");
	eat_order.push_back("candy bar");
	eat_order.push_back("partly eaten byte");
	eat_order.push_back("byte");
	eat_order.push_back("partly eaten lump of royal jelly");
	eat_order.push_back("lump of royal jelly");
	eat_order.push_back("partly eaten pancake");
	eat_order.push_back("pancake");
	eat_order.push_back("partly eaten lembas wafer");
	eat_order.push_back("lembas wafer");
	eat_order.push_back("partly eaten carrot"); // will cure blindness
	eat_order.push_back("carrot"); // will cure blindness
	eat_order.push_back("partly eaten eucalyptus leaf"); // will cure sickness
	eat_order.push_back("eucalyptus leaf"); // will cure sickness
	eat_order.push_back("partly eaten sprig of wolfsbane"); // will cure lycanthropy
	eat_order.push_back("sprig of wolfsbane"); // will cure lycanthropy
	eat_order.push_back("partly eaten lizard corpse"); // wil cure stoning
	eat_order.push_back("lizard corpse"); // wil cure stoning
}

/* methods */
void Food::analyze() {
	/* update prev_monster_loc with seen monsters (not standing on a stash) */
	prev_monster_loc.clear();
	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m)
		prev_monster_loc[m->first] = m->second.symbol;
	/* we can't eat while acrrying too much.
	   TODO drop things so we can eat */
	if (saiph->world->player.encumbrance >= OVERTAXED)
		return;
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
							priority = PRIORITY_FOOD_EAT_HUNGRY;
							break;

						case WEAK:
							priority = PRIORITY_FOOD_EAT_WEAK;
							break;

						default:
							priority = PRIORITY_FOOD_EAT_FAINTING;
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
			req.priority = PRIORITY_FOOD_PRAY_FOR_FOOD;
			saiph->request(req);
		}
	}
	if (saiph->on_ground != NULL && priority < PRIORITY_FOOD_EAT_CORPSE && saiph->getDungeonSymbol() != SHOP_TILE) {
		map<Point, int>::iterator c = corpse_loc.find(saiph->position);
		if (c != corpse_loc.end() && c->second + FOOD_CORPSE_EAT_TIME > saiph->world->player.turn) {
			/* it's safe to eat corpses here */
			for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
				if (i->name.size() >= sizeof (FOOD_CORPSE) + 1 && i->name.find(FOOD_CORPSE, 0) == i->name.size() - sizeof (FOOD_CORPSE) + 1) {
					/* there's a corpse in the stash, is it edible? */
					if ((saiph->world->player.hunger < SATIATED && safeToEat(i->name)) || i->name == "floating eye corpse" || i->name == "wraith corpse") {
						/* it is, and we know we can eat corpses on this position */
						command = EAT;
						command2 = i->name;
						priority = PRIORITY_FOOD_EAT_CORPSE;
						return;
					}
				}
			}
		}
	}
}

void Food::parseMessages(const string &messages) {
	if (!saiph->world->question && command2 == "ate corpse") {
		/* just ate a corpse, we should look at ground */
		priority = PRIORITY_LOOK;
		command = LOOK;
		command2.clear();
		return;
	}
	string::size_type pos;
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_EAT, 0) != string::npos) {
		if (command2.size() > 1) {
			/* sometimes (eating troll that arise) we try to eat
			 * a corpse that's gone.
			 * if command2.size() > 1 we meant to eat a corpse,
			 * but they we wouldn't be here.
			 * we must clear command2 & set command to CLOSE_PAGE */
			command2.clear();
			command = CLOSE_PAGE;
			priority = PRIORITY_CONTINUE_ACTION;
			/* also mark stash as dirty */
			req.request = REQUEST_DIRTY_STASH;
			saiph->request(req);
			return;
		}
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		/* food gone, make inventory dirty */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		return;
	} else if ((pos = messages.find(FOOD_EAT_IT_2, 0)) != string::npos || (pos = messages.find(FOOD_EAT_ONE_2, 0)) != string::npos) {
		/* asks if we should eat the stuff on the floor */
		priority = PRIORITY_CONTINUE_ACTION;
		map<Point, int>::iterator c = corpse_loc.find(saiph->position);
		if (c == corpse_loc.end() || c->second + FOOD_CORPSE_EAT_TIME <= saiph->world->player.turn) {
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
	} else if ((pos = messages.find(FOOD_YOU_KILL, 0)) != string::npos || (pos = messages.find(FOOD_YOU_DESTROY, 0) != string::npos) || (pos = messages.find(FOOD_IS_KILLED, 0)) != string::npos || (pos = messages.find(FOOD_IS_DESTROYED, 0)) != string::npos) {
		/* we killed a monster or saw a monster die.
		 * look for a spot where there used to be a monster,
		 * but now is a stash instead */
		if (messages.rfind("  The ", pos) != string::npos || messages.find("!  ", pos) != string::npos) {
			for (map<Point, unsigned char>::iterator p = prev_monster_loc.begin(); p != prev_monster_loc.end(); ++p) {
				if (p->second == 'Z' || p->second == 'M' || p->second == 'V') {
					/* wherever monsters with symbol Z, M or V die, we'll mark as "tainted corpse" */
					corpse_loc[p->first] = 0 - FOOD_CORPSE_EAT_TIME;
				} else if (corpse_loc.find(p->first) == corpse_loc.end()) {
					/* monster probably leaves an edible corpse */
					corpse_loc[p->first] = saiph->world->player.turn;
				}
			}
		}
		/* also clear "corpse_loc" on squares where there are no items nor monsters */
		for (map<Point, int>::iterator c = corpse_loc.begin(); c != corpse_loc.end(); ) {
			if (saiph->levels[saiph->position.level].monsters.find(c->first) == saiph->levels[saiph->position.level].monsters.end() && saiph->levels[saiph->position.level].stashes.find(c->first) == saiph->levels[saiph->position.level].stashes.end()) {
				corpse_loc.erase(c++);
				continue;
			}
			++c;
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
	map<string, CorpseData *>::iterator c = CorpseData::corpses.find(corpse);
	if (c == CorpseData::corpses.end())
		return false;
	/* acidic ain't so bad
	else if ((c->second->eat_effects & EAT_EFFECT_ACIDIC) != 0)
		return false;
	*/
	else if ((c->second->eat_effects & EAT_EFFECT_AGGRAVATE) != 0)
		return false;
	else if ((c->second->eat_effects & EAT_EFFECT_DIE) != 0)
		return false;
	/* eat dwarves for now
	else if ((c->second->eat_effects & EAT_EFFECT_DWARF) != 0)
		return false;
	*/
	/* eat elves for now
	else if ((c->second->eat_effects & EAT_EFFECT_ELF) != 0)
		return false;
	*/
	/* eat gnomes for now
	else if ((c->second->eat_effects & EAT_EFFECT_GNOME) != 0)
		return false;
	*/
	else if ((c->second->eat_effects & EAT_EFFECT_HALLUCINOGENIC) != 0)
		return false;
	else if ((c->second->eat_effects & EAT_EFFECT_HUMAN) != 0)
		return false;
	else if ((c->second->eat_effects & EAT_EFFECT_LYCANTHROPY) != 0)
		return false;
	/* mimic for some turns isn't that bad, is it?
	else if ((c->second->eat_effects & EAT_EFFECT_MIMIC) != 0)
		return false;
	*/
	else if ((c->second->eat_effects & EAT_EFFECT_PETRIFY) != 0)
		return false;
	else if ((c->second->eat_effects & EAT_EFFECT_POISONOUS) != 0 && 
			!(saiph->world->player.intrinsics & PROPERTY_POISON ||
			  saiph->world->player.extrinsics & PROPERTY_POISON))
		return false;
	else if ((c->second->eat_effects & EAT_EFFECT_POLYMORPH) != 0)
		return false;
	else if ((c->second->eat_effects & EAT_EFFECT_SLIME) != 0)
		return false;
	else if ((c->second->eat_effects & EAT_EFFECT_STUN) != 0)
		return false;
	/* teleportitis might be fun for a bot
	else if ((c->second->eat_effects & EAT_EFFECT_TELEPORTITIS) != 0)
		return false;
	*/
	/* we're not vegan
	else if ((c->second->eat_effects & EAT_EFFECT_VEGAN) != 0)
		return false;
	*/
	/* nor vegetarian
	else if ((c->second->eat_effects & EAT_EFFECT_VEGETARIAN) != 0)
		return false;
	*/
	/* let's eat stuff that makes us strong
	else if ((c->second->eat_effects & EAT_EFFECT_STRENGTH) != 0)
		return false;
	*/
	/* and stuff that gives us a level
	else if ((c->second->eat_effects & EAT_EFFECT_GAIN_LEVEL) != 0)
		return false;
	*/
	/* and stuff that heals us
	else if ((c->second->eat_effects & EAT_EFFECT_HEAL) != 0)
		return false;
	*/
	else if ((c->second->eat_effects & EAT_EFFECT_SPEED_TOGGLE) != 0)
		return false;
	/* since we took out lizards from CorpseData, no monster got this effect
	else if ((c->second->eat_effects & EAT_EFFECT_CURE_STONING) != 0)
		return false;
	*/
	/* since we took out lichens from CorpseData, no monster got this effect
	else if ((c->second->eat_effects & EAT_EFFECT_REDUCE_STUNNING) != 0)
		return false;
	*/
	/* since we took out lichens from CorpseData, no monster got this effect
	else if ((c->second->eat_effects & EAT_EFFECT_REDUCE_CONFUSION) != 0)
		return false;
	*/
	/* sure, let's go invisible if we can
	else if ((c->second->eat_effects & EAT_EFFECT_INVISIBILITY) != 0)
		return false;
	*/
	return true;
}
