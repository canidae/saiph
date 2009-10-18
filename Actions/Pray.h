#ifndef ACTION_PRAY_H
#define ACTION_PRAY_H

#include "Action.h"
#include "../Saiph.h"
#include "../World.h"

#define PRAY_PRAYER_TIMEOUT 1000

namespace action {

	class Pray : public Action {
	public:
		static const int ID;

		Pray(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _pray("#pray\n", priority) {
		}

		virtual ~Pray() {
		}

		static bool isSafeToPray() {
			return World::turn - PRAY_PRAYER_TIMEOUT > Saiph::last_pray_turn;
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			if (_sequence == 0)
				return _pray;
			return Action::NOOP;
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_YOU_FINISH_YOUR_PRAYER) != std::string::npos) {
				Saiph::last_pray_turn = World::turn;
				_sequence = 1;
			}
		}

	private:
		const Command _pray;
	};
}
#endif
