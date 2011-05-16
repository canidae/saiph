#include "Analyzers/Armor.h"

#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Answer.h"
#include "Actions/TakeOff.h"
#include "Actions/Wear.h"
#include "Data/Armor.h"
#include "Events/Beatify.h"
#include "Events/Event.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Armor::Armor() : Analyzer("Armor"), _put_on() {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Armor::analyze() {
	if (_put_on.size() > 0) {
		/* put on highest scoring armor */
		unsigned char best_key = ILLEGAL_ITEM;
		int best_score = 0;
		int best_slot = ILLEGAL_SLOT;
		for (set<unsigned char>::iterator k = _put_on.begin(); k != _put_on.end(); ++k) {
			const Item& to_wear = Inventory::itemAtKey(*k);
			map<const string, const data::Armor*>::const_iterator a = data::Armor::armors().find(to_wear.name());
			if (a == data::Armor::armors().end())
				continue;
			if (Saiph::polymorphed())
				continue;
			if (Inventory::itemInSlot(a->second->slot()).beatitude() == CURSED)
				continue; // can't take off existing armor in this slot
			if (a->second->slot() == SLOT_SHIRT && (Inventory::itemInSlot(SLOT_SUIT).beatitude() == CURSED || Inventory::itemInSlot(SLOT_CLOAK).beatitude() == CURSED))
				continue; // can't take off armor covering this slot
			if (a->second->slot() == SLOT_SUIT && Inventory::itemInSlot(SLOT_CLOAK).beatitude() == CURSED)
				continue; // can't take off armor covering this slot
			if (a->second->slot() == SLOT_CLOAK && (best_slot == SLOT_SUIT || best_slot == SLOT_SHIRT))
				continue; // already want to put on suit or shirt, can't put on cloak first then
			if (a->second->slot() == SLOT_SUIT && best_slot == SLOT_SHIRT)
				continue; // already want to put on shirt, can't put on suit first then
			int score = calculateArmorScore(to_wear, a->second);
			if (score > best_score || (a->second->slot() == SLOT_SHIRT && (best_slot == SLOT_SUIT || best_slot == SLOT_CLOAK)) || (a->second->slot() == SLOT_SUIT && best_slot == SLOT_CLOAK)) {
				/* score is better or the piece of armor goes on the inside of the currently best armor */
				best_key = *k;
				best_score = score;
				best_slot = a->second->slot();
			}
		}
		const Item& best_wear = Inventory::itemAtKey(best_key);
		map<const string, const data::Armor*>::const_iterator a = data::Armor::armors().find(best_wear.name());
		if (a != data::Armor::armors().end() && betterThanCurrent(best_wear)) {
			if (((a->second->slot() == SLOT_SHIRT || a->second->slot() == SLOT_SUIT) && Inventory::keyForSlot(SLOT_CLOAK) != ILLEGAL_ITEM))
				World::setAction(static_cast<action::Action*> (new action::TakeOff(this, Inventory::keyForSlot(SLOT_CLOAK), ARMOR_WEAR_PRIORITY)));
			else if (a->second->slot() == SLOT_SHIRT && Inventory::keyForSlot(SLOT_SUIT) != ILLEGAL_ITEM)
				World::setAction(static_cast<action::Action*> (new action::TakeOff(this, Inventory::keyForSlot(SLOT_SUIT), ARMOR_WEAR_PRIORITY)));
			else if (Inventory::keyForSlot(a->second->slot()) != ILLEGAL_ITEM)
				World::setAction(static_cast<action::Action*> (new action::TakeOff(this, Inventory::keyForSlot(a->second->slot()), ARMOR_WEAR_PRIORITY)));
			else
				World::setAction(static_cast<action::Action*> (new action::Wear(this, best_key, ARMOR_WEAR_PRIORITY)));
		} else {
			_put_on.erase(best_key);
		}
	}
}

