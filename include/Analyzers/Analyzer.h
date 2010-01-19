#ifndef ANALYZER_ANALYZER_H
#define ANALYZER_ANALYZER_H

#include <string>
#include <vector>

#define ILLEGAL_ANALYZER_ID -1

namespace event {
	class Event;
}

namespace analyzer {

	class Analyzer {
	public:
		Analyzer(const std::string& name);
		virtual ~Analyzer();

		static void init();
		static void destroy();
		const std::string& name();
		virtual void parseMessages(const std::string&);
		virtual void analyze();
		virtual void onEvent(event::Event * const);
		virtual void actionCompleted();
		virtual void actionFailed();
		// this overload defaults to calling the parameter-less version
		virtual void actionCompleted(const std::string& messages);

	private:
		static std::vector<Analyzer*> _analyzers;
		std::string _name;
	};
}
#endif
