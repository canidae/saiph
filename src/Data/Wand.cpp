#include "Data/Wand.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Wand*> Wand::_wands;

/* protected constructors */
Wand::Wand(const string& name, int cost, int weight, int material, unsigned long long properties) : Item(name, cost, weight, WAND, material, properties), _recharge(recharge), _charges(charges) {
}

/* destructor */
Wand::~Wand() {
}

/* public static methods */
void Wand::init() {
	/* wands */
	create("wand of light", 100, 7, 0, 0, 0, 30);
	create("wand of nothing", 100, 7, 0, 0, 0, 30);
	create("wand of digging", 150, 7, 0, 0, 0, 30);
	create("wand of enlightenment", 150, 7, 0, 0, 0, 30);
	create("wand of locking", 150, 7, 0, 0, 0, 30);
	create("wand of magic missile", 150, 7, 0, 0, 0, 30);
	create("wand of make invisible", 150, 7, 0, 0, 0, 30);
	create("wand of opening", 150, 7, 0, 0, 0, 30);
	create("wand of probing", 150, 7, 0, 0, 0, 30);
	create("wand of secret door detection", 150, 7, 0, 0, 0, 30);
	create("wand of slow monster", 150, 7, 0, 0, 0, 30);
	create("wand of speed monster", 150, 7, 0, 0, 0, 30);
	create("wand of striking", 150, 7, 0, 0, 0, 30);
	create("wand of undead turning", 150, 7, 0, 0, 0, 30);
	create("wand of cold", 175, 7, 0, 0, 0, 30);
	create("wand of fire", 175, 7, 0, 0, 0, 30);
	create("wand of lightning", 175, 7, 0, 0, 0, 30);
	create("wand of sleep", 175, 7, 0, 0, 0, 30);
	create("wand of cancellation", 200, 7, 0, 0, 0, 30);
	create("wand of create monster", 200, 7, 0, 0, 0, 30);
	create("wand of polymorph", 200, 7, 0, 0, 0, 30);
	create("wand of teleportation", 200, 7, 0, 0, 0, 30);
	create("wand of death", 500, 7, 0, 0, 0, 30);
	create("wand of wishing", 500, 7, 0, 0, 0, 30);

	/* random appearance wands */
	create("glass wand", 0, 7, MATERIAL_GLASS, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("crystal wand", 0, 7, MATERIAL_GLASS, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("oak wand", 0, 7, MATERIAL_WOOD, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("pine wand", 0, 7, MATERIAL_WOOD, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("balsa wand", 0, 7, MATERIAL_WOOD, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("ebony wand", 0, 7, MATERIAL_WOOD, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("maple wand", 0, 7, MATERIAL_WOOD, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("copper wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("aluminum wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("short wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("spiked wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("silver wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("uranium wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("runed wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("jeweled wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("marble wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("platinum wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("long wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("tin wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("iridium wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("steel wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("curved wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("brass wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("zinc wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("hexagonal wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("forked wand", 0, 7, MATERIAL_METALLIC, PROPERTY_RANDOM_APPEARANCE, 0, 30);
	create("iron wand", 0, 7, MATERIAL_IRON, PROPERTY_RANDOM_APPEARANCE, 0, 30);
}

const map<const string, const Wand*>& Wand::wands() {
	return _wands;
}

/* protected static methods */
void Wand::addToMap(const string& name, const Wand* wand) {
	_wands[name] = wand;
	Item::addToMap(name, wand);
}

/* private static methods */
void Wand::create(const string& name, int cost, int weight, int material, unsigned long long properties) {
	addToMap(name, new Wand(name, cost, weight, material, properties));
}
