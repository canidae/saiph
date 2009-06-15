#ifndef ACTION_LOOT_H
#define ACTION_LOOT_H

#include "Action.h"

namespace action {
	class Loot : public Action {
	public:
		static int id;

		Loot(analyzer::Analyzer *analyzer, int priority) : Action(analyzer), loot(",", priority) {}
		virtual ~Loot() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command loot;
	};
}

/* methods */
inline const Command &action::Loot::getCommand() {
	switch (sequence) {
	case 0: 
		return loot;

	default:
		return Action::noop;
	}
}

inline void action::Loot::updateAction(const std::string &) {
	if (sequence == 0)
		sequence = 1;
}
#endif
