#ifndef ACTION_FIGHT_H
#define ACTION_FIGHT_H

#include "Actions/Action.h"

namespace action {

	class Fight : public Action {
	public:
		static const int ID;

		Fight(analyzer::Analyzer* analyzer, unsigned char direction, int priority, bool attack_friendly = true) : Action(analyzer), _fight(std::string("F").append(std::string(1, direction)), priority), _attack_friendly(attack_friendly ? YES : NO, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Fight() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _fight;

			case 1:
				return _attack_friendly;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (_sequence == 0) {
				if (messages.find(MESSAGE_REALLY_ATTACK) != std::string::npos)
					_sequence = 1;
				else
					_sequence = 2;
			} else if (_sequence == 1) {
				_sequence = 2;
			}
		}

	private:
		const Command _fight;
		const Command _attack_friendly;
	};
}
#endif
