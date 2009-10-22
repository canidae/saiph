#include "Debug.h"
#include "World.h"

#define DEBUG_CATEGORY_MAX_LENGTH 9
#define DEBUG_COMMAND "Command"
#define DEBUG_EVENT "Broadcast"
#define DEBUG_INVENTORY "Inventory"
#define DEBUG_PATHING "Pathing"
#define DEBUG_INFO "Info"
#define DEBUG_NOTICE "Notice"
#define DEBUG_WARNING "Warning"
#define DEBUG_ERROR "Error"
#define DEBUG_DATA "Data"

using namespace std;

/* define static variables */
string Debug::_timestamp;
ofstream Debug::_debugfile;

/* methods */
void Debug::init(const string& file) {
	_debugfile.open(file.c_str(), ios_base::trunc);
}

void Debug::destroy() {
	_debugfile.close();
}

ofstream& Debug::analyzer(const string& name) {
	return header(name);
}

ofstream& Debug::command() {
	return header(DEBUG_COMMAND);
}

ofstream& Debug::event() {
	return header(DEBUG_EVENT);
}

ofstream& Debug::inventory() {
	return header(DEBUG_INVENTORY);
}

ofstream& Debug::pathing() {
	return header(DEBUG_PATHING);
}

ofstream& Debug::info() {
	return header(DEBUG_INFO);
}

ofstream& Debug::notice() {
	return header(DEBUG_NOTICE);
}

ofstream& Debug::warning() {
	return header(DEBUG_WARNING);
}

ofstream& Debug::error() {
	return header(DEBUG_ERROR);
}

void Debug::rawCharArray(const char* data, int start, int stop) {
	header(DEBUG_DATA);
	for (int a = start; a < stop; ++a)
		_debugfile << data[a];
	_debugfile << endl;
}

/* private methods */
ofstream& Debug::header(const string &category) {
	_debugfile << "<T" << World::turn << "> " << printTime() << " [";
	if (category.size() > DEBUG_CATEGORY_MAX_LENGTH)
		_debugfile << category.substr(0, DEBUG_CATEGORY_MAX_LENGTH);
	else
		_debugfile << category << string(DEBUG_CATEGORY_MAX_LENGTH - category.size(), ' ');
	_debugfile << "] ";
	return _debugfile;
}

string& Debug::printTime() {
	time_t rawtime;
	time(&rawtime);
	_timestamp = asctime(localtime(&rawtime));
	return _timestamp.erase(_timestamp.size() - 1);
}