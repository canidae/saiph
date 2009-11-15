#include "Javelin.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Javelin*> Javelin::_javelins;

/* protected constructors */
Javelin::Javelin(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Javelin::~Javelin() {
}

/* public static methods */
void Javelin::init() {
	/* javelins */
	create("javelin", 3, 20, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("throwing spear", _javelins["javelin"]);
}

const map<const string, const Javelin*>& Javelin::javelins() {
	return _javelins;
}

/* protected static methods */
void Javelin::addToMap(const string& name, const Javelin* javelin) {
	_javelins[name] = javelin;
	Weapon::addToMap(name, javelin);
}

/* private static methods */
void Javelin::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Javelin(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
