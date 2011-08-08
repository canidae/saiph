#ifndef ACTION_ACTION_H
#define ACTION_ACTION_H

#include <string>
#include "Command.h"

#define NO_ACTION 0

namespace analyzer {
	class Analyzer;
}

namespace action {

	class Action {
	public:
		static const Command NOOP;

		// time_taken values correspond to NetHack nomul values, 1,2,3, counts in moves, -1,-2,-3 count in turns
		// values of 2 or more are counted as "indeterminate" due to interruption potential
		Action(analyzer::Analyzer* analyzer, int time_taken = 1);
		virtual ~Action();

		virtual int id() = 0;
		analyzer::Analyzer* analyzer();
		int timeTaken();
		virtual const Command& command() = 0;
		virtual void update(const std::string& messages) = 0;
		virtual void failed();

	protected:
		int _sequence;

	private:
		analyzer::Analyzer* _analyzer;
		int _time_taken;
	};
}
#endif
