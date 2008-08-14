#include "Valkyrie.h"

/* constructors */
Valkyrie::Valkyrie(Saiph *saiph) : Analyzer("Valkyrie"), saiph(saiph) {
}

/* methods */
void Valkyrie::init() {
	int groupstart = 0;
	setupArmor(groupstart);
	setupFood(groupstart);
	setupTool(groupstart);
	setupWand(groupstart);
	setupWeapon(groupstart);
}

/* private methods */
void Valkyrie::setupArmor(int &groupstart) {
	/* create shirt group */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "1";
	saiph->request(req);
	/* add shirts, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
        req.data = "T-shirt";
	saiph->request(req);
        req.data = "Hawaiian shirt";
	saiph->request(req);

	/* create suit group */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "1";
	saiph->request(req);
	/* add suits, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "gray dragon scale mail";
	saiph->request(req);
        req.data = "silver dragon scale mail";
	saiph->request(req);
        req.data = "black dragon scale mail";
	saiph->request(req);
        req.data = "yellow dragon scale mail";
	saiph->request(req);
        req.data = "green dragon scale mail";
	saiph->request(req);
        req.data = "orange dragon scale mail";
	saiph->request(req);
        req.data = "red dragon scale mail";
	saiph->request(req);
        req.data = "blue dragon scale mail";
	saiph->request(req);
        req.data = "white dragon scale mail";
	saiph->request(req);
        req.data = "gray dragon scale";
	saiph->request(req);
        req.data = "silver dragon scale";
	saiph->request(req);
        req.data = "black dragon scale";
	saiph->request(req);
        req.data = "yellow dragon scale";
	saiph->request(req);
        req.data = "green dragon scale";
	saiph->request(req);
        req.data = "orange dragon scale";
	saiph->request(req);
        req.data = "red dragon scale";
	saiph->request(req);
        req.data = "blue dragon scale";
	saiph->request(req);
        req.data = "white dragon scale";
	saiph->request(req);
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
        req.data = "dwarvish mithril-coat";
	saiph->request(req);
        req.data = "elven mithril-coat";
	saiph->request(req);
        req.data = "studded leather armor";
	saiph->request(req);
        req.data = "leather armor";
	saiph->request(req);
        req.data = "scale mail";
	saiph->request(req);
        req.data = "ring mail";
	saiph->request(req);
        req.data = "orcish ring mail";
	saiph->request(req);
        req.data = "crude ring mail";
	saiph->request(req);
        req.data = "leather jacket";
	saiph->request(req);

	/* create cloak group */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "1";
	saiph->request(req);
	/* add cloaks, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "cloak of magic resistance";
	saiph->request(req);
        req.data = "cloak of protection";
	saiph->request(req);
        req.data = "robe";
	saiph->request(req);
        req.data = "cloak of displacement";
	saiph->request(req);
        req.data = "cloak of invisibility";
	saiph->request(req);
        req.data = "piece of cloth";
	saiph->request(req);
        req.data = "opera cloak";
	saiph->request(req);
        req.data = "ornamental cope";
	saiph->request(req);
        req.data = "tattered cape";
	saiph->request(req);
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
        req.data = "oilskin cloak";
	saiph->request(req);
        req.data = "slippery cloak";
	saiph->request(req);
        req.data = "elven cloak";
	saiph->request(req);
        req.data = "faded pall";
	saiph->request(req);
        req.data = "alchemy smock";
	saiph->request(req);
        req.data = "apron";
	saiph->request(req);
        req.data = "leather cloak";
	saiph->request(req);

	/* create helm group */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "1";
	saiph->request(req);
	/* add helmets, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
        req.data = "helm of telepathy";
	saiph->request(req);
        req.data = "dwarvish iron helm";
	saiph->request(req);
        req.data = "hard hat";
	saiph->request(req);
        req.data = "helm of brilliance";
	saiph->request(req);
        req.data = "plumed helmet";
	saiph->request(req);
        req.data = "etched helmet";
	saiph->request(req);
        req.data = "crested helmet";
	saiph->request(req);
        req.data = "visored helmet";
	saiph->request(req);
        req.data = "elven leather helm";
	saiph->request(req);
        req.data = "leather hat";
	saiph->request(req);
        req.data = "cornuthaum";
	saiph->request(req);
        req.data = "orcish helm";
	saiph->request(req);
        req.data = "iron skull cap";
	saiph->request(req);
        req.data = "helmet";
	saiph->request(req);

	/* create glove group */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "1";
	saiph->request(req);
	/* add gloves, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
        req.data = "pair of gauntlets of power";
	saiph->request(req);
        req.data = "pair of gauntlets of dexterity";
	saiph->request(req);
        req.data = "pair of padded gloves";
	saiph->request(req);
        req.data = "pair of riding gloves";
	saiph->request(req);
        req.data = "pair of fencing gloves";
	saiph->request(req);
        req.data = "pair of old gloves";
	saiph->request(req);
        req.data = "pair of leather gloves";
	saiph->request(req);

	/* create shield group */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "1";
	saiph->request(req);
	/* add shields, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "shield of reflection";
	saiph->request(req);
        req.data = "polished silver shield";
	saiph->request(req);
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
        req.data = "elven shield";
	saiph->request(req);
        req.data = "blue shield";
	saiph->request(req);
        req.data = "green shield";
	saiph->request(req);
        req.data = "small shield";
	saiph->request(req);
        req.data = "orcish shield";
	saiph->request(req);
        req.data = "red-eyed shield";
	saiph->request(req);
        req.data = "Uruk-hai shield";
	saiph->request(req);
        req.data = "white-handed shield";
	saiph->request(req);

	/* create boot group */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "1";
	saiph->request(req);
	/* add boots, most useful first.
	 * note: not adding levitation boots since we're not handling levitation yet */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "pair of speed boots";
	saiph->request(req);
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
        req.data = "pair of water walking boots";
	saiph->request(req);
        req.data = "pair of jumping boots";
	saiph->request(req);
        req.data = "pair of kicking boots";
	saiph->request(req);
        req.data = "pair of mud boots";
	saiph->request(req);
        req.data = "pair of buckled boots";
	saiph->request(req);
        req.data = "pair of riding boots";
	saiph->request(req);
        req.data = "pair of snow boots";
	saiph->request(req);
        req.data = "pair of hiking boots";
	saiph->request(req);
        req.data = "pair of combat boots";
	saiph->request(req);
        req.data = "pair of jungle boots";
	saiph->request(req);
        req.data = "pair of iron shoes";
	saiph->request(req);
        req.data = "pair of hard shoes";
	saiph->request(req);
        req.data = "pair of high boots";
	saiph->request(req);
        req.data = "pair of jackboots";
	saiph->request(req);
        req.data = "pair of elven boots";
	saiph->request(req);
        req.data = "pair of low boots";
	saiph->request(req);
        req.data = "pair of walking shoes";
	saiph->request(req);

	/* phew. now we just have to tell the armor analyzer pretty much the same thing */
	req.request = REQUEST_ARMOR_WEAR;
	req.status = BLESSED | UNCURSED;

	/* shirt */
	req.value = ARMOR_SHIRT;
        req.data = "T-shirt";
	saiph->request(req);
        req.data = "Hawaiian shirt";
	saiph->request(req);

	/* suit */
	req.value = ARMOR_SUIT;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "gray dragon scale mail";
	saiph->request(req);
        req.data = "silver dragon scale mail";
	saiph->request(req);
        req.data = "black dragon scale mail";
	saiph->request(req);
        req.data = "yellow dragon scale mail";
	saiph->request(req);
        req.data = "green dragon scale mail";
	saiph->request(req);
        req.data = "orange dragon scale mail";
	saiph->request(req);
        req.data = "red dragon scale mail";
	saiph->request(req);
        req.data = "blue dragon scale mail";
	saiph->request(req);
        req.data = "white dragon scale mail";
	saiph->request(req);
        req.data = "gray dragon scale";
	saiph->request(req);
        req.data = "silver dragon scale";
	saiph->request(req);
        req.data = "black dragon scale";
	saiph->request(req);
        req.data = "yellow dragon scale";
	saiph->request(req);
        req.data = "green dragon scale";
	saiph->request(req);
        req.data = "orange dragon scale";
	saiph->request(req);
        req.data = "red dragon scale";
	saiph->request(req);
        req.data = "blue dragon scale";
	saiph->request(req);
        req.data = "white dragon scale";
	saiph->request(req);
	req.status = BLESSED | UNCURSED;
        req.data = "dwarvish mithril-coat";
	saiph->request(req);
        req.data = "elven mithril-coat";
	saiph->request(req);
        req.data = "studded leather armor";
	saiph->request(req);
        req.data = "leather armor";
	saiph->request(req);
        req.data = "scale mail";
	saiph->request(req);
        req.data = "ring mail";
	saiph->request(req);
        req.data = "orcish ring mail";
	saiph->request(req);
        req.data = "crude ring mail";
	saiph->request(req);
        req.data = "leather jacket";
	saiph->request(req);

	/* cloak */
	req.value = ARMOR_CLOAK;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "cloak of magic resistance";
	saiph->request(req);
        req.data = "cloak of protection";
	saiph->request(req);
        req.data = "robe";
	saiph->request(req);
        req.data = "cloak of displacement";
	saiph->request(req);
        req.data = "cloak of invisibility";
	saiph->request(req);
        req.data = "piece of cloth";
	saiph->request(req);
        req.data = "opera cloak";
	saiph->request(req);
        req.data = "ornamental cope";
	saiph->request(req);
        req.data = "tattered cape";
	saiph->request(req);
	req.status = BLESSED | UNCURSED;
        req.data = "oilskin cloak";
	saiph->request(req);
        req.data = "slippery cloak";
	saiph->request(req);
        req.data = "elven cloak";
	saiph->request(req);
        req.data = "alchemy smock";
	saiph->request(req);
        req.data = "apron";
	saiph->request(req);
        req.data = "leather cloak";
	saiph->request(req);

	/* helmet */
	req.value = ARMOR_HELMET;
        req.data = "helm of telepathy";
	saiph->request(req);
        req.data = "dwarvish iron helm";
	saiph->request(req);
        req.data = "hard hat";
	saiph->request(req);
        req.data = "helm of brilliance";
	saiph->request(req);
        req.data = "plumed helmet";
	saiph->request(req);
        req.data = "etched helmet";
	saiph->request(req);
        req.data = "crested helmet";
	saiph->request(req);
        req.data = "visored helmet";
	saiph->request(req);
        req.data = "elven leather helm";
	saiph->request(req);
        req.data = "leather hat";
	saiph->request(req);
        req.data = "cornuthaum";
	saiph->request(req);
        req.data = "orcish helm";
	saiph->request(req);
        req.data = "iron skull cap";
	saiph->request(req);
        req.data = "helmet";
	saiph->request(req);

	/* gloves */
	req.value = ARMOR_GLOVES;
        req.data = "pair of gauntlets of power";
	saiph->request(req);
        req.data = "pair of gauntlets of dexterity";
	saiph->request(req);
        req.data = "pair of padded gloves";
	saiph->request(req);
        req.data = "pair of riding gloves";
	saiph->request(req);
        req.data = "pair of fencing gloves";
	saiph->request(req);
        req.data = "pair of old gloves";
	saiph->request(req);
        req.data = "pair of leather gloves";
	saiph->request(req);

	/* shield */
	req.value = ARMOR_SHIELD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "shield of reflection";
	saiph->request(req);
        req.data = "polished silver shield";
	saiph->request(req);
	req.status = BLESSED | UNCURSED;
        req.data = "elven shield";
	saiph->request(req);
        req.data = "blue shield";
	saiph->request(req);
        req.data = "green shield";
	saiph->request(req);
        req.data = "small shield";
	saiph->request(req);
        req.data = "orcish shield";
	saiph->request(req);
        req.data = "red-eyed shield";
	saiph->request(req);
        req.data = "Uruk-hai shield";
	saiph->request(req);
        req.data = "white-handed shield";
	saiph->request(req);

	/* boots */
	req.value = ARMOR_BOOTS;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
        req.data = "pair of speed boots";
	saiph->request(req);
	req.status = BLESSED | UNCURSED;
        req.data = "pair of water walking boots";
	saiph->request(req);
        req.data = "pair of jumping boots";
	saiph->request(req);
        req.data = "pair of kicking boots";
	saiph->request(req);
        req.data = "pair of mud boots";
	saiph->request(req);
        req.data = "pair of buckled boots";
	saiph->request(req);
        req.data = "pair of riding boots";
	saiph->request(req);
        req.data = "pair of snow boots";
	saiph->request(req);
        req.data = "pair of hiking boots";
	saiph->request(req);
        req.data = "pair of combat boots";
	saiph->request(req);
        req.data = "pair of jungle boots";
	saiph->request(req);
        req.data = "pair of iron shoes";
	saiph->request(req);
        req.data = "pair of hard shoes";
	saiph->request(req);
        req.data = "pair of high boots";
	saiph->request(req);
        req.data = "pair of jackboots";
	saiph->request(req);
        req.data = "pair of elven boots";
	saiph->request(req);
        req.data = "pair of low boots";
	saiph->request(req);
        req.data = "pair of walking shoes";
	saiph->request(req);
}

