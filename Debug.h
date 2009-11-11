#ifndef DEBUG_H
#define DEBUG_H

extern "C" {
#include <sys/stat.h>
}
#include <fstream>
#include <string>

class Debug {
public:
	static void init(const std::string& file);
	static void destroy();
	static std::ofstream& custom(const std::string& name);
	static std::ofstream& command();
	static std::ofstream& event();
	static std::ofstream& inventory();
	static std::ofstream& pathing();
	static std::ofstream& info();
	static std::ofstream& notice();
	static std::ofstream& warning();
	static std::ofstream& error();
	static void rawCharArray(const char* data, int start, int stop);

private:
	static std::ofstream _debugfile;
	static std::string _timestamp;

	static std::ofstream& header(const std::string& category);
	static std::string& printTime();
};
#endif
