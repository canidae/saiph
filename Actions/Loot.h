#ifndef ACTION_LOOT_H
#define ACTION_LOOT_H

#include "Action.h"

namespace action {

	class Loot : public Action {
	public:
		static int id;

		Loot(analyzer::Analyzer *analyzer, int priority) : Action(analyzer), loot(",", priority) {
		}

		virtual ~Loot() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command loot;
	};

	inline const Command &action::Loot::command() {
		switch (sequence) {
		case 0:
			return loot;

		default:
			return Action::noop;
		}
	}

	inline void action::Loot::update(const std::string &) {
		if (sequence == 0)
			sequence = 1;
	}
}
#endif
