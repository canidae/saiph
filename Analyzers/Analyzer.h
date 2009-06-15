#ifndef ANALYZER_ANALYZER_H
#define ANALYZER_ANALYZER_H

#include <string>

#define ILLEGAL_ANALYZER_ID -1

namespace event {
	class Event;
}

namespace analyzer {
	class Analyzer {
	public:
		std::string name;

		Analyzer(std::string name) : name(name) {}
		virtual ~Analyzer() {}

		static void init();
		static void destroy();
		virtual void parseMessages(const std::string &) {}
		virtual void analyze() {}
		virtual void onEvent(event::Event *const) {}

	private:
		static bool initialized;
	};
}
#endif
