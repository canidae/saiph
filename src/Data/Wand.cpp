#include "Data/Wand.h"
#include "Globals.h"

using namespace data;

std::map<const std::string, const Wand*> Wand::_wands;

Wand::Wand(const std::string& name, int cost, int material, int maximum_charges, int zap_type, const std::string& engrave_message, unsigned long long properties) : Item(name, cost, 7, WAND, material, properties), _maximum_charges(maximum_charges), _zap_type(zap_type), _engrave_message(engrave_message) {
}

Wand::~Wand() {
}

void Wand::init() {
	create("wand of light", 100, MATERIAL_UNKNOWN, 18, WAND_ZAP_TYPE_NONDIRECTIONAL, WAND_LIGHT_MESSAGE, PROPERTY_MAGIC);
	create("wand of nothing", 100, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, "", 0);
	create("wand of digging", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_RAY, WAND_DIGGING_MESSAGE, PROPERTY_MAGIC);
	create("wand of enlightenment", 150, MATERIAL_UNKNOWN, 15, WAND_ZAP_TYPE_NONDIRECTIONAL, WAND_ENLIGHTENMENT_MESSAGE, PROPERTY_MAGIC);
	create("wand of locking", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, "", PROPERTY_MAGIC);
	create("wand of magic missile", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_RAY, WAND_MAGIC_MISSILE_MESSAGE, PROPERTY_MAGIC);
	create("wand of make invisible", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, WAND_VANISHER_MESSAGE, PROPERTY_MAGIC);
	create("wand of opening", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, "", PROPERTY_MAGIC);
	create("wand of probing", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, "", PROPERTY_MAGIC);
	create("wand of secret door detection", 150, MATERIAL_UNKNOWN, 15, WAND_ZAP_TYPE_NONDIRECTIONAL, "", PROPERTY_MAGIC);
	create("wand of slow monster", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, WAND_SLOW_MONSTER_MESSAGE, PROPERTY_MAGIC);
	create("wand of speed monster", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, WAND_SPEED_MONSTER_MESSAGE, PROPERTY_MAGIC);
	create("wand of striking", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, WAND_STRIKING_MESSAGE, PROPERTY_MAGIC);
	create("wand of undead turning", 150, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, "", PROPERTY_MAGIC);
	create("wand of cold", 175, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_RAY, WAND_COLD_MESSAGE, PROPERTY_MAGIC);
	create("wand of fire", 175, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_RAY, WAND_FIRE_MESSAGE, PROPERTY_MAGIC);
	create("wand of lightning", 175, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_RAY, WAND_LIGHTNING_MESSAGE, PROPERTY_MAGIC);
	create("wand of sleep", 175, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_RAY, WAND_SLEEP_DEATH_MESSAGE, PROPERTY_MAGIC);
	create("wand of cancellation", 200, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, WAND_VANISHER_MESSAGE, PROPERTY_MAGIC);
	create("wand of create monster", 200, MATERIAL_UNKNOWN, 15, WAND_ZAP_TYPE_NONDIRECTIONAL, WAND_CREATE_MONSTER_MESSAGE, PROPERTY_MAGIC);
	create("wand of polymorph", 200, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, WAND_POLYMORPH_MESSAGE, PROPERTY_MAGIC);
	create("wand of teleportation", 200, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_BEAM, WAND_VANISHER_MESSAGE, PROPERTY_MAGIC);
	create("wand of death", 500, MATERIAL_UNKNOWN, 8, WAND_ZAP_TYPE_RAY, WAND_SLEEP_DEATH_MESSAGE, PROPERTY_MAGIC);
	create("wand of wishing", 500, MATERIAL_UNKNOWN, 3, WAND_ZAP_TYPE_NONDIRECTIONAL, WAND_WISHING_MESSAGE, PROPERTY_MAGIC);

	/* random appearances */
	create("glass wand", 0, MATERIAL_GLASS, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("balsa wand", 0, MATERIAL_WOOD, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("crystal wand", 0, MATERIAL_GLASS, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("maple wand", 0, MATERIAL_WOOD, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("pine wand", 0, MATERIAL_WOOD, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("oak wand", 0, MATERIAL_WOOD, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("ebony wand", 0, MATERIAL_WOOD, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("marble wand", 0, MATERIAL_MINERAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("tin wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("brass wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("copper wand", 0, MATERIAL_COPPER, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("silver wand", 0, MATERIAL_SILVER, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("platinum wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("iridium wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("zinc wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("aluminum wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("uranium wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("iron wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("steel wand", 0, MATERIAL_METAL, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("hexagonal wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("short wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("runed wand", 0, MATERIAL_WOOD, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("long wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("curved wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("forked wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("spiked wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
	create("jeweled wand", 0, MATERIAL_IRON, 0, WAND_ZAP_TYPE_UNKNOWN, "", PROPERTY_RANDOM_APPEARANCE | PROPERTY_MAGIC);
}

const std::map<const std::string, const Wand*>& Wand::wands() {
	return _wands;
}

int Wand::maximumCharges() const {
	return _maximum_charges;
}

int Wand::zapType() const {
	return _zap_type;
}

const std::string& Wand::engraveMessage() const {
	return _engrave_message;
}

void Wand::addToMap(const std::string& name, const Wand* wand) {
	_wands[name] = wand;
	Item::addToMap(name, wand);
}

void Wand::create(const std::string& name, int cost, int material, int maximum_charges, int zap_type, const std::string& engrave_message, unsigned long long properties) {
	addToMap(name, new Wand(name, cost, material, maximum_charges, zap_type, engrave_message, properties));
}
