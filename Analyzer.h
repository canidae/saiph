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
		virtual void complete();
		virtual void fail();
		virtual std::string &getCommand();
		virtual int getPriority();
		virtual void init();
		virtual void inspect(const Point &point);
		virtual void parseMessages(const std::string &messages);
		virtual bool request(const Request &request);

	protected:
		std::vector<Command> commands;
		int sequence;

		virtual bool setCommand(int index, int priority, const std::string &data);
};
#endif
