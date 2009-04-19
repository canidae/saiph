#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

namespace event {
	class Event;
}

namespace analyzer {
	class Analyzer {
	public:
		std::string name;

		Analyzer(std::string name) : name(name) {}
		virtual ~Analyzer() {}

		virtual void parseMessages(const std::string &) {}
		virtual void analyze() {}
		virtual void onEvent(event::Event *const) {}
	};
}
#endif
