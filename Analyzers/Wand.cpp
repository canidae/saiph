#include <utility>
#include "Wand.h"
#include "../Saiph.h"
#include "../Globals.h"
#include "../World.h"
#include "../Debug.h"

using namespace std;

/* constructors/destructor */
Wand::Wand(Saiph *saiph) : Analyzer("Wand"), saiph(saiph), wand_key(0), state(WAND_STATE_INIT) {
	wand_appearances.push_back("glass");
	wand_appearances.push_back("balsa");
	wand_appearances.push_back("crystal");
	wand_appearances.push_back("maple");
	wand_appearances.push_back("pine");
	wand_appearances.push_back("oak");
	wand_appearances.push_back("ebony");
	wand_appearances.push_back("marble");
	wand_appearances.push_back("tin");
	wand_appearances.push_back("brass");
	wand_appearances.push_back("copper");
	wand_appearances.push_back("silver");
	wand_appearances.push_back("platinum");
	wand_appearances.push_back("iridium");
	wand_appearances.push_back("zinc");
	wand_appearances.push_back("aluminum");
	wand_appearances.push_back("uranium");
	wand_appearances.push_back("iron");
	wand_appearances.push_back("steel");
	wand_appearances.push_back("hexagonal");
	wand_appearances.push_back("short");
	wand_appearances.push_back("runed");
	wand_appearances.push_back("curved");
	wand_appearances.push_back("long");
	wand_appearances.push_back("forked");
	wand_appearances.push_back("spiked");
	wand_appearances.push_back("jeweled");
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_VANISHER_MESSAGE, WAND_VANISHER_NAME));
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_COLD_MESSAGE, "wand of cold"));
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_SLEEP_DEATH_MESSAGE, WAND_SLEEP_DEATH_NAME));
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_MAGIC_MISSILE_MESSAGE, "wand of magic missile"));
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_POLYMORPH_MESSAGE, "wand of polymorph"));
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_SLOW_MONSTER_MESSAGE, "wand of slow monster"));
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_SPEED_MONSTER_MESSAGE, "wand of speed monster"));
	wand_engrave_messages.push_back(make_pair<string, string>(WAND_STRIKING_MESSAGE, "wand of striking"));
}

/* methods */
void Wand::analyze() {

}

void Wand::parseMessages(const string &messages) {
	if (state == WAND_STATE_INIT) {
		if (saiph->inventory_changed) {
			if (wand_key != 0 && !isUnidentifiedWand(wand_key))
				wand_key = 0;
			if (wand_key == 0)
				findUnidentifiedWands();
		}
		if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != FLOOR ||
				saiph->world->player.levitating || saiph->world->player.blind)
			return;
		if (wand_key != 0)
			state = WAND_STATE_DUST_X;
	} //no else here.  deliberate.
	if (state == WAND_STATE_DUST_X) {
		if (messages.find(MESSAGE_ENGRAVE_ADD, 0) != string::npos) {
			command = YES;
			priority = PRIORITY_CONTINUE_ACTION;
		} else if (messages.find(MESSAGE_ENGRAVE_WITH, 0) != string::npos) {
			command = HANDS;
			priority = PRIORITY_CONTINUE_ACTION;
		} else if (messages.find(MESSAGE_ENGRAVE_DUST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_DUST_ADD, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST_ADD, 0) != string::npos) {
			command = "x\n";
			priority = PRIORITY_CONTINUE_ACTION;
			state = WAND_STATE_WANTS_LOOK;
		} else {
			/* don't engrave if we're now where we can't or shouldn't */
			if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != FLOOR ||
					saiph->world->player.levitating || saiph->world->player.blind)
				return;
			command = ENGRAVE;
			priority = PRIORITY_WAND_ENGRAVE_ID;
		}
	} else if (state == WAND_STATE_WANTS_LOOK) {
		//we consumed a move before this, ensure we didn't get interrupted
		if (wand_key == 0 || !isUnidentifiedWand(wand_key)) {
			wand_key = 0;
			state = WAND_STATE_INIT;
		}
		command = LOOK;
		priority = PRIORITY_LOOK;
		state = WAND_STATE_CONFIRM_LOOK;
	} else if (state == WAND_STATE_CONFIRM_LOOK) {
		if (messages.find(MESSAGE_TEXT_DUSTED, 0) != string::npos ||
			messages.find(MESSAGE_TEXT_FROSTED, 0) != string::npos)
			state = WAND_STATE_ENGRAVING;
		else
			state = WAND_STATE_INIT;
	} else if (state == WAND_STATE_ENGRAVING) {
		if (messages.find(MESSAGE_ENGRAVE_ADD, 0) != string::npos) {
			command = YES;
		} else if (messages.find(WAND_DIGGING_MESSAGE, 0) != string::npos ||
				messages.find(WAND_LIGHTNING_MESSAGE, 0) != string::npos ||
				messages.find(WAND_FIRE_MESSAGE, 0) != string::npos) {
			command = ESCAPE;
			req.request = REQUEST_DIRTY_INVENTORY;
			saiph->request(req);
			wand_key = 0;
			state = WAND_STATE_INIT;
		} else if (messages.find(MESSAGE_ENGRAVE_DUST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_DUST_ADD, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST_ADD, 0) != string::npos) {
			command = "x\n";
			state = WAND_STATE_READY_TO_NAME;
		} else if (messages.find(MESSAGE_ENGRAVE_WITH, 0) != string::npos) {
			priority = PRIORITY_CONTINUE_ACTION;
			command = wand_key;
		} else {
			/* make sure we didn't get interrupted */
			if (wand_key == 0 || !isUnidentifiedWand(wand_key) ||
					saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != FLOOR ||
					saiph->world->player.levitating || saiph->world->player.blind) {
				wand_key = 0;
				state = WAND_STATE_INIT;
				return;
			}
			command = ENGRAVE;
		}
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (state == WAND_STATE_READY_TO_NAME) {
		string name = WAND_NO_EFFECT_NAME;
		for (vector<pair<string, string> >::size_type i = 0; i < wand_engrave_messages.size(); i++)
			if (messages.find(wand_engrave_messages[i].first, 0) != string::npos)
				name = wand_engrave_messages[i].second;
		req.request = REQUEST_CALL_ITEM;
		req.data = name;
		req.key = wand_key;
		saiph->request(req);
		state = WAND_STATE_WANT_DIRTY_INVENTORY;
	} else if (state == WAND_STATE_WANT_DIRTY_INVENTORY) {
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		state = WAND_STATE_INIT;
	}
}

void Wand::findUnidentifiedWands() {
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); i++)
		if (isUnidentifiedWand(i->second)) {
			wand_key = i->first;
			return;
		}
}

bool Wand::isUnidentifiedWand(const Item& i) {
	string::size_type split = i.name.find(" wand", 0);
	if (split == string::npos)
		return false;
	string appearance = i.name.substr(0, split);
	for (vector<string>::size_type j = 0; j < wand_appearances.size(); j++)
		if (appearance == wand_appearances[j])
			return true;
	return false;
}

bool Wand::isUnidentifiedWand(const unsigned char& c) {
	map<unsigned char, Item>::iterator iter = saiph->inventory.find(c);
	return iter != saiph->inventory.end() && isUnidentifiedWand(iter->second);
}
