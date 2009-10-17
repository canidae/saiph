#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

#include "Action.h"

#define MOVE_COST_MEDIAN 50

namespace action {

	class Move : public Action {
	public:
		static int id;

		Move(analyzer::Analyzer *analyzer, unsigned char direction, int priority) : Action(analyzer), move(std::string(1, direction), priority) {
		}

		virtual ~Move() {
		}

		static int calculatePriority(int max_priority, int moves) {
			return max_priority * MOVE_COST_MEDIAN / (moves + MOVE_COST_MEDIAN - 1);
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command move;
	};

	inline const Command &action::Move::command() {
		switch (sequence) {
		case 0:
			return move;

		default:
			return Action::NOOP;
		}
	}

	inline void action::Move::update(const std::string &) {
		++sequence;
	}
}
#endif
