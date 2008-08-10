#include "Valkyrie.h"

/* constructors */
Valkyrie::Valkyrie(Saiph *saiph) : Analyzer("Valkyrie"), saiph(saiph) {
}

/* methods */
void Valkyrie::init() {
	setupFood();
	setupWand();
	setupWeapon();
}

/* private methods */
void Valkyrie::setupFood() {
	/* first some food items we don't really want for food, but beneficial effects */
	req.request = REQUEST_ITEM_PICKUP;
	req.value = 5;
	req.data = "lizard corpse";
	saiph->request(req);
	req.data = "partly eaten lizard corpse";
	saiph->request(req);
        req.data = "sprig of wolfsbane";
	saiph->request(req);
        req.data = "eucalyptus leaf";
	saiph->request(req);
        req.data = "carrot";
	saiph->request(req);

	/* create a group for food */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = 0;
	req.data = "100";
	saiph->request(req);
	/* add food, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "lembas wafer";
	saiph->request(req);
        req.data = "partly eaten lembas wafer";
	saiph->request(req);
        req.data = "pancake";
	saiph->request(req);
        req.data = "partly eaten pancake";
	saiph->request(req);
        req.data = "lump of royal jelly";
	saiph->request(req);
        req.data = "byte";
	saiph->request(req);
        req.data = "candy bar";
	saiph->request(req);
        req.data = "orange";
	saiph->request(req);
        req.data = "banana";
	saiph->request(req);
        req.data = "fortune cookie";
	saiph->request(req);
        req.data = "clove of garlic";
	saiph->request(req);
        req.data = "K-ration";
	saiph->request(req);
        req.data = "gunyoki";
	saiph->request(req);
        req.data = "partly eaten gunyoki";
	saiph->request(req);
        req.data = "food ration";
	saiph->request(req);
        req.data = "partly eaten food ration";
	saiph->request(req);
        req.data = "cram ration";
	saiph->request(req);
        req.data = "partly eaten cram ration";
	saiph->request(req);
        req.data = "C-ration";
	saiph->request(req);
        req.data = "kelp frond";
	saiph->request(req);
        req.data = "pear";
	saiph->request(req);
        req.data = "apple";
	saiph->request(req);
        req.data = "melon";
	saiph->request(req);
        req.data = "tripe ration";
	saiph->request(req);
        req.data = "partly eaten tripe ration";
	saiph->request(req);
        req.data = "lichen corpse";
	saiph->request(req);
        req.data = "partly eaten lichen corpse";
	saiph->request(req);
        req.data = "cream pie";
	saiph->request(req);
        req.data = "huge chunk of meat";
	saiph->request(req);
        req.data = "partly eaten huge chunk of meat";
	saiph->request(req);
        req.data = "meat stick";
	saiph->request(req);
        req.data = "meat ring";
	saiph->request(req);
        req.data = "meatball";
	saiph->request(req);
}

void Valkyrie::setupWand() {
	/* create a group for wands */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = 1;
	req.data = "20";
	saiph->request(req);
	/* add wands to the group, best wand first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "wishing";
	saiph->request(req);
	req.data = "death";
	saiph->request(req);
	req.data = "fire";
	saiph->request(req);
	req.data = "lightning";
	saiph->request(req);
	req.data = "cold";
	saiph->request(req);
	req.data = "magic missile";
	saiph->request(req);
	req.data = "striking";
	saiph->request(req);
	req.data = "cancellation";
	saiph->request(req);
	req.data = "speed monster";
	saiph->request(req);
	req.data = "sleep";
	saiph->request(req);
	req.data = "slow monster";
	saiph->request(req);
	req.data = "digging";
	saiph->request(req);
	req.data = "make invisible";
	saiph->request(req);
	req.data = "teleportation";
	saiph->request(req);
	req.data = "polymorph";
	saiph->request(req);
	req.data = "create monster";
	saiph->request(req);
	req.data = "secret door detection";
	saiph->request(req);
	req.data = "undead turning";
	saiph->request(req);
	req.data = "locking";
	saiph->request(req);
	req.data = "enlightenment";
	saiph->request(req);
	req.data = "probing";
	saiph->request(req);
	req.data = "opening";
	saiph->request(req);
	req.data = "light";
	saiph->request(req);
	req.data = "glass wand";
	saiph->request(req);
	req.data = "oak wand";
	saiph->request(req);
	req.data = "copper wand";
	saiph->request(req);
	req.data = "aluminum wand";
	saiph->request(req);
	req.data = "short wand";
	saiph->request(req);
	req.data = "spiked wand";
	saiph->request(req);
	req.data = "balsa wand";
	saiph->request(req);
	req.data = "ebony wand";
	saiph->request(req);
	req.data = "silver wand";
	saiph->request(req);
	req.data = "uranium wand";
	saiph->request(req);
	req.data = "runed wand";
	saiph->request(req);
	req.data = "jeweled wand";
	saiph->request(req);
	req.data = "crystal wand";
	saiph->request(req);
	req.data = "marble wand";
	saiph->request(req);
	req.data = "platinum wand";
	saiph->request(req);
	req.data = "iron wand";
	saiph->request(req);
	req.data = "long wand";
	saiph->request(req);
	req.data = "maple wand";
	saiph->request(req);
	req.data = "tin wand";
	saiph->request(req);
	req.data = "iridium wand";
	saiph->request(req);
	req.data = "steel wand";
	saiph->request(req);
	req.data = "curved wand";
	saiph->request(req);
	req.data = "pine wand";
	saiph->request(req);
	req.data = "brass wand";
	saiph->request(req);
	req.data = "zinc wand";
	saiph->request(req);
	req.data = "hexagonal wand";
	saiph->request(req);
	req.data = "forked wand";
	saiph->request(req);
	req.data = "nothing";
	saiph->request(req);
}

void Valkyrie::setupWeapon() {
	/* create a group for thrown weapons */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = 2;
	req.data = "20";
	saiph->request(req);
	/* add weapons to the group, best weapon first */
	req.request = REQUEST_ITEM_GROUP_ADD;
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

	/* create a group for weapons we wish to wield too */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = 3;
	req.data = "2"; // we'll have main weapon & backup, nothing else
	saiph->request(req);
	/* add the weapons */
	req.request = REQUEST_ITEM_GROUP_ADD;
	/* allow picking up cursed excalibur */
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "Excalibur";
	saiph->request(req);
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "long sword";
	saiph->request(req);

	/* set other weapons we wish to pick up that is not in a group */
	req.request = REQUEST_ITEM_PICKUP;
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

	/* and set which weapons we prefer wielding */
	req.request = REQUEST_WEAPON_WIELD;
	/* allow wielding cursed Excalibur */
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "Excalibur";
	saiph->request(req);
	/* we don't want to wield a cursed long sword (we'll gamble on unknown, though) */
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "long sword";
	saiph->request(req);
}
