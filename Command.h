#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include "Globals.h"

class Command {
	public:
		int priority;
		std::string data;
		bool remember;
	
		Command(int priority = ILLEGAL_PRIORITY, const std::string &data = "", bool remember = false);
		~Command();

		void setCommand(int priority, const std::string &data, bool remember = false);
};
#endif