void Valkyrie::setupFood(int &groupstart) {
	/* first some food items we don't really want for food, but beneficial effects */
	req.request = REQUEST_ITEM_PICKUP;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
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
	req.value = groupstart++;
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

void Valkyrie::setupTool(int &groupstart) {
	/* unihorn */
	req.request = REQUEST_ITEM_PICKUP;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.value = 10;
	req.data = "unicorn horn";
	saiph->request(req);
}

void Valkyrie::setupWand(int &groupstart) {
	/* create a group for wands */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
	req.data = "20";
	saiph->request(req);
	/* add wands to the group, best wand first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.status = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "wand of wishing";
	saiph->request(req);
	req.data = "wand of death";
	saiph->request(req);
	req.data = "wand of fire";
	saiph->request(req);
	req.data = "wand of lightning";
	saiph->request(req);
	req.data = "wand of cold";
	saiph->request(req);
	req.data = "wand of magic missile";
	saiph->request(req);
	req.data = "wand of striking";
	saiph->request(req);
	req.data = "wand of cancellation";
	saiph->request(req);
	req.data = "wand of speed monster";
	saiph->request(req);
	req.data = "wand of sleep";
	saiph->request(req);
	req.data = "wand of slow monster";
	saiph->request(req);
	req.data = "wand of digging";
	saiph->request(req);
	req.data = "wand of make invisible";
	saiph->request(req);
	req.data = "wand of teleportation";
	saiph->request(req);
	req.data = "wand of polymorph";
	saiph->request(req);
	req.data = "wand of create monster";
	saiph->request(req);
	req.data = "wand of secret door detection";
	saiph->request(req);
	req.data = "wand of undead turning";
	saiph->request(req);
	req.data = "wand of locking";
	saiph->request(req);
	req.data = "wand of enlightenment";
	saiph->request(req);
	req.data = "wand of probing";
	saiph->request(req);
	req.data = "wand of opening";
	saiph->request(req);
	req.data = "wand of light";
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
	req.data = "wand of nothing";
	saiph->request(req);
}

void Valkyrie::setupWeapon(int &groupstart) {
	/* create a group for thrown weapons */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.value = groupstart++;
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
	req.value = groupstart++;
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
	req.data = "pick-axe";
	req.value = 1;
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
	req.data = "pick-axe";
	saiph->request(req);
	req.data = "silver dagger";
	saiph->request(req);
	req.data = "elven dagger";
	saiph->request(req);
	req.data = "dagger";
	saiph->request(req);
	req.data = "orcish dagger";
	saiph->request(req);
}
