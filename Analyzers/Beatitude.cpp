#include "Beatitude.h"

#include "../Globals.h"
#include "../Item.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Drop.h"
#include "../Actions/Move.h"
#include "../Events/Beatify.h"
#include "../Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Beatitude::Beatitude() : Analyzer("Beatitude") {
}

/* methods */
void Beatitude::analyze() {
	if (_beatify.size() <= 0)
		return; // no items to beatify
	else if (Saiph::blind() || Saiph::confused() || Saiph::stunned())
		return; // no buc-testing while blind/confused/stunned
	else if (_max_priority < World::currentPriority())
		return; // got something more important to do

	/* path to nearest altar */
	Tile tile = World::shortestPath(ALTAR);
	if (tile.cost() >= UNPASSABLE)
		return; // can't path to altar
	if (tile.direction() == NOWHERE)
		World::setAction(static_cast<action::Action*> (new action::Drop(this, _max_priority)));
	else
		World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), _max_priority)));
}

void Beatitude::onEvent(event::Event * const event) {
	if (event->id() == Beatify::ID) {
		Beatify* e = static_cast<Beatify*> (event);
		set<unsigned char>::iterator b = _beatify.find(e->key());
		_beatify.insert(e->key());
		if (e->priority() > _max_priority)
			_max_priority = e->priority();
	} else if (event->id() == WantItems::ID && World::level().tile().symbol() == ALTAR) {
		/* looting or picking up items at an altar */
		WantItems* e = static_cast<WantItems*> (event);
		if (e->dropping()) {
			for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
				if (dropItem(i->second))
					i->second.count(0); // setting count to 0 to force dropping the item
			}
			_beatify.clear();
			_max_priority = ILLEGAL_PRIORITY;
		}
	}
}

/* private methods */
bool Beatitude::dropItem(const Item& item) {
	if (item.beatitude() != BEATITUDE_UNKNOWN)
		return false;
	if (item.name() == "gold piece")
		return false;
	if (item.additional() == "being worn")
		return false;
	if (item.additional() == "embedded in your skin")
		return false;
	if (item.additional() == "in use")
		return false;
	if (item.additional() == "wielded")
		return false;
	if (item.additional().find("weapon in ", 0) == 0 || item.additional().find("wielded in other ", 0) == 0)
		return false;
	if (item.additional().find("on left ", 0) == 0 || item.additional().find("on right ", 0) == 0)
		return false;
	return true;
}
