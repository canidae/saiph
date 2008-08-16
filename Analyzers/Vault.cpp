#include "Vault.h"

/* constructors */
Vault::Vault(Saiph *saiph) : Analyzer("Vault"), saiph(saiph), drop_gold(false) {
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
		command = NEXT_PAGE;
		priority = PRIORITY_CLOSE_ITEM_LIST;
	} else if (drop_gold) {
		/* move next to the guard before dropping */
		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
			if (m->second.symbol != '@' || m->second.color != BLUE || !m->second.visible)
				continue;
			int distance = max(abs(m->first.row - saiph->position.row), abs(m->first.col - saiph->position.col));
			if (distance <= 1)
				continue;
			/* we must move */
			int moves = 0;
			unsigned char move = saiph->shortestPath(m->first, true, &moves);
			if (move != ILLEGAL_MOVE) {
				command = move;
				priority = PRIORITY_CONTINUE_ACTION;
				return;
			}
		}
		/* haven't got the drop menu yet, ask for it */
		command = DROP;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}
