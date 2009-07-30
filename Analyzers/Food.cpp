#include "Food.h"
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Eat.h"
#include "../Actions/EatCorpse.h"
#include "../Actions/Pray.h"
#include "../Data/Corpse.h"
#include "../Data/Food.h"
#include "../Events/EatItem.h"
#include "../Events/ItemsOnGround.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Food::Food() : Analyzer("Food") {
	/* try to eat the food with lowest nutrition/weight first, and avoid food that cures bad effects */
	for (map<string, data::Food *>::iterator f = data::Food::foods.begin(); f != data::Food::foods.end(); ++f) {
		int priority = 1000;
		priority -= f->second->nutrition / f->second->weight;
		if (f->second->eat_effects & EAT_EFFECT_CURE_BLINDNESS)
			priority -= 200;
		if (f->second->eat_effects & EAT_EFFECT_CURE_SICKNESS)
			priority -= 400;
		if (f->second->eat_effects & EAT_EFFECT_CURE_LYCANTHROPY)
			priority -= 600;
		if (f->second->eat_effects & EAT_EFFECT_CURE_STONING)
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
}

void Food::parseMessages(const string &messages) {
	if ((messages.find(MESSAGE_YOU_KILL)) != string::npos || (messages.find(MESSAGE_YOU_DESTROY) != string::npos) || (messages.find(MESSAGE_IS_KILLED)) != string::npos || (messages.find(MESSAGE_IS_DESTROYED)) != string::npos) {
		/* we killed a monster or saw a monster die */
		for (map<Point, unsigned char>::iterator p = prev_monster_loc.begin(); p != prev_monster_loc.end(); ++p) {
			if (p->second == 'Z' || p->second == 'M' || p->second == 'V') {
				/* wherever monsters with symbol Z, M or V die, we'll mark as "tainted corpse" */
				corpse_loc[p->first] = 0 - FOOD_CORPSE_EAT_TIME;
			} else if (corpse_loc.find(p->first) == corpse_loc.end()) {
				/* monster probably leaves an edible corpse */
				corpse_loc[p->first] = World::turn;
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
	}
}

void Food::onEvent(Event *const event) {
	if (event->getID() == ItemsOnGround::id && World::getDungeonSymbol() != SHOP_TILE) {
		map<Point, int>::iterator cl = corpse_loc.find(Saiph::position);
		if (cl != corpse_loc.end() && cl->second + FOOD_CORPSE_EAT_TIME > World::turn) {
			/* it's safe to eat corpses here */
			ItemsOnGround *e = static_cast<ItemsOnGround *>(event);
			for (list<Item>::iterator i = e->items.begin(); i != e->items.end(); ++i) {
				map<string, data::Corpse *>::iterator c = data::Corpse::corpses.find(i->name);
				if (c == data::Corpse::corpses.end())
					continue; // item is not a corpse
				if (!safeToEat(c))
					continue; // corpse is not safe to eat
				/* we should eat this corpse */
				World::setAction(static_cast<action::Action *>(new action::EatCorpse(this, i->name, PRIORITY_FOOD_EAT_CORPSE)));
				break;
			}
		}
	} else if (event->getID() == EatItem::id) {
		EatItem *e = static_cast<EatItem *>(event);
		World::setAction(static_cast<action::Action *>(new action::Eat(this, e->key, e->priority)));
	}
}

/* private methods */
bool Food::safeToEat(map<string, data::Corpse *>::iterator c) {
	/* this method returns true if it's safe to eat given corpse */
	if (Saiph::hunger >= SATIATED && !(c->second->eat_effects & EAT_EFFECT_GAIN_LEVEL) && !(c->second->eat_effects & EAT_EFFECT_ESP))
		return false; // satiated and eating it won't give us benefits that's worth the risk of choking
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
