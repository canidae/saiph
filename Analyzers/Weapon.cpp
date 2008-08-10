#include "Weapon.h"

/* constructors */
Weapon::Weapon(Saiph *saiph) : Analyzer("Weapon"), saiph(saiph) {
}

/* methods */
void Weapon::analyze() {
	if (saiph->inventory_updated) {
		/* inventory is changed somehow */
		/* check that we're (still) wielding our preferred weapon */
		unsigned char wielded = 0;
		unsigned char best_key = 0;
		int best_weapon = INT_MAX;
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.additional == "weapon in hand")
				wielded = i->first;
			for (vector<string>::size_type w = 0; w < wield_weapon.size(); ++w) {
				if (wield_weapon[w] == i->second.name && (int) w < best_weapon) {
					best_key = i->first;
					best_weapon = w;
				}
			}
		}
		if (wielded != 0 && wielded == best_key)
			return; // wielding best weapon
		/* we're not wielding the best weapon, or we're not wielding any weapon */
		if (best_key == 0)
			return; // we don't have any weapons we'd like to wield
		command = WIELD;
		command2 = best_key;
		priority = WEAPON_WIELD_PRIORITY;
		return;
	} else if (saiph->on_ground != NULL) {
		/* items on ground, check if we want any */
		req.request = REQUEST_LOOT_STASH;
		req.priority = WEAPON_PICKUP_PRIORITY;
		for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
			int wanted = weaponWanted(*i);
			if (wanted == 0)
				continue;
			saiph->request(req);
			return;
		}
	}
}

void Weapon::parseMessages(const string &messages) {
	if (saiph->world->question && !command2.empty() && messages.find(WEAPON_WHAT_TO_WIELD, 0) != string::npos) {
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->pickup.size() > 0) {
		/* pick up weapons according to our rules */
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			int wanted = weaponWanted(p->second);
			if (wanted == 0)
				continue;
			if (wanted <= p->second.count) {
				/* pick them all up */
				command = p->first;
			} else {
				/* only pick some up */
				stringstream tmp;
				tmp << wanted << p->first;
				command = tmp.str();
			}
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
	}
}

bool Weapon::request(const Request &request) {
	if (request.request == REQUEST_WEAPON_GROUP_CREATE) {
		/* set total amount of weapons in the given group */
		int amount = atoi(request.data.c_str());
		if (amount <= 0)
			return false;
		weapon_group_total[request.value] = amount;
		return true;
	} else if (request.request == REQUEST_WEAPON_GROUP_ADD) {
		/* add weapon to group */
		if (weapon_group_total.find(request.value) == weapon_group_total.end())
			return false; // group does not exist
		if (weapon.find(request.data) == weapon.end())
			weapon[request.data] = 0; // we need to add an entry about this weapon
		weapon_accept_beatitude[request.data] = request.status;
		weapon_group[request.value].push_back(request.data);
		return true;
	} else if (request.request == REQUEST_WEAPON_WIELD) {
		/* player wish to wield this weapon */
		if (weapon.find(request.data) == weapon.end() || weapon[request.data] <= 0)
			weapon[request.data] = 1;
		weapon_accept_beatitude[request.data] = request.status;
		wield_weapon.push_back(request.data);
		return true;
	} else if (request.request == REQUEST_WEAPON_PICKUP) {
		/* pick up weapons that are not part of a group nor should be wielded.
		 * for example, darts */
		weapon_accept_beatitude[request.data] = request.status;
		weapon[request.data] = request.value;
		return true;
	}
	return false;
}

/* private methods */
int Weapon::weaponWanted(const Item &item) {
	/* return how many we want of this item */
	map<string, int>::iterator w = weapon.find(item.name);
	if (w == weapon.end())
		return 0; // item is not in our weapon list
	/* groups */
	for (map<int, vector<string> >::iterator wg = weapon_group.begin(); wg != weapon_group.end(); ++wg) {
		/* figure out how many weapons we got in this group */
		int count = 0;
		int weapon_in_group = false;
		for (vector<string>::iterator wgw = wg->second.begin(); wgw != wg->second.end(); ++wgw) {
			for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
				if (i->second.name != *wgw)
					continue;
				count += i->second.count;
			}
			if (*wgw == item.name) {
				/* weapon is in group.
				 * we'll also break here so she'll pick up better weapons in group,
				 * even if that means she'll exceed the limit.
				 * otherwise she won't pick up eg. elven daggers when she's full on orcish daggers */
				weapon_in_group = true;
				break;
			}
		}
		if (!weapon_in_group || count >= weapon_group_total[wg->first])
			continue;
		if (count + item.count <= weapon_group_total[wg->first]) {
			/* we want all of them */
			return item.count;
		} else {
			/* we only want some of them */
			return weapon_group_total[wg->first] - count - item.count;
		}
	}
	/* solitary weapons */
	if (w->second <= 0)
		return 0; // we don't desire this weapon
	if (item.beatitude & weapon_accept_beatitude[item.name] == 0)
		return 0; // item does not have a beatitude we'll accept
	/* figure out how many we got of this item already */
	int count = 0;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.name != item.name)
			continue;
		count += i->second.count;
	}
	if (count >= w->second)
		return 0; // got enough of this weapon already
	if (count + item.count <= w->second) {
		/* we want all of them */
		return item.count;
	} else {
		/* we only want some of them */
		return w->second - count - item.count;
	}
}
