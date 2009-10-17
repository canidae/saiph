#ifndef ACTION_PRAY_H
#define ACTION_PRAY_H

#include "Action.h"
#include "../Saiph.h"
#include "../World.h"

#define PRAY_PRAYER_TIMEOUT 1000

namespace action {

	class Pray : public Action {
	public:
		static int id;

		Pray(analyzer::Analyzer *analyzer, int priority) : Action(analyzer), pray("#pray\n", priority) {
		}

		virtual ~Pray() {
		}

		static bool isSafeToPray() {
			return World::turn - PRAY_PRAYER_TIMEOUT > Saiph::last_pray_turn;
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command pray;
	};

	inline const Command &action::Pray::command() {
		if (sequence == 0)
			return pray;
		return Action::NOOP;
	}

	inline void action::Pray::update(const std::string &messages) {
		if (messages.find(MESSAGE_YOU_FINISH_YOUR_PRAYER) != std::string::npos) {
			Saiph::last_pray_turn = World::turn;
			sequence = 1;
		}
	}
}
#endif
