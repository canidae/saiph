#include "Analyzers/Shop.h"

#include <stdlib.h>
#include "Debug.h"
#include "Globals.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "EventBus.h"
#include "Actions/Pay.h"
#include "Actions/Look.h"
#include "Actions/Move.h"
#include "Actions/Drop.h"
#include "Data/Pickaxe.h"
#include "Events/ReceivedItems.h"
#include "Events/ShopDetected.h"
#include "Events/WantItems.h"

// Shop divides space into three regions.  "In shop" is SHOP_TILE and the walls and doors adjacent to them; "Threshold" is 1 square further out; "Out shop" is everything else.
// Shop maintains a "shop mode" bit, which serves mostly to control whether saiph wants digging tools.  Shop mode must be on in shops and off outside them.
// If saiph is going to step into a shop, she must be in threshold, and we need to drop or bag our digging tools.  To prevent her from immediately picking them back up, we set shop mode at the same time.

using namespace analyzer;
using namespace event;
using namespace std;

#define IN_SHOP_PROPER 0
#define IN_SHOP 1
#define THRESHOLD 2

/* constructors/destructor */
Shop::Shop() : Analyzer("Shop") {
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Shop::parseMessages(const string&) {
}

int Shop::nearShop(const Coordinate& where) {
	int y0 = where.row() - 2;
	int x0 = where.col() - 2;
	int rank = 3;

	if (where.level() == _tentative_shop_door.level())
		rank = 1 + Point::gridDistance(where, _tentative_shop_door);

	for (int y = y0; y <= y0 + 4; ++y) {
		for (int x = x0; x <= x0 + 4; ++x) {
			if (World::level().tile(Point(y,x)).symbol() == SHOP_TILE)
				rank = min(rank, Point::gridDistance(Point(y,x), where));
		}
	}

	return rank;
}

bool Shop::inBlockedDoorway() {
	static unsigned char check[4] = { N, E, S, W };
	int shopkeep = 0;
	int wall = 0;
	for (unsigned char *it = check; it != (check + 4); ++it) {
		Point p = Saiph::position();
		p.moveDirection(*it);

		Tile& t = World::level().tile(p);
		if (t.symbol() == HORIZONTAL_WALL || t.symbol() == VERTICAL_WALL)
			wall++;

		map<Point, Monster*>::const_iterator m = World::level().monsters().find(p);
		if (m != World::level().monsters().end() && m->second->shopkeeper())
			shopkeep++;
	}

	return wall == 2 && shopkeep == 1;
}

void Shop::setShopping(bool newState) {
	if (_shopping != newState)
		Debug::custom(name()) << "Shop status is now " << newState << endl;
	_shopping = newState;
}

void Shop::lastChance(action::Action* const plan) {
	if (plan->id() == action::Move::ID && nearShop(Saiph::position()) == THRESHOLD) {
		action::Move *m = static_cast<action::Move *>(plan);
		Coordinate to = Saiph::position();
		to.moveDirection(m->target().direction());
		int nnear = nearShop(to);
		if (nnear < THRESHOLD && !_shopping) {
			Debug::custom(name()) << "We seem to be moving into a shop!" << endl;
			setShopping(true);
			dropPicks();
		} else if (nnear > THRESHOLD && _shopping) {
			Debug::custom(name()) << "We seem to be moving out of a shop!" << endl;
			setShopping(false);
			// Force analyzers to rerun so we can maybe get our pick back
			// Maybe we should have a dedicated NoOp action for this
			World::setAction(new action::Look(this));
		}
	}
}

void Shop::dropPicks() {
	bool have_pick = false;
	for (map<unsigned char, Item>::const_iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (data::Pickaxe::pickaxes().find(i->second.name()) != data::Pickaxe::pickaxes().end())
			have_pick = true;
	}
	if (have_pick)
		World::setAction(new action::Drop(this, PRIORITY_SHOP_DROP_DIGGING_TOOL, false));
}

void Shop::onEvent(event::Event* const event) {
	if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (data::Pickaxe::pickaxes().find(i->second.name()) == data::Pickaxe::pickaxes().end())
				continue;

			int near = nearShop(Saiph::position());

			if (_shopping && near == THRESHOLD) {
				// drop them all
				// setting count to 0 prevents any other analyzer from overriding
				i->second.count(0);
			} else if (near == IN_SHOP) {
				// a pick in a doorway is a problem and should be picked up even if shopping
				i->second.want(i->second.count());
			}
		}
	}
}

