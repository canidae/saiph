#include "Valkyrie.h"
#include "../Saiph.h"
#include "../World.h"

/* constructors/destructor */
Valkyrie::Valkyrie(Saiph *saiph) : Analyzer("Valkyrie"), saiph(saiph), loot_group(0) {
}

/* methods */
void Valkyrie::init() {
	setupAmulet();
	setupArmor();
	setupFood();
	setupPotion();
	setupRing();
	setupTool();
	setupWand();
	setupWeapon();

	/* set the cold resistance all valks start with */
	saiph->world->player.cold_resistance = true;
}

/* private methods */
void Valkyrie::setupAmulet() {
	/* 10 of each unidentified amulet */
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 10;
	req.data = "circular amulet";
	saiph->request(req);
	req.data = "spherical amulet";
	saiph->request(req);
	req.data = "oval amulet";
	saiph->request(req);
	req.data = "triangular amulet";
	saiph->request(req);
	req.data = "pyramidal amulet";
	saiph->request(req);
	req.data = "square amulet";
	saiph->request(req);
	req.data = "concave amulet";
	saiph->request(req);
	req.data = "hexagonal amulet";
	saiph->request(req);
	req.data = "octagonal amulet";
	saiph->request(req);

	/* and 10 of each identified amulet */
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 10;
	req.data = "amulet of ESP";
	saiph->request(req);
	req.data = "amulet of life saving";
	saiph->request(req);
	req.data = "amulet of magical breathing";
	saiph->request(req);
	req.data = "amulet of reflection";
	saiph->request(req);
	req.data = "amulet of unchanging";
	saiph->request(req);
	req.data = "Amulet of Yendor";
	saiph->request(req);
	req.data = "amulet versus poison";
	saiph->request(req);
	req.data = "amulet of change";
	saiph->request(req);
	req.data = "amulet of restful sleep";
	saiph->request(req);
	req.data = "amulet of strangulation";
	saiph->request(req);

	/* tell the amulet analyzer which amulets we wish to wear */
	req.request = REQUEST_AMULET_WEAR;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "amulet of life saving";
	saiph->request(req);
	req.data = "amulet of reflection";
	saiph->request(req);
	req.beatitude = BLESSED | UNCURSED;
	req.data = "amulet of ESP";
	saiph->request(req);
	req.data = "amulet of magical breathing";
	saiph->request(req);
	req.data = "amulet of unchanging";
	saiph->request(req);
	req.data = "Amulet of Yendor";
	saiph->request(req);
	req.data = "amulet versus poison";
	saiph->request(req);
}

