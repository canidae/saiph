#include "Key.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Key *> Key::keys;

Key::Key(const string &name, int cost, int weight, int material, unsigned long long properties) : Tool(name, cost, weight, material, properties) {
}

void Key::addToMap(const string &name, Key *key) {
	Key::keys[name] = key;
	Tool::addToMap(name, key);
}

void Key::create(const string &name, int cost, int weight, int material, unsigned long long properties) {
	addToMap(name, new Key(name, cost, weight, material, properties));
}

void Key::init() {
	/* keys */
	create("Master Key of Thievery", 3500, 3, MATERIAL_METAL, PROPERTY_ARTIFACT);
	create("skeleton key", 10, 3, MATERIAL_METAL, 0);
	create("lock pick", 20, 4, MATERIAL_METAL, 0);
	create("credit card", 10, 1, MATERIAL_METAL, 0);

	/* aliases */
	addToMap("key", keys["skeleton key"]);
}
