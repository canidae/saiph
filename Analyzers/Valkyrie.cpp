#include "Valkyrie.h"

/* constructors */
Valkyrie::Valkyrie(Saiph *saiph) : Analyzer("Valkyrie"), saiph(saiph) {
}

/* methods */
void Valkyrie::init() {
	/* "create" weapon group 0 with max 20 weapons.
	 * we use this so we'll carry 20 daggers/spears total */
	req.request = REQUEST_WEAPON_GROUP_CREATE;
	req.value = 0;
	req.data = "20";
	saiph->request(req);
	/* add weapons to the group, best weapon first */
	req.request = REQUEST_WEAPON_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "silver dagger";
	saiph->request(req);
	req.data = "elven dagger";
	saiph->request(req);
	req.data = "dagger";
	saiph->request(req);
	req.data = "orcish dagger";
	saiph->request(req);
	req.data = "silver spear";
	saiph->request(req);
	req.data = "dwarvish spear";
	saiph->request(req);
	req.data = "elven spear";
	saiph->request(req);
	req.data = "spear";
	saiph->request(req);
	req.data = "orcish spear";
	saiph->request(req);

	/* and set which weapons we prefer wielding.
	 * if we only want 1 of the weapons we wish to wield,
	 * then we don't need to REQUEST_WEAPON_PICKUP them. */
	req.request = REQUEST_WEAPON_WIELD;
	/* allow wielding cursed Excalibur */
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "Excalibur";
	saiph->request(req);
	/* we don't want to wield a cursed long sword */
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "long sword";
	saiph->request(req);

	/* set weapons we wish to pick up */
	req.request = REQUEST_WEAPON_PICKUP;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "dart";
	req.value = 200;
	saiph->request(req);

	/* add thrown weapons in the order we want to throw them.
	 * currently we'll throw them in the order we wish to get rid of stuff,
	 * not the order that yields max damage */
	req.request = REQUEST_ADD_THROWN_WEAPON;
	req.data = "orcish spear";
	saiph->request(req);
	req.data = "spear";
	saiph->request(req);
	req.data = "elven spear";
	saiph->request(req);
	req.data = "dwarvish spear";
	saiph->request(req);
	req.data = "silver spear";
	saiph->request(req);
	req.data = "orcish dagger";
	saiph->request(req);
	req.data = "dagger";
	saiph->request(req);
	req.data = "elven dagger";
	saiph->request(req);
	req.data = "silver dagger";
	saiph->request(req);
	req.data = "dart";
	saiph->request(req);
}
