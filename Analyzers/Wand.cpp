#include "Wand.h"
#include "../Saiph.h"
#include "../Globals.h"
#include "../World.h"
#include "../Debug.h"

using namespace std;

/* constructors/destructor */
Wand::Wand(Saiph *saiph) : Analyzer("Wand"), saiph(saiph), wand_key(0), processing(false) {
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
}

/* methods */
void Wand::analyze() {
	if (priority >= PRIORITY_WAND_ENGRAVE_ID || wand_key == 0 || saiph->world->player.blind || saiph->world->player.levitating)
		return;
	if (saiph->inventory_changed && wand_key != 0) {//make sure it's still there and still a wand
		map<unsigned char, Item>::iterator i = saiph->inventory.find(wand_key);
		if (i == saiph->inventory.end() || !isUnidentifiedWand(i->second)) {
			wand_key = 0;
			processing = false;
		}
	}
	if (wand_key != 0 && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == FLOOR) {
		command = ENGRAVE;
		priority = PRIORITY_WAND_ENGRAVE_ID;
		Debug::notice() << "[Wand       ] Asking to engrave-ID " << saiph->inventory[wand_key].name << endl;
	}
}

void Wand::parseMessages(const string &messages) {
	if (wand_key == 0) {
		findUnidentifiedWands();
		return;
	} else {
		map<unsigned char, Item>::iterator i = saiph->inventory.find(wand_key);
		if (i == saiph->inventory.end() || !isUnidentifiedWand(i->second)) {
			wand_key = 0;
			processing = false;
		}
	}
	if (messages.find(MESSAGE_ENGRAVE_WITH) != string::npos) {
		command = wand_key;
		processing = true;
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	}
	req.request = REQUEST_CALL_ITEM;
	req.key = wand_key;
	req.data = "";
	//some of the wands auto-ID, so we only need to look for the other ones.
	if (messages.find(WAND_VANISHER_MESSAGE) != string::npos)
		req.data = WAND_VANISHER_NAME; //if we ever use BoHs, we need to see this
	if (messages.find(WAND_COLD_MESSAGE) != string::npos)
		req.data = "wand of cold";
	if (messages.find(WAND_SLEEP_DEATH_MESSAGE) != string::npos)
		req.data = "wand of sleep";
	if (messages.find(WAND_MAGIC_MISSILE_MESSAGE) != string::npos)
		req.data = "wand of magic missile";
	if (messages.find(WAND_POLYMORPH_MESSAGE) != string::npos)
		req.data = "wand of polymorph";
	if (messages.find(WAND_SLOW_MONSTER_MESSAGE) != string::npos)
		req.data = "wand of slow monster";
	if (messages.find(WAND_SPEED_MONSTER_MESSAGE) != string::npos)
		req.data = "wand of speed monster";
	if (messages.find(WAND_STRIKING_MESSAGE) != string::npos)
		req.data = "wand of striking";
	if (req.data != "") {
		Debug::notice() << "[Wand       ] Calling wand " << req.data << endl;
		saiph->request(req);
		processing = false;
		wand_key = 0;
	} else if (processing) {
		//we engraved, but we didn't see a message, so it had no effect
		Debug::notice() << "[Wand       ] Wand had no engrave message; naming as such" << endl;
		req.data = WAND_NO_EFFECT_NAME;
		saiph->request(req);
		processing = false;
		wand_key = 0;
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
	for (vector<string>::size_type j = 0; j < wand_appearances.size(); j++)
		if (i.name == wand_appearances[j] + " wand")
			return true;
	return false;
}

void Wand::complete() {
	req.request = REQUEST_DIRTY_INVENTORY;
	saiph->request(req);
}
