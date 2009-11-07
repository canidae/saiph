#ifndef ACTION_EAT_CORPSE_H
#define ACTION_EAT_CORPSE_H

#include "Action.h"
#include "../World.h"

namespace action {

	class EatCorpse : public Action {
	public:
		static const int ID;

		EatCorpse(analyzer::Analyzer* analyzer, const std::string& corpse, const int& priority) : Action(analyzer), _eat("e", priority), _corpse(corpse), _answer_no(NO, PRIORITY_CONTINUE_ACTION), _answer_yes(YES, PRIORITY_CONTINUE_ACTION), _look(":", PRIORITY_CONTINUE_ACTION), _escape((unsigned char) 27, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~EatCorpse() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {

			case 0:
				return _eat;

			case 1:
				return _answer_no;

			case 2:
				return _answer_yes;

			case 3:
				return _look;

			case 4:
				return _escape;

			default:
				return Action::NOOP;
			}
		};

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_EAT) != std::string::npos) {
				/* we got "what do you want to eat?", we shouldn't get this
				 * as we want to eat a corpse on the ground. escape */
				_sequence = 4;
			} else if (messages.find(MESSAGE_EAT_IT_2) != std::string::npos || messages.find(MESSAGE_EAT_ONE_2) != std::string::npos) {
				/* asking if we should eat corpse on ground, check that it's the right corpse */
				if (messages.find(_corpse) != std::string::npos && (messages.find(MESSAGE_EAT_IT_1) != std::string::npos || messages.find(MESSAGE_EAT_ONE_1) != std::string::npos))
					_sequence = 2;
				else
					_sequence = 1;
			} else if (messages.find(MESSAGE_STOP_EATING) != std::string::npos) {
				/* we're about to choke, abort eating */
				_sequence = 2;
			} else if (_sequence == 1 || _sequence == 2 || _sequence == 4) {
				/* either ate a corpse or cancelled the eat request, make stash dirty and look at ground */
				std::map<Point, Stash>::iterator stash = World::level().stashes().find(Saiph::position());
				if (stash != World::level().stashes().end())
					stash->second.items().clear();
				_sequence = 3;
			} else if (_sequence == 3) {
				/* looked at ground, action is complete */
				_sequence = 5;
			}
		}

	private:
		const Command _eat;
		const std::string _corpse;
		const Command _answer_no;
		const Command _answer_yes;
		const Command _look;
		const Command _escape;
	};
}
#endif