void Valkyrie::setupArmor() {
	/* tell the armor analyzer what to wear.
	 * priorities only affects armor in same group */
	req.request = REQUEST_ARMOR_WEAR;

	/* shirt */
	req.key = ARMOR_SHIRT;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.sustain = false;
	req.value = 3; // allow carrying up to 3 shirts, we'll drop 2 when we know the enchantment
	req.data = "T-shirt";
	req.priority = 10;
	saiph->request(req);
	req.data = "Hawaiian shirt";
	req.priority = 10;
	saiph->request(req);

	/* suit */
	req.key = ARMOR_SUIT;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = false;
	req.value = 1; // heavy stuff, only carry 1 at a time
	req.data = "gray dragon scale mail";
	req.priority = 20;
	saiph->request(req);
	req.data = "silver dragon scale mail";
	req.priority = 19;
	saiph->request(req);
	req.data = "set of gray dragon scales";
	req.priority = 16;
	saiph->request(req);
	req.data = "set of silver dragon scales";
	req.priority = 15;
	saiph->request(req);
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "black dragon scale mail";
	req.priority = 12;
	saiph->request(req);
	req.data = "yellow dragon scale mail";
	req.priority = 12;
	saiph->request(req);
	req.data = "green dragon scale mail";
	req.priority = 12;
	saiph->request(req);
	req.data = "orange dragon scale mail";
	req.priority = 12;
	saiph->request(req);
	req.data = "red dragon scale mail";
	req.priority = 12;
	saiph->request(req);
	req.data = "blue dragon scale mail";
	req.priority = 12;
	saiph->request(req);
	req.data = "white dragon scale mail";
	req.priority = 12;
	saiph->request(req);
	req.beatitude = BLESSED | UNCURSED;
	req.data = "dwarvish mithril-coat";
	req.priority = 8;
	saiph->request(req);
	req.data = "elven mithril-coat";
	req.priority = 7;
	saiph->request(req);
	req.data = "splint mail";
	req.priority = 6;
	saiph->request(req);
	req.data = "bronze plate mail";
	req.priority = 6;
	saiph->request(req);
	req.data = "scale mail";
	req.priority = 4;
	saiph->request(req);
	req.data = "studded leather armor";
	req.priority = 3;
	saiph->request(req);
	req.data = "ring mail";
	req.priority = 3;
	saiph->request(req);
	req.data = "leather armor";
	req.priority = 2;
	saiph->request(req);
	req.data = "orcish ring mail";
	req.priority = 2;
	saiph->request(req);
	req.data = "crude ring mail";
	req.priority = 2;
	saiph->request(req);
	req.data = "leather jacket";
	req.priority = 1;
	saiph->request(req);

	/* cloak */
	req.key = ARMOR_CLOAK;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = true;
	req.value = 3; // allow carrying up to 3 cloaks, we'll drop 2 when we know the enchantment
	req.data = "piece of cloth";
	req.priority = 20;
	saiph->request(req);
	req.data = "opera cloak";
	req.priority = 20;
	saiph->request(req);
	req.data = "ornamental cope";
	req.priority = 20;
	saiph->request(req);
	req.data = "tattered cape";
	req.priority = 20;
	saiph->request(req);
	req.data = "cloak of magic resistance";
	req.priority = 20;
	saiph->request(req);
	req.data = "cloak of protection";
	req.priority = 20;
	saiph->request(req);
	req.data = "cloak of displacement";
	req.priority = 20;
	saiph->request(req);
	req.data = "cloak of invisibility";
	req.priority = 20;
	saiph->request(req);
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "oilskin cloak";
	req.priority = 15;
	saiph->request(req);
	req.data = "slippery cloak";
	req.priority = 15;
	saiph->request(req);
	req.beatitude = BLESSED | UNCURSED;
	req.sustain = false;
	req.data = "elven cloak";
	req.priority = 10;
	saiph->request(req);
	req.data = "faded pall";
	req.priority = 10;
	saiph->request(req);
	req.data = "robe";
	req.priority = 6;
	saiph->request(req);
	req.data = "alchemy smock";
	req.priority = 5;
	saiph->request(req);
	req.data = "apron";
	req.priority = 5;
	saiph->request(req);
	req.data = "leather cloak";
	req.priority = 5;
	saiph->request(req);
	req.data = "dwarvish cloak";
	req.priority = 4;
	saiph->request(req);
	req.data = "hooded cloak";
	req.priority = 4;
	saiph->request(req);
	req.data = "orcish cloak";
	req.priority = 4;
	saiph->request(req);
	req.data = "coarse mantelet";
	req.priority = 4;
	saiph->request(req);

	/* helmet */
	req.key = ARMOR_HELMET;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = false;
	req.value = 3; // allow carrying up to 3 helmets, we'll drop 2 when we know the enchantment
	req.data = "Mitre of Holiness";
	req.priority = 20;
	saiph->request(req);
	req.data = "helm of telepathy";
	req.priority = 15;
	saiph->request(req);
	req.data = "helm of brilliance";
	req.priority = 15;
	saiph->request(req);
	req.beatitude = BLESSED | UNCURSED;
	req.data = "dwarvish iron helm";
	req.priority = 10;
	saiph->request(req);
	req.data = "hard hat";
	req.priority = 10;
	saiph->request(req);
	req.data = "plumed helmet";
	req.priority = 10;
	saiph->request(req);
	req.data = "etched helmet";
	req.priority = 10;
	saiph->request(req);
	req.data = "crested helmet";
	req.priority = 10;
	saiph->request(req);
	req.data = "visored helmet";
	req.priority = 10;
	saiph->request(req);
	req.data = "elven leather helm";
	req.priority = 9;
	saiph->request(req);
	req.data = "leather hat";
	req.priority = 9;
	saiph->request(req);
	req.data = "cornuthaum";
	req.priority = 9;
	saiph->request(req);
	req.data = "orcish helm";
	req.priority = 9;
	saiph->request(req);
	req.data = "iron skull cap";
	req.priority = 9;
	saiph->request(req);
	req.data = "helmet";
	req.priority = 9;
	saiph->request(req);

	/* gloves */
	req.key = ARMOR_GLOVES;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = false;
	req.value = 3; // allow carrying up to 3 gloves, we'll drop 2 when we know the enchantment
	req.data = "pair of gauntlets of power";
	req.priority = 20;
	saiph->request(req);
	req.data = "pair of gauntlets of dexterity";
	req.priority = 20;
	saiph->request(req);
	req.beatitude = BLESSED | UNCURSED;
	req.data = "pair of padded gloves";
	req.priority = 15;
	saiph->request(req);
	req.data = "pair of riding gloves";
	req.priority = 15;
	saiph->request(req);
	req.data = "pair of fencing gloves";
	req.priority = 15;
	saiph->request(req);
	req.data = "pair of old gloves";
	req.priority = 15;
	saiph->request(req);
	req.data = "pair of leather gloves";
	req.priority = 10;
	saiph->request(req);

	/* shield */
	req.key = ARMOR_SHIELD;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = false;
	req.value = 2; // allow carrying up to 2 shields, we'll drop 1 when we know the enchantment
	req.data = "shield of reflection";
	req.priority = 20;
	saiph->request(req);
	req.data = "polished silver shield";
	req.priority = 20;
	saiph->request(req);
	req.beatitude = BLESSED | UNCURSED;
	req.data = "elven shield";
	req.priority = 15;
	saiph->request(req);
	req.data = "blue shield";
	req.priority = 15;
	saiph->request(req);
	req.data = "green shield";
	req.priority = 15;
	saiph->request(req);
	req.data = "small shield";
	req.priority = 14;
	saiph->request(req);
	req.data = "orcish shield";
	req.priority = 14;
	saiph->request(req);
	req.data = "red-eyed shield";
	req.priority = 14;
	saiph->request(req);
	req.data = "Uruk-hai shield";
	req.priority = 14;
	saiph->request(req);
	req.data = "white-handed shield";
	req.priority = 14;
	saiph->request(req);
	req.data = "dwarvish roundshield";
	req.priority = 14;
	saiph->request(req);
	req.data = "large round shield";
	req.priority = 14;
	saiph->request(req);
	req.data = "large shield";
	req.priority = 14;
	saiph->request(req);

	/* boots */
	req.key = ARMOR_BOOTS;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = true;
	req.value = 3; // allow carrying up to 3 boots, we'll drop 2 when we know the enchantment
	req.data = "pair of speed boots";
	req.priority = 20;
	saiph->request(req);
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "pair of water walking boots";
	req.priority = 15;
	saiph->request(req);
	req.data = "pair of jumping boots";
	req.priority = 15;
	saiph->request(req);
	req.beatitude = BLESSED | UNCURSED;
	req.data = "pair of mud boots";
	req.priority = 10;
	saiph->request(req);
	req.data = "pair of buckled boots";
	req.priority = 10;
	saiph->request(req);
	req.data = "pair of riding boots";
	req.priority = 10;
	saiph->request(req);
	req.data = "pair of snow boots";
	req.priority = 10;
	saiph->request(req);
	req.sustain = false;
	req.data = "pair of hiking boots";
	req.priority = 10;
	saiph->request(req);
	req.data = "pair of combat boots";
	req.priority = 10;
	saiph->request(req);
	req.data = "pair of jungle boots";
	req.priority = 10;
	saiph->request(req);
	req.data = "pair of iron shoes";
	req.priority = 9;
	saiph->request(req);
	req.data = "pair of hard shoes";
	req.priority = 8;
	saiph->request(req);
	req.data = "pair of high boots";
	req.priority = 8;
	saiph->request(req);
	req.data = "pair of jackboots";
	req.priority = 8;
	saiph->request(req);
	req.data = "pair of elven boots";
	req.priority = 8;
	saiph->request(req);
	req.data = "pair of low boots";
	req.priority = 8;
	saiph->request(req);
	req.data = "pair of walking shoes";
	req.priority = 8;
	saiph->request(req);
}

