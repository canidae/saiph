#include "Data/Tool.h"

#include "Data/Key.h"
#include "Data/Lamp.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Tool*> Tool::_tools;

/* protected constructors */
Tool::Tool(const string& name, int cost, int weight, int material, unsigned long long properties) : Item(name, cost, weight, TOOL, material, properties) {
}

/* destructor */
Tool::~Tool() {
}

/* public static methods */
void Tool::init() {
	Key::init();
	Lamp::init();
}

const map<const string, const Tool*>& Tool::tools() {
	return _tools;
}

/* protected static methods */
void Tool::addToMap(const string& name, const Tool* tool) {
	_tools[name] = tool;
	Item::addToMap(name, tool);
}
