#include "Tool.h"
#include "Key.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Tool *> Tool::tools;

Tool::Tool(const string &name, int cost, int weight, int material, unsigned long long properties) : Item(name, cost, weight, TOOL, material, properties) {
}

void Tool::addToMap(const string &name, Tool *tool) {
	Tool::tools[name] = tool;
	Item::addToMap(name, tool);
}

void Tool::init() {
	Key::init();
}