void Valkyrie::setupFood() {
	/* first some food items we don't really want for food, but beneficial effects */
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 10;
	req.data = "lizard corpse";
	saiph->request(req);
	req.data = "partly eaten lizard corpse";
	saiph->request(req);
	req.data = "sprig of wolfsbane";
	saiph->request(req);
	req.data = "partly eaten sprig of wolfsbane";
	saiph->request(req);
	req.data = "eucalyptus leaf";
	saiph->request(req);
	req.data = "partly eaten eucalyptus leaf";
	saiph->request(req);
	req.data = "carrot";
	saiph->request(req);
	req.data = "partly eaten carrot";
	saiph->request(req);

	/* create a group for food */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.key = loot_group++;
	req.value = 100;
	saiph->request(req);
	/* add food, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
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
	req.data = "partly eaten lump of royal jelly";
	saiph->request(req);
	req.data = "byte";
	saiph->request(req);
	req.data = "partly eaten byte";
	saiph->request(req);
	req.data = "candy bar";
	saiph->request(req);
	req.data = "partly eaten candy bar";
	saiph->request(req);
	req.data = "orange";
	saiph->request(req);
	req.data = "partly eaten orange";
	saiph->request(req);
	req.data = "banana";
	saiph->request(req);
	req.data = "partly eaten banana";
	saiph->request(req);
	req.data = "fortune cookie";
	saiph->request(req);
	req.data = "partly eaten fortune cookie";
	saiph->request(req);
	req.data = "clove of garlic";
	saiph->request(req);
	req.data = "partly eaten clove of garlic";
	saiph->request(req);
	req.data = "K-ration";
	saiph->request(req);
	req.data = "partly eaten K-ration";
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
	req.data = "partly eaten C-ration";
	saiph->request(req);
	req.data = "kelp frond";
	saiph->request(req);
	req.data = "partly eaten kelp frond";
	saiph->request(req);
	req.data = "pear";
	saiph->request(req);
	req.data = "partly eaten pear";
	saiph->request(req);
	req.data = "apple";
	saiph->request(req);
	req.data = "partly eaten apple";
	saiph->request(req);
	req.data = "melon";
	saiph->request(req);
	req.data = "partly eaten melon";
	saiph->request(req);
	/* this kills her too often, and we got plenty food
	req.data = "tripe ration";
	saiph->request(req);
	req.data = "partly eaten tripe ration";
	saiph->request(req);
	*/
	req.data = "lichen corpse";
	saiph->request(req);
	req.data = "partly eaten lichen corpse";
	saiph->request(req);
	req.data = "cream pie";
	saiph->request(req);
	req.data = "partly eaten cream pie";
	saiph->request(req);
	/* we mostly find these in bones == cursed
	req.data = "huge chunk of meat";
	saiph->request(req);
	req.data = "partly eaten huge chunk of meat";
	saiph->request(req);
	req.data = "meat stick";
	saiph->request(req);
	req.data = "partly eaten meat stick";
	saiph->request(req);
	req.data = "meat ring";
	saiph->request(req);
	req.data = "partly eaten meat ring";
	saiph->request(req);
	req.data = "meatball";
	saiph->request(req);
	req.data = "partly eaten meatball";
	saiph->request(req);
	*/
}

