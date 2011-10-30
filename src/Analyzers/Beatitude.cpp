#include "Analyzers/Beatitude.h"

#include "EventBus.h"
#include "Globals.h"
#include "Item.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Drop.h"
#include "Actions/Move.h"
#include "Events/Beatify.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Beatitude::Beatitude() : Analyzer("Beatitude"), _max_priority(ILLEGAL_PRIORITY) {
	EventBus::registerEvent(Beatify::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Beatitude::analyze() {
	if (_beatify.size() <= 0)
		return; // no items to beatify
	else if (Saiph::blind() || Saiph::confused() || Saiph::stunned() || Saiph::hallucinating())
		return; // no buc-testing while blind/confused/stunned/hallucinating
	else if (_max_priority < World::currentPriority())
		return; // got something more important to do

	/* path to nearest altar */
	Tile tile = World::shortestPath(ALTAR);
	if (tile.cost() >= UNPASSABLE)
		return; // can't path to altar
	if (tile.direction() == NOWHERE)
		World::setAction(new action::Drop(this, _max_priority, false));
	else
		World::setAction(new action::Move(this, tile, action::Move::calculatePriority(_max_priority, tile.cost())));
}

void Beatitude::onEvent(event::Event* const event) {
	if (event->id() == Beatify::ID) {
		Beatify* e = static_cast<Beatify*> (event);
		_beatify.insert(e->key());
		if (e->priority() > _max_priority)
			_max_priority = e->priority();
	} else if (event->id() == WantItems::ID && World::level().tile().symbol() == ALTAR) {
		/* looting or picking up items at an altar */
		WantItems* e = static_cast<WantItems*> (event);
		if (e->dropping()) {
			for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
				if (i->second.beatitude() != BEATITUDE_UNKNOWN)
					continue; // know beatitude already
				/* should drop this item, force it by setting count to 0 */
				i->second.count(0);
			}
			_beatify.clear();
			_max_priority = ILLEGAL_PRIORITY;
		}
	}
}
