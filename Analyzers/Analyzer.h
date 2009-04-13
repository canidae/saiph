#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

namespace action {
	class Action;
}
namespace event {
	class Event;
}
class Command;
class Point;
class Request;

class Analyzer {
public:
	std::string name;
	action::Action *action;

	Analyzer(std::string name) : name(name), action(NULL) {}
	virtual ~Analyzer();

	virtual void analyze(const Command &) {};
	virtual void init() {};
	virtual void parseMessages(const std::string &, const Command &) {};
	virtual void handle(event::Event *const) {};
};
#endif
