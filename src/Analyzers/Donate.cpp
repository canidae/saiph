#include "Analyzers/Donate.h"

#include "EventBus.h"
#include "Monster.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Donate.h"
#include "Actions/Move.h"
#include "Events/Event.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

Donate::Donate() : Analyzer("Donate"), _priest_loc() {
	EventBus::registerEvent(WantItems::ID, this);
}

void Donate::analyze() {
	if (Saiph::zorkmids() < 400 * Saiph::experience())
		return; // not enough gold
	if (World::level().tile().symbol() == STAIRS_DOWN) {
		/* standing on stairs down or at same level as priest, find priest */
		_priest_loc.level(-1);
		unsigned int nearest = UNREACHABLE;
		for (int lev = 0; lev < (int) World::levels().size(); lev++) {
			for (map<Point, Monster>::const_iterator i = World::level(lev).monsters().begin(); i != World::level(lev).monsters().end(); ++i) {
				if (!i->second.priest())
					continue;
				Tile tile = World::shortestPath(Coordinate(lev, i->first));
				if (tile.cost() == UNREACHABLE)
					continue; // can't path to this priest
				if (tile.cost() < nearest) {
					/* this priest is closer than the previous priest */
					_priest_loc = tile.coordinate();
					nearest = tile.cost();
				}
			}
		}
	}
	if (_priest_loc.level() != -1) {
		if (_priest_loc.level() == Saiph::position().level()) {
			/* on same level as priest, update coordinate */
			_priest_loc.level(-1);
			for (map<Point, Monster>::const_iterator i = World::level().monsters().begin(); i != World::level().monsters().end(); ++i) {
				if (!i->second.priest())
					continue;
				Tile tile = World::shortestPath(Coordinate(Saiph::position().level(), i->first));
				if (tile.cost() == UNREACHABLE)
					continue; // can't path to this priest
				_priest_loc = tile.coordinate();
				break;
			}
		}
		if (_priest_loc.level() != -1) {
			Tile tile = World::shortestPath(_priest_loc);
			if (tile.distance() == 1)
				World::setAction(static_cast<action::Action*> (new action::Donate(this, tile.direction(), Saiph::experience() * 400, DONATE_PRIORITY)));
			else
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile, DONATE_PRIORITY)));
		}
	}
}

void Donate::onEvent(Event * const event) {
	if (event->id() == WantItems::ID) {
		/* we want gold to donate to priests */
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.name() == "gold piece")
				i->second.want(i->second.count());
		}
	}
}
