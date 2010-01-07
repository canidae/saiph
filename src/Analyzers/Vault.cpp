#include "Analyzers/Vault.h"

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
Vault::Vault() : Analyzer("Vault"), _answering_guard(false), _dropping_gold(false), _looking_at_ground(false), _following_guard(false) {
}

/* methods */
void Vault::parseMessages(const string& messages) {
	if (World::question() && messages.find(VAULT_MESSAGE_STRANGER) != string::npos) {
		/* guard asking who we are */
		/* if we got some means of getting out (teleportitis, wand of teleport, scroll of teleport, pick-axe),
		 * then claim we're Croesus */
		if (Saiph::intrinsics() & PROPERTY_TELEPORT) {
			World::setAction(static_cast<action::Action*> (new action::Answer(this, VAULT_CROESUS)));
			return;
		}
		/* otherwise, tell our real name */
		World::setAction(static_cast<action::Action*> (new action::Answer(this, "saiph\n")));
		_answering_guard = true;
	} else if (_dropping_gold) {
		/* drop gold */
		World::setAction(static_cast<action::Action*> (new action::DropGold(this, 999)));
	} else if (_looking_at_ground) {
		/* XXX: look at ground to prevent us from picking up the gold again */
		World::setAction(static_cast<action::Action*> (new action::Look(this)));
	} else if (messages.find(VAULT_MESSAGE_DISAPPEAR) != string::npos) {
		/* guard is gone, stop following */
		_following_guard = false;
	} else if (_following_guard) {
		/* follow the guard out */
		/* this is fairly tricky.
		 * general idea:
		 * if we're next to guard, rest.
		 * if we can see guard, move towards guard.
		 * XXX: otherwise, hope the explore analyzer lead us out */
		for (map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
			if (m->second.data() == NULL || m->second.data()->name() != "guard")
				continue;
			Tile& tile = World::shortestPath(m->first);
			if (tile.cost() == UNREACHABLE)
				continue;
			if (tile.distance() == 1)
				World::setAction(static_cast<action::Action*> (new action::Rest(this, 999)));
			else
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile, 999, false)));
			return;
		}
	}
}

void Vault::actionCompleted() {
	if (_answering_guard) {
		_answering_guard = false;
		_dropping_gold = true;
	} else if (_dropping_gold) {
		_dropping_gold = false;
		_looking_at_ground = true;
	} else if (_looking_at_ground) {
		_looking_at_ground = false;
		_following_guard = true;
	}
}
