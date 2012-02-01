#include "Analyzers/Rub.h"

#include <string>
#include "Request.h"
#include "Saiph.h"
#include "World.h"

using namespace analyzer;
using namespace std;

Rub::Rub(Saiph* saiph) : Analyzer("Rub"), saiph(saiph), magic_lamp_key(ILLEGAL_ITEM) {
}

void Rub::analyze() {
	if (magic_lamp_key == ILLEGAL_ITEM)
		return;
	priority = PRIORITY_RUB_MAGIC_LAMP;
	command = RUB;
}

void Rub::parseMessages(const string& messages) {
	if (saiph->inventory_changed)
		findMagicLamp();
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_RUB) != string::npos) {
		command = magic_lamp_key;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

/* private methods */
void Rub::findMagicLamp() {
	map<unsigned char, Item>::iterator i = saiph->inventory.find(magic_lamp_key);
	if (i != saiph->inventory.end() && isMagicLamp(i->second))
		return;
	for (i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (isMagicLamp(i->second)) {
			magic_lamp_key = i->first;
			return;
		}
	}
	magic_lamp_key = ILLEGAL_ITEM;
}

bool Rub::isMagicLamp(const Item& item) {
	if (item.beatitude == CURSED || item.beatitude == UNCURSED || item.name != "magic lamp")
		return false;
	if (item.beatitude == BEATITUDE_UNKNOWN) {
		/* must beatify lamp first */
		req.request = REQUEST_BEATIFY_ITEMS;
		saiph->request(req);
		return false;
	}
	return true;
}
