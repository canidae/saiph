#include <string>
#include "Rub.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Globals.h"

using namespace std;

Rub::Rub(Saiph *saiph) : Analyzer("Rub"), saiph(saiph), magic_lamp_key(0) {
}

void Rub::parseMessages(const string &messages) {
	if (saiph->inventory_changed)
		lookForMagicLamp();
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_RUB) != string::npos) {
		command = magic_lamp_key;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

void Rub::analyze() {
	if (priority >= PRIORITY_RUB_MAGIC_LAMP || magic_lamp_key == 0)
		return;
	priority = PRIORITY_RUB_MAGIC_LAMP;
	command = RUB;
}

void Rub::lookForMagicLamp() {
	if (magic_lamp_key != 0) {
		map<unsigned char, Item>::iterator i = saiph->inventory.find(magic_lamp_key);
		if (i != saiph->inventory.end() && i->second.name == "magic lamp")
			return;
		/* magic lamp we had is gone, try to find a new one */
		magic_lamp_key = 0;
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name == "magic lamp" && i->second.beatitude == BLESSED)
				magic_lamp_key = i->first;
		}
	}
}