void Valkyrie::setupPotion() {
	/* create a group for potions */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.key = loot_group++;
	req.value = 10;
	saiph->request(req);
	/* add potions, most useful first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "potion of gain level";
	saiph->request(req);
	req.data = "potion of full healing";
	saiph->request(req);
	req.data = "potion of extra healing";
	saiph->request(req);
	req.data = "potion of healing";
	saiph->request(req);
}

void Valkyrie::setupRing() {
	/* 2 of each unidentified ring */
	/*
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 2;
	req.data = "pearl ring";
	saiph->request(req);
	req.data = "iron ring";
	saiph->request(req);
	req.data = "twisted ring";
	saiph->request(req);
	req.data = "steel ring";
	saiph->request(req);
	req.data = "wire ring";
	saiph->request(req);
	req.data = "engagement ring";
	saiph->request(req);
	req.data = "shiny ring";
	saiph->request(req);
	req.data = "bronze ring";
	saiph->request(req);
	req.data = "brass ring";
	saiph->request(req);
	req.data = "copper ring";
	saiph->request(req);
	req.data = "silver ring";
	saiph->request(req);
	req.data = "gold ring";
	saiph->request(req);
	req.data = "wooden ring";
	saiph->request(req);
	req.data = "granite ring";
	saiph->request(req);
	req.data = "opal ring";
	saiph->request(req);
	req.data = "clay ring";
	saiph->request(req);
	req.data = "coral ring";
	saiph->request(req);
	req.data = "black onyx ring";
	saiph->request(req);
	req.data = "moonstone ring";
	saiph->request(req);
	req.data = "tiger eye ring";
	saiph->request(req);
	req.data = "jade ring";
	saiph->request(req);
	req.data = "agate ring";
	saiph->request(req);
	req.data = "topaz ring";
	saiph->request(req);
	req.data = "sapphire ring";
	saiph->request(req);
	req.data = "ruby ring";
	saiph->request(req);
	req.data = "diamond ring";
	saiph->request(req);
	req.data = "ivory ring";
	saiph->request(req);
	req.data = "emerald ring";
	saiph->request(req);
	*/

	/* and one of each useful ring */
	/*
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 1;
	req.data = "ring of protection";
	saiph->request(req);
	req.data = "ring of protection from shape changers";
	saiph->request(req);
	req.data = "ring of stealth";
	saiph->request(req);
	req.data = "ring of sustain ability";
	saiph->request(req);
	req.data = "ring of warning";
	saiph->request(req);
	req.data = "ring of gain constitution";
	saiph->request(req);
	req.data = "ring of gain strength";
	saiph->request(req);
	req.data = "ring of increase accuracy";
	saiph->request(req);
	req.data = "ring of increase damage";
	saiph->request(req);
	req.data = "ring of invisibility";
	saiph->request(req);
	req.data = "ring of see invisible";
	saiph->request(req);
	req.data = "ring of free action";
	saiph->request(req);
	req.data = "ring of levitation";
	saiph->request(req);
	req.data = "ring of regeneration";
	saiph->request(req);
	req.data = "ring of searching";
	saiph->request(req);
	req.data = "ring of slow digestion";
	saiph->request(req);
	req.data = "ring of teleportation";
	saiph->request(req);
	req.data = "ring of conflict";
	saiph->request(req);
	req.data = "ring of teleport control";
	saiph->request(req);
	*/

	/* tell the ring analyzer which rings we wish to wear */
	req.request = REQUEST_RING_WEAR;
	req.beatitude = BLESSED | UNCURSED;
	req.data = "ring of free action";
	saiph->request(req);
	req.data = "ring of conflict";
	saiph->request(req);
	req.data = "ring of protection from shape changers";
	saiph->request(req);
	req.data = "ring of warning";
	saiph->request(req);
	req.data = "ring of regeneration";
	saiph->request(req);
	req.data = "ring of protection";
	saiph->request(req);
}

