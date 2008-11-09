#include "Debug.h"

using namespace std;

/* static variables */
bool Debug::initialized = false;
string Debug::timestamp;
ofstream Debug::debugfile;

/* static methods */
bool Debug::close() {
	if (!initialized)
		return true;
	debugfile.close();
	initialized = false;
	return true;
}

ofstream &Debug::error() {
	debugfile << "[" << printTime() << "] [ERROR  ] ";
	return debugfile;
}

ofstream &Debug::info() {
	debugfile << "[" << printTime() << "] [INFO   ] ";
	return debugfile;
}

ofstream &Debug::notice() {
	debugfile << "[" << printTime() << "] [NOTICE ] ";
	return debugfile;
}

bool Debug::open(const string &file) {
	if (initialized)
		close();
	initialized = true;
	debugfile.open(file.c_str(), ios_base::trunc);
	return true;
}

ofstream &Debug::warning() {
	debugfile << "[" << printTime() << "] [WARNING] ";
	return debugfile;
}

/* private static methods */
string &Debug::printTime() {
	time_t rawtime;
	time(&rawtime);
	timestamp = asctime(localtime(&rawtime));
	return timestamp.erase(timestamp.size() - 1);
}