void Shop::analyze() {
	int near_shop = nearShop(Saiph::position());

	if (!(near_shop <= IN_SHOP) && inBlockedDoorway()) {
		Debug::custom(name()) << "A shopkeeper seems to be blocking us, this must be a shop we don't know yet" << endl;
		_tentative_shop_door = Saiph::position();
		near_shop = IN_SHOP;
	}

	if (near_shop <= IN_SHOP)
		setShopping(true);
	if (near_shop > THRESHOLD)
		setShopping(false);

	if (_shopping && near_shop == THRESHOLD)
		dropPicks();

	/* FIXME:
	 * this currently bugs. she's marking SHOP_TILE as FLOOR
	 * and picks up items. why? */
	//	if (symbol == SHOP_TILE) {
	//		/* if we're standing on SHOP_TILE, check if we can see the shopkeeper.
	//		 * if we can't we probably killed him/her, and then we should remove the SHOP_TILE */
	//		bool shopkeeper_seen = false;
	//		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
	//			if (!m->second.shopkeeper || !m->second.visible)
	//				continue;
	//			shopkeeper_seen = true;
	//			break;
	//		}
	//		if (!shopkeeper_seen) {
	//			/* can't see any shopkeeper, make the tile FLOOR */
	//			saiph->levels[saiph->position.level].setDungeonSymbol(FLOOR);
	//			/* we should also look at floor so Loot gets a chance to
	//			 * pick up the item as the last time Loot checked, this
	//			 * tile was a SHOP_TILE */
	//			command = LOOK;
	//			priority = PRIORITY_CONTINUE_ACTION;
	//			/* return because there's no need to check for a shopkeeper again */
	//			return;
	//		}
	//	}

	if (World::level().tile().symbol() == SHOP_TILE) {
		unsigned int reach_any = UNREACHABLE;
		bool adj_shk = false;

		for (map<Point,int>::const_iterator ppt = World::level().symbols(OPEN_DOOR).begin(); ppt != World::level().symbols(OPEN_DOOR).end(); ++ppt)
			reach_any = min(reach_any, World::level().tile(ppt->first).cost());

		for (map<Point, Monster*>::const_iterator mit = World::level().monsters().begin(); mit != World::level().monsters().end(); ++mit) {
			if (Point::gridDistance(mit->first, Saiph::position()) == 1)
				adj_shk = true;
		}

		if (adj_shk && reach_any >= UNPASSABLE && World::turn() - _payed > 10) {
			World::setAction(new action::Pay(this, PRIORITY_PAY_FOR_ITEMS));
			Debug::custom(name()) << "Setting action to pay." << endl;
		}
	}

	if (World::level().tile().symbol() != FLOOR && World::level().tile().symbol() != UNKNOWN_TILE)
		return; // not standing on FLOOR or UNKNOWN_TILE, no shop here (or detected already)

	for (map<Point, Monster*>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
		if (!m->second->shopkeeper() || !m->second->visible())
			continue;

		/* figure out if we're in the same room as the shopkeeper */
		Point nw = Saiph::position();
		Point se = Saiph::position();

		/* find north wall */
		while ((World::level().tile(nw).symbol() == FLOOR || World::level().tile(nw).symbol() == UNKNOWN_TILE))
			nw.moveNorth();
		nw.moveSouth();

		/* find west wall */
		while ((World::level().tile(nw).symbol() == FLOOR || World::level().tile(nw).symbol() == UNKNOWN_TILE))
			nw.moveWest();
		nw.moveEast();

		/* find south wall */
		while ((World::level().tile(se).symbol() == FLOOR || World::level().tile(se).symbol() == UNKNOWN_TILE))
			se.moveSouth();
		se.moveNorth();

		/* find east wall */
		while ((World::level().tile(se).symbol() == FLOOR || World::level().tile(se).symbol() == UNKNOWN_TILE))
			se.moveEast();
		se.moveWest();

		if (m->first.row() < nw.row() || m->first.col() < nw.col() || m->first.row() > se.row() || m->first.col() > se.col())
			return; // we're not in the same room as the shopkeeper

		/* check that area assumed to be a shop does not contain VERTICAL_WALL, HORIZONTAL_WALL or ALTAR */
		Point p;
		for (p.row(nw.row()); p.row() <= se.row(); p.moveSouth()) {
			for (p.col(nw.col()); p.col() <= se.col(); p.moveEast()) {
				if (World::level().tile(p).symbol() == VERTICAL_WALL || World::level().tile(p).symbol() == HORIZONTAL_WALL || World::level().tile(p).symbol() == ALTAR)
					return; // detected too large area as shop
			}
		}

		Debug::custom(name()) << "Shop bounds are " << nw << " to " << se << endl;
		ShopDetected ev(nw, se);
		EventBus::broadcast(&ev);

		/* mark all tiles within boundaries as SHOP_TILE */
		for (p.row(nw.row()); p.row() <= se.row(); p.moveSouth()) {
			for (p.col(nw.col()); p.col() <= se.col(); p.moveEast())
				World::level().setDungeonSymbol(p, SHOP_TILE);
		}
	}
}

void Shop::actionFailed() {
	// Try something else for a while
	_payed = World::turn();
}

