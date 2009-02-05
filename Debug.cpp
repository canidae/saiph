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

bool Debug::open(const string &file) {
	if (initialized)
		close();
	initialized = true;
	debugfile.open(file.c_str(), ios_base::trunc);
	return true;
}

ofstream &Debug::error() {
	debugfile << "<T-> " << printTime() << ": [ERROR:";
	return debugfile;
}

ofstream &Debug::info() {
	debugfile << "<T-> " << printTime() << ": [INFO:";
	return debugfile;
}

ofstream &Debug::notice() {
	debugfile << "<T-> " << printTime() << ": [NOTICE:";
	return debugfile;
}

ofstream &Debug::warning() {
	debugfile << "<T-> " << printTime() << ": [WARNING:";
	return debugfile;
}

/* Surcharging previous functions to add turncount */
ofstream &Debug::error(int turn) {
	debugfile << "<T" << turn << "> " << printTime() << ": [ERROR:";
	return debugfile;
}

ofstream &Debug::info(int turn) {
	debugfile << "<T" << turn << "> " << printTime() << ": [INFO:";
	return debugfile;
}

ofstream &Debug::notice(int turn) {
	debugfile << "<T" << turn << "> " << printTime() << ": [NOTICE:";
	return debugfile;
}

ofstream &Debug::warning(int turn) {
	debugfile << "<T" << turn << "> " << printTime() << ": [WARNING:";
	return debugfile;
}


/* private static methods */
string &Debug::printTime() {
	time_t rawtime;
	time(&rawtime);
	timestamp = asctime(localtime(&rawtime));
	return timestamp.erase(timestamp.size() - 1);
}