void Valkyrie::setupTool() {
	/* unihorn */
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.only_unknown_enchantment = false;
	req.value = 3;
	req.data = "unicorn horn";
	saiph->request(req);

	/* stethoscope */
	/* currently not used, need to fix this in explore
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.only_unknown_enchantment = false;
	req.value = 1;
	req.data = "stethoscope";
	saiph->request(req);
	*/

	/* magic marker */
	/* currently not used
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 10;
	req.data = "magic marker";
	saiph->request(req);
	*/

	/* lamps & lanterns */
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 5;
	req.data = "magic lamp";
	saiph->request(req);
	req.data = "lamp";
	saiph->request(req);
	req.data = "oil lamp";
	saiph->request(req);
	req.data = "brass lantern";
	saiph->request(req);

	/* key/lock pick/credit card */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.key = loot_group++;
	req.value = 1;
	saiph->request(req);
	/* most useful unlocking device first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.data = "skeleton key";
	saiph->request(req);
	req.data = "key";
	saiph->request(req);
	req.data = "lock pick";
	saiph->request(req);
	req.data = "credit card";
	saiph->request(req);

	/* artifact tools */
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.only_unknown_enchantment = false;
	req.value = 1;
	req.data = "Orb of Detection";
	saiph->request(req);
	/* neutral, will kill us
	req.data = "Heart of Ahriman";
	saiph->request(req);
	*/
	req.data = "Magic Mirror of Merlin";
	saiph->request(req);
	/* neutral, will kill us
	req.data = "Eyes of the Overworld";
	saiph->request(req);
	*/
	/* chaotic, will kill us
	req.data = "Master Key of Thievery";
	saiph->request(req);
	*/
	/* neutral, will kill us
	req.data = "Platinum Yendorian Express Card";
	saiph->request(req);
	*/
	req.data = "Orb of Fate";
	saiph->request(req);
	/* neutral, will kill us
	req.data = "Eye of the Aethiopica";
	saiph->request(req);
	*/
}

