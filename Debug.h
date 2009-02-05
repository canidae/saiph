#ifndef DEBUG_H
#define DEBUG_H

extern "C" {
#include <sys/stat.h>
}
#include <fstream>
#include <iostream>
#include <string>

class Debug {
	public:
		static std::ofstream debugfile;

		static bool close();
		static bool open(const std::string &file);
		static std::ofstream &error();
		static std::ofstream &info();
		static std::ofstream &notice();
		static std::ofstream &warning();
		static std::ofstream &error(int turn);
		static std::ofstream &info(int turn);
		static std::ofstream &notice(int turn);
		static std::ofstream &warning(int turn);

	private:
		static bool initialized;
		static std::string timestamp;

		static std::string &printTime();
};
#endif
