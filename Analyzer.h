#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include "Actions/Action.h"
#include "Events/Event.h"

class Command;
class Point;
class Request;

class Analyzer {
	public:
		std::string name;
		action::Action *action;
		bool expired; // TODO: rather send a signal when the analyzer is expired
	
		Analyzer(std::string name);
		virtual ~Analyzer();

		virtual void analyze(const Command &command);
		virtual void complete(); // TODO: die (replaced by actions)
		virtual void fail(); // TODO: die? (replaced by actions?)
		virtual void init();
		virtual void parseMessages(const std::string &messages, const Command &command);
		virtual bool request(const Request &request); // TODO: die (replaced by event system)
		virtual void handle(const Event *event);
};
#endif
