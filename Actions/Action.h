#ifndef _ACTION_H
#define _ACTION_H

#include <string>
#include "../Command.h"

#define ILLEGAL_ACTION_ID -1

namespace analyzer {
	class Analyzer;
}

namespace action {
	class Action {
	public:
		static const Command noop;

		Action(analyzer::Analyzer *analyzer) : sequence(0), analyzer(analyzer) {}
		virtual ~Action() {}

		static void init();
		static void destroy();
		virtual int getID() = 0;
		virtual analyzer::Analyzer *getAnalyzer() {return analyzer;}
		virtual const Command &getCommand() = 0;
		virtual void updateAction(const std::string &messages) = 0;

	protected:
		int sequence;

	private:
		static bool initialized;
		analyzer::Analyzer *analyzer;
	};
}
#endif
