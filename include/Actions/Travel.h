#ifndef ACTION_TRAVEL_H
#define ACTION_TRAVEL_H

#include "Actions/Action.h"

#define TRAVEL_WHERE_TO_GO "Where do you want to travel to?"

namespace action {

	class Travel : public Action {
	public:
		static const int ID;

		Travel(analyzer::Analyzer* analyzer, const Point &location, int priority) : Action(analyzer), _location(location), _do_travel("_", priority), _travel_target(Action::NOOP) {
		}

		virtual ~Travel() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_travel;

			case 1:
				return _travel_target;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(TRAVEL_WHERE_TO_GO) != std::string::npos) {
				_travel_target = Command(World::cursorMoves(World::cursor(), _location) + ".", PRIORITY_CONTINUE_ACTION);
				_sequence = 1;
			} else if (_sequence == 1) {
				_sequence = 2;
			}
		}

	private:
		const Point _location;
		const Command _do_travel;
		Command _travel_target;
	};
}
#endif
