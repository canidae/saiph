#include "Analyzers/Vault.h"

#include "Debug.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Action.h"
#include "Actions/Answer.h"
#include "Actions/DropGold.h"
#include "Actions/Look.h"
#include "Actions/Move.h"
#include "Actions/Rest.h"
#include "Data/Monster.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Vault::Vault() : Analyzer("Vault"), _answering_guard(false), _dropping_gold(false), _following_guard(false) {
}

/* methods */
void Vault::findNearestUnstepped(unsigned& best_cost, Point& target, const map<Point, int>& list) {
	for (map<Point, int>::const_iterator w = list.begin(); w != list.end(); ++w) {
		Tile& t = World::level().tile(w->first);
		if (t.search() == TILE_FULLY_SEARCHED) continue;
		if (t.cost() >= best_cost) continue;
		best_cost = t.cost();
		target = w->first;
	}
}

void Vault::parseMessages(const string& messages) {
	if (World::question() && messages.find(VAULT_MESSAGE_STRANGER) != string::npos) {
		/* guard asking who we are */
		/* if we got some means of getting out (teleportitis, wand of teleport, scroll of teleport, pick-axe),
		 * then claim we're Croesus */
		if (Saiph::intrinsics() & PROPERTY_TELEPORT) {
			World::setAction(static_cast<action::Action*> (new action::Answer(this, VAULT_CROESUS)));
			Debug::notice() << "Not dropping gold since we can tele out" << endl;
			return;
		}
		/* otherwise, tell our real name */
		World::setAction(static_cast<action::Action*> (new action::Answer(this, "saiph\n")));
		Debug::notice() << "Oops, we're stuck in a vault." << endl;
		_answering_guard = true;
	} else if (_dropping_gold) {
		/* drop gold */
		Debug::notice() << "Going to drop gold for the guard." << endl;
		World::setAction(static_cast<action::Action*> (new action::DropGold(this, 999)));
	} else if (messages.find(VAULT_MESSAGE_DISAPPEAR) != string::npos) {
		/* guard is gone, stop following */
		Debug::notice() << "No longer following guard." << endl;
		_following_guard = false;
	} else if (_following_guard) {
		/* follow the guard out */
		/* this is fairly tricky.
		 * general idea:
		 * if we're next to guard, rest.
		 * if we can see guard, move towards guard.
		 * otherwise, move towards the nearest unstepped corridor */
		/* TODO: If we get caught in a vault more than once, the corridor might not be unstepped! */
		Point target = Saiph::position();
		bool have_target;
		for (map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
			if (m->second.data() == NULL || m->second.data()->name() != "guard")
				continue;
			if (m->second.attitude() != FRIENDLY) {
				// Oops.  Well, we need to fight now.
				_following_guard = false;
				Debug::notice() << "No longer following angry guard." << endl;
				return;
			}
			_last_saw_guard_turn = World::turn();
			target = m->first;
			have_target = true;
			break;
		}

		if (_last_saw_guard_turn + 200 < int(World::turn())) {
			// Give up.
			Debug::notice() << "We haven't seen the guard for a while, assuming we lost him." << endl;
			_following_guard = false;
			return;
		}

		if (!have_target) {
			unsigned best_cost = UNPASSABLE;
			findNearestUnstepped(best_cost, target, World::level().symbols((unsigned char) CORRIDOR));
			findNearestUnstepped(best_cost, target, World::level().symbols((unsigned char) FLOOR));
		}

		Debug::notice() << "Going to follow guard." << endl;
		Tile& tile = World::shortestPath(target);
		if (tile.cost() >= UNPASSABLE)
			World::setAction(static_cast<action::Action*> (new action::Rest(this, 999)));
		else
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, 999, false)));
		return;
	}
}

void Vault::actionCompleted(const string&) {
	if (_answering_guard) {
		_answering_guard = false;
		_dropping_gold = true;
	} else if (_dropping_gold) {
		_dropping_gold = false;
		_following_guard = true;
		_last_saw_guard_turn = World::turn();
	}
}
