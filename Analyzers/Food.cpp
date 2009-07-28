#include "Food.h"
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Action/Eat.h"
#include "../Action/Pray.h"
#include "../Data/Corpse.h"
#include "../Data/Food.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Food::Food() : Analyzer("Food") {
	/* try to eat the food with lowest nutrition/weight first, and avoid food that cures bad effects */
	for (map<string, data::Food *>::iterator f = data::Food::foods.begin() f != data::Food::foods.end(); ++f) {
		int priority = 1000;
		priority -= f->second.nutrition / f->second.weight;
		if (f->second.eat_effects & EAT_EFFECT_CURE_BLINDNESS)
			priority -= 200;
		if (f->second.eat_effects & EAT_EFFECT_CURE_SICKNESS)
			priority -= 400;
		if (f->second.eat_effects & EAT_EFFECT_CURE_LYCANTHROPY)
			priority -= 600;
		if (f->second.eat_effects & EAT_EFFECT_CURE_STONING)
			priority -= 800;
		eat_priority[f->first] = priority;
	}
}

/* methods */
void Food::analyze() {
	if (Saiph::encumbrance >= OVERTAXED)
		return; // we can't eat while carrying too much

	/* update prev_monster_loc with seen monsters (not standing on a stash) */
	prev_monster_loc.clear();
	for (map<Point, Monster>::iterator m = World::levels[Saiph::position.level].monsters.begin(); m != World::levels[Saiph::position.level].monsters.end(); ++m) {
		if (m->second.visible)
			prev_monster_loc[m->first] = m->second.symbol;
	}

	/* are we hungry? */
	if (Saiph::hunger <= WEAK) {
		/* yes, we are, eat the food item in our inventory with lowest priority */
		if (food_items.size() > 0) {
			map<unsigned char, Item>::iterator eat = Inventory::items.end();
			for (set<unsigned char>::iterator f = food_items.begin(); f != food_items.end(); ++f) {
				map<unsigned char, Item>::iterator i = Inventory::items.find(*f);
				map<string, int>::iterator ep = eat_priority.find(i->second.name);
				if (i == Inventory::items.end()) {
					/* this should not happen */
					Debug::analyzer(name) << "Food item mysteriously disappeared from inventory slot '" << *f << "'" << endl;
					continue;
				} else if (ep == eat_priority.end()) {
					/* neither should this */
					Debug::analyzer(name) << "Want to eat item '" << i->second << "', but that's not in our list of edible items" << endl;
					continue;
				} else if (eat == Inventory::items.end() || eat_priority.find(eat->second.name)->second > ep->second) {
					/* this food item got a lower eat priority than previous (if any) food item */
					eat = i;
				}
			}
			if (eat != Inventory::items.end()) {
				/* we got something to eat, hooray! */
				World::setAction(static_cast<action::Action *>(new action::Eat(this, eat->first, (Saiph::hunger == WEAK ? PRIORITY_FOOD_EAT_WEAK : PRIORITY_FOOD_EAT_FAINTING))));
				return;
			}
		}
		/* hmm, nothing to eat, how bad is it? */
		if (Saiph::hunger <= WEAK) {
			/* bad enough to pray for help.
			 * if this doesn't work... help! */
			if (action::Pray::isSafeToPray())
				World::setAction(static_cast<action::Action *>(new action::Pray(this, PRIORITY_FOOD_PRAY_FOR_FOOD)));
		}
	}

	/* TODO: event */
	/* check if there's a fresh corpse on the ground */
	if (Saiph::on_ground != NULL && priority < PRIORITY_FOOD_EAT_CORPSE && Saiph::getDungeonSymbol() != SHOP_TILE) {
		map<Point, int>::iterator c = corpse_loc.find(Saiph::position);
		if (c != corpse_loc.end() && c->second + FOOD_CORPSE_EAT_TIME > Saiph::turn) {
			/* it's safe to eat corpses here */
			for (list<Item>::iterator i = Saiph::on_ground->items.begin(); i != Saiph::on_ground->items.end(); ++i) {
				if (i->name.size() >= sizeof (FOOD_CORPSE) + 1 && i->name.find(FOOD_CORPSE, 0) == i->name.size() - sizeof (FOOD_CORPSE) + 1) {
					/* there's a corpse in the stash, is it edible? */
					if ((Saiph::hunger < SATIATED && safeToEat(i->name)) || i->name == "floating eye corpse" || i->name == "wraith corpse") {
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
	if (!World::question && command2 == "ate corpse") {
		/* just ate a corpse, we should look at ground */
		priority = PRIORITY_LOOK;
		command = LOOK;
		command2.clear();
		return;
	}
	string::size_type pos;
	if (World::question && messages.find(MESSAGE_WHAT_TO_EAT, 0) != string::npos) {
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
			Saiph::request(req);
			return;
		}
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		/* food gone, make inventory dirty */
		req.request = REQUEST_DIRTY_INVENTORY;
		Saiph::request(req);
		return;
	} else if ((pos = messages.find(FOOD_EAT_IT_2, 0)) != string::npos || (pos = messages.find(FOOD_EAT_ONE_2, 0)) != string::npos) {
		/* asks if we should eat the stuff on the floor */
		priority = PRIORITY_CONTINUE_ACTION;
		map<Point, int>::iterator c = corpse_loc.find(Saiph::position);
		if (c == corpse_loc.end() || c->second + FOOD_CORPSE_EAT_TIME <= Saiph::turn) {
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
					corpse_loc[p->first] = Saiph::turn;
				}
			}
		}
		/* also clear "corpse_loc" on squares where there are no items nor monsters */
		for (map<Point, int>::iterator c = corpse_loc.begin(); c != corpse_loc.end(); ) {
			if (World::levels[Saiph::position.level].monsters.find(c->first) == World::levels[Saiph::position.level].monsters.end() && World::levels[Saiph::position.level].stashes.find(c->first) == World::levels[Saiph::position.level].stashes.end()) {
				corpse_loc.erase(c++);
				continue;
			}
			++c;
		}
	} else if (World::question && messages.find(FOOD_STOP_EATING, 0) != string::npos) {
		/* we should stop eating when we get this message */
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

void Food::onEvent(Event *const event) {
}

/* TODO: remove
bool Food::request(const Request &request) {
	if (request.request == REQUEST_EAT) {
		if (request.priority < priority)
			return false;
		for (map<unsigned char, Item>::iterator i = Inventory::items.begin(); i != Inventory::items.end(); ++i) {
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
*/

/* private methods */
bool Food::safeToEat(const string &corpse) {
	/* this method returns true if it's safe to eat given corpse */
	map<string, data::Corpse *>::iterator c = data::Corpse::corpses.find(corpse);
	if (c == data::Corpse::corpses.end())
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
			!(Saiph::intrinsics & PROPERTY_POISON ||
			  Saiph::extrinsics & PROPERTY_POISON))
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
	/* since we took out lizards from data::Corpse, no monster got this effect
	else if ((c->second->eat_effects & EAT_EFFECT_CURE_STONING) != 0)
		return false;
	*/
	/* since we took out lichens from data::Corpse, no monster got this effect
	else if ((c->second->eat_effects & EAT_EFFECT_REDUCE_STUNNING) != 0)
		return false;
	*/
	/* since we took out lichens from data::Corpse, no monster got this effect
	else if ((c->second->eat_effects & EAT_EFFECT_REDUCE_CONFUSION) != 0)
		return false;
	*/
	/* sure, let's go invisible if we can
	else if ((c->second->eat_effects & EAT_EFFECT_INVISIBILITY) != 0)
		return false;
	*/
	return true;
}
