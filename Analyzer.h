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

	Analyzer(std::string name);
	virtual ~Analyzer();

	virtual void analyze(const Command &best_command);
	virtual void init();
	virtual void parseMessages(const std::string &messages, const Command &best_command);
	virtual void handle(event::Event *const event);
};
#endif
