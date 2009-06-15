#ifndef ACTION_LOOK_H
#define ACTION_LOOK_H

#include "Action.h"

namespace action {
	class Look : public Action {
	public:
		static int id;

		Look(analyzer::Analyzer *analyzer) : Action(analyzer), look(":", PRIORITY_LOOK) {}
		virtual ~Look() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command look;
	};
}

/* methods */
inline const Command &action::Look::getCommand() {
	switch (sequence) {
	case 0:
		return look;

	default:
		return Action::noop;
	}
}

inline void action::Look::updateAction(const std::string &) {
	if (sequence == 0)
		sequence = 1;
}
#endif
