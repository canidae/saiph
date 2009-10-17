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

		Analyzer(std::string name) : _name(name) {
		}

		virtual ~Analyzer() {
		}

		const std::string &name() {
			return _name;
		}

		static void init();
		static void destroy();

		virtual void parseMessages(const std::string &) {
		}

		virtual void analyze() {
		}

		virtual void onEvent(event::Event * const) {
		}

	private:
		std::string _name;
		static bool _initialized;
	};
}
#endif
