#include "Weapon.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Weapon::Weapon(Saiph *saiph) : Analyzer("Weapon"), saiph(saiph), wield_weapon(false) {
}

/* methods */
void Weapon::parseMessages(const string &messages) {
	if (saiph->world->question && !command2.empty() && messages.find(WEAPON_WHAT_TO_WIELD, 0) != string::npos) {
		/* wield a weapon */
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		wield_weapon = false;
	} else if (saiph->inventory_changed || wield_weapon) {
		wieldWeapon();
	}
}

bool Weapon::request(const Request &request) {
	if (request.request == REQUEST_WEAPON_WIELD) {
		/* player wish to wield this weapon */
		WeaponData wd;
		wd.beatitude = request.beatitude;
		wd.priority = request.priority;
		wd.amount = request.value;
		wd.keep = request.sustain;
		wd.name = request.data;
		weapons.push_back(wd);
		/* tell loot analyzer to pick up this weapon too */
		req.request = REQUEST_ITEM_PICKUP;
		req.value = request.value;
		req.beatitude = request.beatitude | BEATITUDE_UNKNOWN;
		req.data = request.data;
		saiph->request(req);
		return true;
	}
	return false;
}

/* private methods */
void Weapon::wieldWeapon() {
	/* check that we're (still) wielding our preferred weapon */
	unsigned char wielded = 0;
	unsigned char best_key = 0;
	int best_priority = INT_MIN;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional.find("weapon in ", 0) == 0 || i->second.additional == "wielded") {
			wielded = i->first;
			if (i->second.beatitude == CURSED) {
				/* weapon is cursed, we can't wield another weapon */
				best_key = 0;
				break;
			}
		}
		for (vector<WeaponData>::iterator w = weapons.begin(); w != weapons.end(); ++w) {
			if (w->name != i->second.name && w->name != i->second.named) {
				continue;
			} else if (i->second.beatitude == BEATITUDE_UNKNOWN) {
				/* weapon with unknown beatitude, request it beatified */
				req.request = REQUEST_BEATIFY_ITEMS;
				saiph->request(req);
			}
			if ((w->beatitude & i->second.beatitude) == 0)
				continue;
			else if (w->priority + i->second.enchantment - i->second.damage <= best_priority)
				continue;
			best_key = i->first;
			best_priority = w->priority + i->second.enchantment - i->second.damage;
		}
	}
	if (best_key == 0 || (wielded == best_key)) {
		/* wielding best weapon or got no weapon to wield */
		command.clear();
		command2.clear();
		priority = ILLEGAL_PRIORITY;
		wield_weapon = false;
		return;
	}
	wield_weapon = true;
	command = WIELD;
	command2 = best_key;
	priority = PRIORITY_WEAPON_WIELD;

	/* tell Loot to drop unwanted weapons */
	for (vector<WeaponData>::iterator w = weapons.begin(); w != weapons.end(); ++w) {
		req.request = REQUEST_ITEM_PICKUP;
		req.beatitude = w->beatitude | BEATITUDE_UNKNOWN;
		req.data = w->name;
		if (w->keep || w->priority + 7 >= best_priority) {
			/* we [still] want this weapon.
			 * in case we lost a good weapon and now wield a less good
			 * weapon we'll need to tell Loot to pick up this weapon */
			req.value = w->amount;
		} else {
			/* we don't want to keep this weapon and it'll never
			 * be better than the weapon we currently got */
			req.value = 0;
		}
		saiph->request(req);
	}
}
