#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

#include "Action.h"

#define MOVE_COST_MEDIAN 200

namespace action {

	class Move : public Action {
	public:
		static const int ID;

		Move(analyzer::Analyzer* analyzer, unsigned char direction, int priority) : Action(analyzer), _move(direction, priority) {
		}

		virtual ~Move() {
		}

		static int calculatePriority(int max_priority, unsigned int cost) {
			return max_priority * MOVE_COST_MEDIAN / (cost + MOVE_COST_MEDIAN - 1);
		}

		virtual int id() {
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
			if (_sequence == 0)
				_sequence = 1;
		}

	private:
		const Command _move;
	};
}
#endif