void Valkyrie::setupWand() {
	/* create a group for wands */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.key = loot_group++;
	req.value = 10;
	saiph->request(req);
	/* add wands to the group, best wand first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
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
	req.data = "wand";
	saiph->request(req);
}

void Valkyrie::setupWeapon() {
	/* set up weapons for Weapon analyzer */
	req.request = REQUEST_WEAPON_WIELD;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = true; // never drop the following weapons
	req.value = 1; // only keep 1 of each weapon for now
	req.data = "Mjollnir";
	req.priority = 100;
	saiph->request(req);
	req.data = "Grayswandir";
	req.priority = 95;
	saiph->request(req);
	req.data = "Fire Brand";
	req.priority = 94;
	saiph->request(req);
	req.data = "Frost Brand";
	req.priority = 94;
	saiph->request(req);
	req.data = "Excalibur";
	req.priority = 93;
	saiph->request(req);
	req.data = "Sunsword";
	req.priority = 92;
	saiph->request(req);
	req.data = "Snickersnee";
	req.priority = 91;
	saiph->request(req);
	req.data = "Werebane";
	req.priority = 90;
	saiph->request(req);
	req.data = "Demonbane";
	req.priority = 90;
	saiph->request(req);
	req.data = "Dragonbane";
	req.priority = 90;
	saiph->request(req);
	/* then wield common weapons, blessed/uncursed first */
	req.beatitude = BLESSED | UNCURSED;
	req.sustain = false; // drop the following weapons if we find better weapons
	req.data = "long sword";
	req.priority = 15;
	saiph->request(req);
	/* Shop analyzer is deactivated for the time being
	req.data = "pick-axe";
	req.priority = 13;
	saiph->request(req);
	*/
	req.data = "silver dagger";
	req.priority = 10;
	saiph->request(req);
	req.data = "elven dagger";
	req.priority = 10;
	saiph->request(req);
	req.data = "dagger";
	req.priority = 9;
	saiph->request(req);
	req.data = "orcish dagger";
	req.priority = 9;
	saiph->request(req);
	/* finally fall back to weapons of unknown beatitude */
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.data = "long sword";
	req.priority = 8;
	saiph->request(req);
	/* Shop analyzer is deactivated for the time being
	req.data = "pick-axe";
	req.priority = 7;
	saiph->request(req);
	*/
	req.data = "silver dagger";
	req.priority = 6;
	saiph->request(req);
	req.data = "elven dagger";
	req.priority = 5;
	saiph->request(req);
	req.data = "dagger";
	req.priority = 4;
	saiph->request(req);
	req.data = "orcish dagger";
	req.priority = 3;
	saiph->request(req);
	/* we won't use these weapons, we just pick them up for fun */
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED | CURSED;
	req.sustain = true; // don't drop them
	req.priority = 0;
	req.data = "Cleaver";
	saiph->request(req);
	req.data = "Giantslayer";
	saiph->request(req);
	req.data = "Grimtooth";
	saiph->request(req);
	/* chaotic quest artifact, will evade our grasp
	req.data = "Longbow of Diana";
	saiph->request(req);
	*/
	req.data = "Magicbane";
	saiph->request(req);
	req.data = "Ogresmasher";
	saiph->request(req);
	req.data = "Orcrist";
	saiph->request(req);
	req.data = "Sceptre of Might";
	saiph->request(req);
	/* neutral quest artifact, will evade our grasp
	req.data = "Staff of Aesculapius";
	saiph->request(req);
	*/
	req.data = "Sting";
	saiph->request(req);
	req.data = "Stormbringer";
	saiph->request(req);
	req.data = "Trollsbane";
	saiph->request(req);
	req.data = "Tsurugi of Muramasa";
	saiph->request(req);
	req.data = "Vorpal Blade";
	saiph->request(req);


	/* FIXME: make an own function for thrown weapons */
	/* create a group for thrown weapons */
	req.request = REQUEST_ITEM_GROUP_SET_AMOUNT;
	req.key = loot_group++;
	req.value = 15;
	saiph->request(req);
	/* add weapons to the group, best weapon first */
	req.request = REQUEST_ITEM_GROUP_ADD;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
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

	/* set other weapons we wish to pick up that is not in a group */
	req.request = REQUEST_ITEM_PICKUP;
	req.beatitude = BEATITUDE_UNKNOWN | BLESSED | UNCURSED;
	req.only_unknown_enchantment = false;
	req.data = "dart";
	req.value = 50;
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
