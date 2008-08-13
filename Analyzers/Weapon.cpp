#include "Weapon.h"

/* constructors */
Weapon::Weapon(Saiph *saiph) : Analyzer("Weapon"), saiph(saiph) {
}

/* methods */
void Weapon::analyze() {
	wieldWeapon();
}

void Weapon::parseMessages(const string &messages) {
	if (saiph->world->question && !command2.empty() && messages.find(WEAPON_WHAT_TO_WIELD, 0) != string::npos) {
		/* wield a weapon */
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (command == WIELD) {
		/* in case we didn't get to wield the weapon */
		priority = WEAPON_WIELD_PRIORITY;
	}
}

bool Weapon::request(const Request &request) {
	if (request.request == REQUEST_WEAPON_WIELD) {
		/* player wish to wield this weapon */
		WieldWeapon ww;
		ww.beatitude = request.status;
		ww.name = request.data;
		wield.push_back(ww);
		return true;
	}
	return false;
}

/* private methods */
void Weapon::wieldWeapon() {
	/* inventory was just listed */
	/* check that we're (still) wielding our preferred weapon */
	unsigned char wielded = 0;
	unsigned char best_key = 0;
	int best_weapon = INT_MAX;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional == "weapon in hand") {
			wielded = i->first;
			if (i->second.beatitude == CURSED) {
				/* weapon is cursed, we can't wield another weapon */
				best_key = 0;
				break;
			}
		}
		for (vector<WieldWeapon>::size_type w = 0; w < wield.size(); ++w) {
			if (wield[w].name != i->second.name || (int) w >= best_weapon)
				continue;
			if ((wield[w].beatitude & i->second.beatitude) == 0)
				continue;
			best_key = i->first;
			best_weapon = w;
		}
	}
	if (best_key == 0 || (wielded != 0 && saiph->inventory[wielded].name == saiph->inventory[best_key].name))
		return; // wielding best weapon or got no weapon to wield
	command = WIELD;
	command2 = best_key;
	priority = WEAPON_WIELD_PRIORITY;
}
