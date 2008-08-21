#include "Vault.h"

/* constructors */
Vault::Vault(Saiph *saiph) : Analyzer("Vault"), saiph(saiph), drop_gold(false), look_at_ground(false), follow_guard(false) {
}

/* methods */
void Vault::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(VAULT_MESSAGE_STRANGER, 0) != string::npos) {
		/* guard asking who we are */
		/* if we got some means of getting out (teleportitis, wand of teleport, scroll of teleport, pick-axe),
		 * then claim we're Croesus */
		if (saiph->world->player.teleportitis) {
			command = VAULT_CROESUS;
			priority = PRIORITY_CONTINUE_ACTION;
			return;
		}
		/* otherwise, tell our real name */
		command = "saiph\n";
		priority = PRIORITY_CONTINUE_ACTION;
		drop_gold = true;
	} else if (saiph->got_drop_menu && drop_gold) {
		/* drop our gold */
		for (map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
			if (d->second.name != "gold piece")
				continue;
			command = d->first;
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
		/* we've probably selected our gold if we're here */
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_ITEM_PAGE;
		look_at_ground = true;
	} else if (drop_gold && !look_at_ground) {
		/* bring up drop menu */
		command = DROP;
		priority = VAULT_GO_OUT_PRIORITY;
	} else if (look_at_ground) {
		/* we'll look at ground after dropping the gold.
		 * this makes us aware of the stash,
		 * and the loot analyzer won't "visit" the stash after we move */
		command = LOOK;
		priority = PRIORITY_LOOK;
		drop_gold = false;
		look_at_ground = false;
		follow_guard = true;
	} else if (messages.find(VAULT_MESSAGE_DISAPPEAR, 0) != string::npos) {
		/* guard is gone, stop following */
		follow_guard = false;
	} else if (follow_guard) {
		/* follow the guard out */
		/* this is really tricky.
		 * general idea:
		 * if we're next to guard, rest.
		 * if we can see guard, move towards guard.
		 * otherwise, hope the explore analyzer lead us out */
		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
			if (m->second.symbol != '@' || m->second.color != BLUE || !m->second.visible)
				continue;
			int moves = 0;
			unsigned char move = saiph->shortestPath(m->first, true, &moves);
			if (move != ILLEGAL_MOVE) {
				if (moves == 1)
					command = REST;
				else
					command = move;
				priority = VAULT_GO_OUT_PRIORITY;
				return;
			}
		}
	}
}
