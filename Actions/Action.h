#ifndef ACTION_ACTION_H
#define ACTION_ACTION_H

#include <string>
#include "../Command.h"

#define NO_ACTION 0

namespace analyzer {
	class Analyzer;
}

namespace action {

	class Action {
	public:
		static const Command NOOP;

		Action(analyzer::Analyzer *analyzer) : _sequence(0), _analyzer(analyzer) {
		}

		virtual ~Action() {
		}

		virtual int id() = 0;

		virtual analyzer::Analyzer *analyzer() {
			return _analyzer;
		}
		virtual const Command &command() = 0;
		virtual void update(const std::string &messages) = 0;

	protected:
		int _sequence;

	private:
		analyzer::Analyzer *_analyzer;
	};
}
#endif
