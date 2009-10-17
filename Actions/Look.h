#ifndef ACTION_LOOK_H
#define ACTION_LOOK_H

#include "Action.h"

namespace action {

	class Look : public Action {
	public:
		static int id;

		Look(analyzer::Analyzer *analyzer) : Action(analyzer), look(":", PRIORITY_LOOK) {
		}

		virtual ~Look() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command look;
	};

	inline const Command &action::Look::command() {
		switch (sequence) {
		case 0:
			return look;

		default:
			return Action::NOOP;
		}
	}

	inline void action::Look::update(const std::string &) {
		if (sequence == 0)
			sequence = 1;
	}
}
#endif
