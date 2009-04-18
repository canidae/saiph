#ifndef DEBUG_H
#define DEBUG_H

extern "C" {
#include <sys/stat.h>
}
#include <fstream>
#include <iostream>
#include <string>
#include "World.h"

class Debug {
	public:
		static void init(const std::string &file);
		static void destroy();
		static std::ofstream &error();
		static std::ofstream &info();
		static std::ofstream &notice();
		static std::ofstream &warning();

	private:
		static std::ofstream debugfile;
		static std::string timestamp;

		static std::string &printTime();
};

/* inline methods */
inline void Debug::init(const std::string &file) {
        debugfile.open(file.c_str(), std::ios_base::trunc);
}

inline void Debug::destroy() {
        debugfile.close();
}

inline std::ofstream &Debug::error() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [ERROR:";
        return debugfile;
}

inline std::ofstream &Debug::info() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [INFO:";
        return debugfile;
}

inline std::ofstream &Debug::notice() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [NOTICE:";
        return debugfile;
}

inline std::ofstream &Debug::warning() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [WARNING:";
        return debugfile;
}

/* inline private methods */
inline std::string &Debug::printTime() {
	time_t rawtime;
	time(&rawtime);
	timestamp = asctime(localtime(&rawtime));
	return timestamp.erase(timestamp.size() - 1);
}

/* init static variables */
std::string Debug::timestamp;
std::ofstream Debug::debugfile;
#endif
