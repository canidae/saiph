#ifndef ACTION_ACTION_H
#define ACTION_ACTION_H

#include <string>
#include "Command.h"

// Classify time requirements of various actions for sub_turn tracking:
#define TIME_NONE      0  // No time at all (farlook, etc)
#define TIME_STANDARD  1  // Most actions take one move
#define TIME_MULTIPLE  2  // Takes multiple actions and can be interrupted at any point, like eating - we know nothing afterward
#define TIME_NOMUL     3  // Calls nomul() with a negative argument, consuming an integral number of turns

// Note that if a TIME_STANDARD action takes multiple turns, it is considered TIME_NOMUL because saiph assumes she was paralyzed.

#define NO_ACTION 0

namespace analyzer {
	class Analyzer;
}

namespace action {

	class Action {
	public:
		static const Command NOOP;

		Action(analyzer::Analyzer* analyzer, int time_taken = TIME_STANDARD);
		virtual ~Action();

		virtual int id() = 0;
		analyzer::Analyzer* analyzer();
		int timeTaken();
		virtual const Command& command() = 0;
		virtual void update(const std::string& messages) = 0;
		virtual void failed();

	protected:
		int _sequence;
		int _time_taken;

	private:
		analyzer::Analyzer* _analyzer;
	};
}
#endif
