#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

#include "Action.h"

#define MOVE_COST_MEDIAN 50

namespace action {

	class Move : public Action {
	public:
		static const int ID;

		Move(analyzer::Analyzer* analyzer, const unsigned char& direction, const int& priority) : Action(analyzer), _move(std::string(1, direction), priority) {
		}

		virtual ~Move() {
		}

		static int calculatePriority(const int& max_priority, const int& moves) {
			return max_priority * MOVE_COST_MEDIAN / (moves + MOVE_COST_MEDIAN - 1);
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _move;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			++_sequence;
		}

	private:
		const Command _move;
	};
}
#endif
