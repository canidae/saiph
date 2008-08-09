#include "Weapon.h"

/* constructors */
Weapon::Weapon(Saiph *saiph) : Analyzer("Weapon"), saiph(saiph) {
	/* daggers */
	weapon["orcish dagger"] = 0;
	weapon["dagger"] = 0;
	weapon["silver dagger"] = 0;
	weapon["athame"] = 0;
	weapon["elven dagger"] = 0;
	/* knives */
	weapon["worm tooth"] = 0;
	weapon["knife"] = 0;
	weapon["stiletto"] = 0;
	weapon["scalpel"] = 0;
	weapon["crysknife"] = 0;
	/* axes */
	weapon["axe"] = 0;
	weapon["battle-axe"] = 0;
	/* pick-axes */
	weapon["pick-axe"] = 0;
	weapon["dwarvish mattock"] = 0;
	/* short swords */
	weapon["orcish short sword"] = 0;
	weapon["short sword"] = 0;
	weapon["dwarvish short sword"] = 0;
	weapon["elven short sword"] = 0;
	/* broadswords */
	weapon["broadsword"] = 0;
	weapon["runesword"] = 0;
	weapon["elven broadsword"] = 0;
	/* long swords */
	weapon["long sword"] = 0;
	weapon["katana"] = 0;
	/* two-handed swords */
	weapon["two-handed sword"] = 0;
	weapon["tsurugi"] = 0;
	/* scimitars */
	weapon["scimitar"] = 0;
	/* sabers */
	weapon["silver saber"] = 0;
	/* clubs */
	weapon["club"] = 0;
	weapon["aklys"] = 0;
	/* maces */
	weapon["mace"] = 0;
	/* morning stars */
	weapon["morning star"] = 0;
	/* flails */
	weapon["flail"] = 0;
	weapon["grappling hook"] = 0;
	/* hammers */
	weapon["war hammer"] = 0;
	/* quarterstaves */
	weapon["quarterstaff"] = 0;
	/* polearms */
	weapon["partisan"] = 0;
	weapon["fauchard"] = 0;
	weapon["glaive"] = 0;
	weapon["bec-de-corbin"] = 0;
	weapon["spetum"] = 0;
	weapon["lucern hammer"] = 0;
	weapon["guisarme"] = 0;
	weapon["ranseur"] = 0;
	weapon["voulge"] = 0;
	weapon["bill-guisarme"] = 0;
	weapon["bardiche"] = 0;
	weapon["halberd"] = 0;
	/* spears */
	weapon["orcish spear"] = 0;
	weapon["spear"] = 0;
	weapon["silver spear"] = 0;
	weapon["elven spear"] = 0;
	weapon["dwarvish spear"] = 0;
	/* javelins */
	weapon["javelin"] = 0;
	/* tridents */
	weapon["trident"] = 0;
	/* lances */
	weapon["lance"] = 0;
	/* bows */
	weapon["orcish bow"] = 0;
	weapon["orcish arrow"] = 0;
	weapon["bow"] = 0;
	weapon["arrow"] = 0;
	weapon["elven bow"] = 0;
	weapon["elven arrow"] = 0;
	weapon["yumi"] = 0;
	weapon["ya"] = 0;
	weapon["silver arrow"] = 0;
	/* slings */
	weapon["sling"] = 0;
	weapon["flintstone"] = 0;
	/* crossbows */
	weapon["crossbow"] = 0;
	weapon["crossbow bolt"] = 0;
	/* darts */
	weapon["dart"] = 0;
	/* shurikens */
	weapon["shuriken"] = 0;
	/* boomerangs */
	weapon["boomerang"] = 0;
	/* whips */
	weapon["bullwhip"] = 0;
	weapon["rubber hose"] = 0;
	/* unicorn horns */
	weapon["unicorn horn"] = 0;

	/* artifact weapons */
	/* chaotic */
	weapon["Grimtooth"] = 0;
	weapon["Longbow of Diana"] = 0;
	weapon["Orcrist"] = 0;
	weapon["Sting"] = 0;
	weapon["Stormbringer"] = 0;
	/* neutral */
	weapon["Cleaver"] = 0;
	weapon["Giantslayer"] = 0;
	weapon["Magicbane"] = 0;
	weapon["Mjollnir"] = 0;
	weapon["Staff of Aesculapius"] = 0;
	weapon["Vorpal Blade"] = 0;
	/* lawful */
	weapon["Demonbane"] = 0;
	weapon["Excalibur"] = 0;
	weapon["Grayswandir"] = 0;
	weapon["Sceptre of Might"] = 0;
	weapon["Snickersnee"] = 0;
	weapon["Sunsword"] = 0;
	weapon["Tsurugi of Muramasa"] = 0;
	/* unaligned */
	weapon["Dragonbane"] = 0;
	weapon["Fire Brand"] = 0;
	weapon["Frost Brand"] = 0;
	weapon["Ogresmasher"] = 0;
	weapon["Trollsbane"] = 0;
	weapon["Werebane"] = 0;
}

/* methods */
void Weapon::analyze() {
}

void Weapon::parseMessages(const string &messages) {
	if (saiph->pickup.size() > 0) {
		/* pick up weapons according to our rules */
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			map<string, int>::iterator w = weapon.find(p->second.name);
			if (w == weapon.end())
				continue; // item is not a weapon
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
					if (*wgw == p->second.name) {
						/* weapon is in group.
						 * we'll also break here so she'll pick up better weapons in group,
						 * even if that means she'll exceed the limit.
						 * otherwise she won't pick up eg. elven daggers when she's full on orcish daggers */
						weapon_in_group = true;
						break;
					}
				}
				if (weapon_in_group && count < weapon_group_total[wg->first]) {
					if (count + p->second.count <= weapon_group_total[wg->first]) {
						/* pick them all up */
						command = p->first;
					} else {
						/* only pick some up */
						stringstream tmp;
						tmp << weapon_group_total[wg->first] - count - p->second.count << p->first;
						command = tmp.str();
					}
					priority = PRIORITY_SELECT_ITEM;
					return;
				}
			}
			/* solitary weapons */
			if (w->second <= 0)
				continue; // we don't desire this weapon
			if (p->second.beatitude & weapon_accept_beatitude[p->second.name] == 0)
				continue; // item does not have a beatitude we'll accept
			/* figure out how many we got of this item already */
			int count = 0;
			for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
				if (i->second.name != p->second.name)
					continue;
				count += i->second.count;
			}
			if (count >= w->second)
				continue; // got enough of this weapon already
			if (count + p->second.count <= w->second) {
				/* pick them all up */
				command = p->first;
			} else {
				/* only pick some up */
				stringstream tmp;
				tmp << w->second - count - p->second.count << p->first;
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
		if (request.status <= 0)
			return false;
		weapon_group_total[request.value] = request.status;
		return true;
	} else if (request.request == REQUEST_WEAPON_GROUP_ADD) {
		/* add weapon to group */
		if (weapon_group.find(request.value) == weapon_group.end())
			return false;
		if (weapon.find(request.data) == weapon.end())
			return false;
		weapon_accept_beatitude[request.data] = request.status;
		weapon_group[request.value].push_back(request.data);
		return true;
	} else if (request.request == REQUEST_WEAPON_AMOUNT) {
		/* set how many weapons of this type we wish to carry */
		if (weapon.find(request.data) == weapon.end())
			return false;
		weapon_accept_beatitude[request.data] = request.status;
		weapon[request.data] = request.value;
		return true;
	}
	return false;
}
