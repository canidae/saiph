#ifndef ACTION_ACTION_H
#define ACTION_ACTION_H

#include <string>
#include "../Command.h"
#include "../Analyzers/Analyzer.h"

#define NO_ACTION 0

namespace action {

	class Action {
	public:
		static const Command NOOP;

		Action(analyzer::Analyzer* analyzer);
		virtual ~Action();

		virtual const int& id() = 0;
		analyzer::Analyzer* analyzer();
		virtual const Command& command() = 0;
		virtual void update(const std::string& messages) = 0;

	protected:
		int _sequence;

	private:
		analyzer::Analyzer* _analyzer;
	};
}
#endif
