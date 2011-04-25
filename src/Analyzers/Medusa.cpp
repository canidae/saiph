#include "Analyzers/Medusa.h"

#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Move.h"
#include "Actions/Search.h"
#include "Actions/PutOn.h"
#include "Actions/Remove.h"
#include "Actions/ApplyInDirection.h"
#include "Events/WantItems.h"
#include "Events/GotDiggingTool.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Medusa::Medusa() : Analyzer("Medusa"), _medusa_level(-1), _fighting_medusa(false), _medusa_killed(false), _pick_key(ILLEGAL_ITEM) {
	/* register events */
	EventBus::registerEvent(WantItems::ID, this);
	EventBus::registerEvent(GotDiggingTool::ID, this);
}

/* methods */
void Medusa::analyze() {
	// Try to recognize Medusa
	if (_medusa_level < 0 && World::level().depth() >= 21 && World::level().depth() <= 28) { // && not Castle
		if (World::level().symbols(WATER).size() >= 50) {
			Debug::custom(name()) << "Level is very wet, likely Medusa" << endl;
			_medusa_level = Saiph::position().level();
		} else {
			int pools = 0;
			for (int row = 15; row <= 17; ++row) {
				if (World::level().tile(Point(row, 7)).symbol() == WATER)
					pools++;
			}
			if (pools == 3) {
				Debug::custom(name()) << "Recognized Medusa-2 pool pattern" << endl;
				_medusa_level = Saiph::position().level();
			}
		}
	}

	if (Saiph::position().level() == _medusa_level && Saiph::position().col() < 20 && _pick_key != ILLEGAL_ITEM) {
		Tile t = World::shortestPath(Point(11,4)); // a nice digging point
		if (t.direction() == NOWHERE) {
			World::setAction(new action::ApplyInDirection(this, _pick_key, DOWN, PRIORITY_MEDUSA_DIG_DOWN));
		} else if (t.cost() <= UNPASSABLE) {
			World::setAction(new action::Move(this, t, action::Move::calculatePriority(PRIORITY_MEDUSA_DIG_DOWN, t.cost())));
		}
	}

	if (_medusa_level >= 0 && World::level().depth() == World::level(_medusa_level).depth() + 1 && !_medusa_killed && !World::level().symbols(STAIRS_UP).empty()) {
		Tile stairs = World::shortestPath(World::level().symbols(STAIRS_UP).begin()->first);
		unsigned char blinder_key = ILLEGAL_ITEM;
		for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
			if (i->second.name() == "blindfold" || i->second.name() == "towel")
				blinder_key = i->first;
		}
		if (stairs.direction() != NOWHERE) {
			World::setAction(new action::Move(this, stairs, PRIORITY_MEDUSA_KILL_MEDUSA));
		} else if (!Saiph::blind() && blinder_key != ILLEGAL_ITEM) {
			World::setAction(new action::PutOn(this, blinder_key, PRIORITY_MEDUSA_KILL_MEDUSA));
		} else { /* already blind, or no blindfold (we'll probably die...) */
			_fighting_medusa = true; // TODO move this to actionCompleted somehow
			_medusa_done_turn = World::turn() + 20;
			stairs.direction(UP);
			World::setAction(new action::Move(this, stairs, PRIORITY_MEDUSA_KILL_MEDUSA));
		}
	}

	if (Inventory::keyForSlot(SLOT_EYES) != ILLEGAL_ITEM && _medusa_killed) {
		World::setAction(new action::Remove(this, Inventory::keyForSlot(SLOT_EYES), PRIORITY_MEDUSA_KILL_MEDUSA));
	}

	if (Saiph::position().level() == _medusa_level && _fighting_medusa) {
		for (map<Point,Monster>::const_iterator i = World::level().monsters().begin(); i != World::level().monsters().end(); ++i) {
			if (i->second.visible() && (i->second.symbol() == 'I' || (i->second.symbol() == '@' && i->second.color() == BOLD_GREEN)))
				_medusa_done_turn = World::turn() + 20;
		}
		if (unsigned(World::turn()) > _medusa_done_turn) {
			_medusa_killed = true;
			_fighting_medusa = false;
			Debug::custom(name()) << "Medusa seems dead." << endl;
		}
		World::setAction(new action::Search(this, PRIORITY_MEDUSA_KILL_MEDUSA));
	}
}

void Medusa::onEvent(event::Event* const event) {
	if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.name() == "blindfold" || i->second.name() == "towel") {
				i->second.want(i->second.count());
			}
		}
	} else if (event->id() == GotDiggingTool::ID) {
		_pick_key = (static_cast<GotDiggingTool*> (event))->key();
	}
}

/* private methods */
