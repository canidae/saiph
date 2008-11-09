#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>
#include "Command.h"

class Point;
class Request;

class Analyzer {
	public:
		std::string name;
	
		Analyzer(std::string name);
		virtual ~Analyzer();

		virtual void analyze();
		void clearCommands();
		virtual void complete();
		virtual void fail();
		std::string &getCommand();
		int getPriority();
		virtual void init();
		virtual void inspect(const Point &point);
		virtual void parseMessages(const std::string &messages);
		bool rememberCommand();
		virtual bool request(const Request &request);
		bool wasLastCommand();

	protected:
		std::vector<Command> commands;
		int sequence;

		bool setCommand(int index, int priority, const std::string &data, bool remember = false);

	private:
		int last_sequence;
};
#endif
