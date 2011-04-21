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

		Action(analyzer::Analyzer* analyzer, bool increase_turn_counter = true);
		virtual ~Action();

		virtual int id() = 0;
		analyzer::Analyzer* analyzer();
		bool increaseTurnCounter();
		virtual const Command& command() = 0;
		virtual void update(const std::string& messages) = 0;
		virtual void failed();

	protected:
		int _sequence;

	private:
		analyzer::Analyzer* _analyzer;
		bool _increase_turn_counter;
	};
}
#endif
