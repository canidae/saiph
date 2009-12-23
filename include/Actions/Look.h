#ifndef ACTION_LOOK_H
#define ACTION_LOOK_H

#include "Actions/Action.h"

namespace action {

	class Look : public Action {
	public:
		static const int ID;

		Look(analyzer::Analyzer* analyzer) : Action(analyzer, false), _look(":", PRIORITY_LOOK) {
		}

		virtual ~Look() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _look;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			if (_sequence == 0)
				_sequence = 1;
		}

	private:
		const Command _look;
	};
}
#endif
