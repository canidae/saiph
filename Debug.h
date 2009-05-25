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
	static std::ofstream &analyzer(const std::string &name);
	static std::ofstream &info();
	static std::ofstream &notice();
	static std::ofstream &warning();
	static std::ofstream &error();
	static void rawCharArray(const char *data, int start, int stop);

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

inline std::ofstream &Debug::analyzer(const std::string &name) {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [" << name << "] ";
        return debugfile;
}

inline std::ofstream &Debug::info() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [Info] ";
        return debugfile;
}

inline std::ofstream &Debug::notice() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [Notice] ";
        return debugfile;
}

inline std::ofstream &Debug::warning() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [Warning] ";
        return debugfile;
}

inline std::ofstream &Debug::error() {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [Error] ";
        return debugfile;
}

inline void Debug::rawCharArray(const char *data, int start, int stop) {
        debugfile << "<T" << World::turn << "> " << printTime() << ": [Data] ";
	for (int a = start; a < stop; ++a)
		debugfile << data[a];
	debugfile << std::endl;
}

/* inline private methods */
inline std::string &Debug::printTime() {
	time_t rawtime;
	time(&rawtime);
	timestamp = asctime(localtime(&rawtime));
	return timestamp.erase(timestamp.size() - 1);
}
#endif