void Armor::parseMessages(const string& messages) {
	string::size_type pos;
	if (World::question() && (pos = messages.find(MESSAGE_FOOCUBUS_QUESTION)) != string::npos) {
		/* A foocubus wants to remove something. Check the beatitude of that armor (and any armor covering it) */
		bool remove = false;
		if (messages.find(" cloak, ", pos) == pos || messages.find(" wrapping, ", pos) == pos || messages.find(" robe, ", pos) == pos || messages.find(" apron, ", pos) == pos || messages.find(" smock, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_CLOAK).beatitude() == CURSED || Inventory::itemInSlot(SLOT_SUIT).beatitude() == CURSED || Inventory::itemInSlot(SLOT_SHIRT).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" suit, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_SUIT).beatitude() == CURSED || Inventory::itemInSlot(SLOT_SHIRT).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" boots, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_BOOTS).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" gloves, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_GLOVES).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" shield, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_SHIELD).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" helmet, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_HELMET).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" shirt, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_SHIRT).beatitude() == CURSED)
				remove = true;
		}
		World::setAction(static_cast<action::Action*> (new action::Answer(this, (remove ? YES : NO))));
	}
}

void Armor::onEvent(event::Event* const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		for (set<unsigned char>::iterator k = e->keys().begin(); k != e->keys().end(); ++k) {
			if (betterThanCurrent(Inventory::itemAtKey(*k)))
				_put_on.insert(*k);
			else
				_put_on.erase(*k);
		}
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (data::Armor::armors().find(i->second.name()) == data::Armor::armors().end())
				continue; // not armor
			if (i->second.beatitude() != BEATITUDE_UNKNOWN) {
				/* known beatitude, should we wear it? */
				if (i->second.beatitude() != CURSED && betterThanCurrent(i->second))
					_put_on.insert(i->first); // it's not cursed and (possibly) better than what we're wearing
				else
					_put_on.erase(i->first);
				continue;
			}
			Beatify b(i->first, 175);
			EventBus::broadcast(&b);
		}
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
                if (Saiph::encumbrance() >= BURDENED) {
                    /* burdened and beatifying, (and is on a stash): pick up */
                    if(World::shortestPath(ALTAR).cost() < UNPASSABLE){
                        if(Saiph::encumbrance() < STRESSED){
                            if(e->safeStash()){
                                for(map<unsigned char, Item>::iterator i = e->items ().begin(); i != e->items().end(); ++i){
                                    if (betterThanCurrent(i->second))
                                        i->second.want(i->second.count());
                                }
                            }
                        }
                        /* burdened and not beatifying: stash */
                    } else  {
                        if(e->safeStash()){
                        }
                    }
                /* not burdened */
                } else {
                    /* and not on safe stash: pick up */
                    if(!e->safeStash()){
                        /* ^ this also means 'drop at every safe stash if not burdened' */
                        /* to avoid this, check whether on upstair, without stashing */
                        for(map<unsigned char, Item>::iterator i = e->items ().begin(); i != e->items().end(); ++i){
                            if (betterThanCurrent(i->second))
                                i->second.want(i->second.count());
                        }
                    }
                }
        }
}

/* private methods */
bool Armor::betterThanCurrent(const Item& item) {
	if (item.beatitude() == CURSED)
		return false;
	map<const string, const data::Armor*>::const_iterator a = data::Armor::armors().find(item.name());
	if (a == data::Armor::armors().end())
		return false; // not armor

	int score = calculateArmorScore(item, a->second);
	if (score <= 0)
		return false;
	const Item& cur_armor = Inventory::itemInSlot(a->second->slot());
	if (cur_armor.beatitude() == CURSED)
		return false; // current armor is cursed, can't replace it
	map<const string, const data::Armor*>::const_iterator a2 = data::Armor::armors().find(cur_armor.name());
	if (a2 != data::Armor::armors().end()) {
		int score2 = calculateArmorScore(cur_armor, a2->second);
		if (score <= score2)
			return false; // armor is worse than what we currently got
	}
	return true;
}

int Armor::calculateArmorScore(const Item& item, const data::Armor* armor) {
	int score = 0;
	score += armor->ac();
	score += armor->mc();
	score += item.unknownEnchantment() ? ARMOR_UNKNOWN_ENCHANTMENT_BONUS : item.enchantment();
	if (item.beatitude() == BLESSED)
		++score;
	score -= item.damage();
	return score;
}
