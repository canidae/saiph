#include "Key.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Key*> Key::_keys;

/* protected constructors */
Key::Key(const string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties) : Tool(name, cost, weight, material, properties) {
}

/* destructor */
Key::~Key() {
}

/* public static methods */
void Key::init() {
	/* keys */
	create("skeleton key", 10, 3, MATERIAL_METAL, 0);
	create("lock pick", 20, 4, MATERIAL_METAL, 0);
	create("credit card", 10, 1, MATERIAL_METAL, 0);

	/* aliases */
	addToMap("key", _keys["skeleton key"]);

	/* artifact keys */
	create("Master Key of Thievery", 3500, 3, MATERIAL_METAL, PROPERTY_ARTIFACT);
}

const map<const string, const Key*>& Key::keys() {
	return _keys;
}

/* protected static methods */
void Key::addToMap(const string& name, const Key* key) {
	_keys[name] = key;
	Tool::addToMap(name, key);
}

/* private static methods */
void Key::create(const string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties) {
	addToMap(name, new Key(name, cost, weight, material, properties));
}