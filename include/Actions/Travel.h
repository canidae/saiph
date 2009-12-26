#ifndef ACTION_TRAVEL_H
#define ACTION_TRAVEL_H

#include "Actions/Action.h"

#define TRAVEL_WHERE_TO_GO "  Where do you want to travel to?  "

namespace action {

	class Travel : public Action {
	public:
		static const int ID;

		Travel(analyzer::Analyzer* analyzer, const Tile& target, int priority) : Action(analyzer), _target(target), _travel_target(Action::NOOP) {
			bool travel = true;
			if (_target.distance() <= 1 || _travel_ban > 0) {
				--_travel_ban;
				travel = false;
			} else {
				for (std::map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
					if (m->second.visible()) {
						travel = false;
						break;
					}
				}
			}
			if (travel)
				_do_travel = Command("_", priority);
			else
				_do_travel = Command(_target.direction(), priority);
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
				_travel_target = Command(World::cursorMoves(World::cursor(), _target.coordinate()) + ".", PRIORITY_CONTINUE_ACTION);
				_sequence = 1;
			} else {
				_sequence = 2;
			}
		}

		virtual void failed() {
			/* probably can't use travel to get somewhere for some reason */
			_travel_ban = 8;
		}

	private:
		static int _travel_ban;
		Tile _target;
		Command _do_travel;
		Command _travel_target;
	};
}
#endif
