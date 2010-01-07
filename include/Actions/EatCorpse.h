#ifndef ACTION_EAT_CORPSE_H
#define ACTION_EAT_CORPSE_H

#include "World.h"
#include "Actions/Action.h"

namespace action {

	class EatCorpse : public Action {
	public:
		static const int ID;

		EatCorpse(analyzer::Analyzer* analyzer, const std::string& corpse, int priority) : Action(analyzer), _eat("e", priority), _corpse(corpse), _answer_no(NO, PRIORITY_CONTINUE_ACTION), _answer_yes(YES, PRIORITY_CONTINUE_ACTION), _escape((unsigned char) 27, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~EatCorpse() {
		}

		virtual int id() {
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
				return _escape;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_EAT) != std::string::npos) {
				/* we got "what do you want to eat?", we shouldn't get this
				 * as we want to eat a corpse on the ground. escape */
				_sequence = 3;
			} else if (messages.find(MESSAGE_EAT_IT_2) != std::string::npos || messages.find(MESSAGE_EAT_ONE_2) != std::string::npos) {
				/* asking if we should eat corpse on ground, check that it's the right corpse */
				if (messages.find(_corpse) != std::string::npos && (messages.find(MESSAGE_EAT_IT_1) != std::string::npos || messages.find(MESSAGE_EAT_ONE_1) != std::string::npos))
					_sequence = 2;
				else
					_sequence = 1;
			} else if (messages.find(MESSAGE_STOP_EATING) != std::string::npos) {
				/* we're about to choke, abort eating */
				_sequence = 2;
			} else {
				/* either ate a corpse or cancelled the eat request, make stash dirty and we're done */
				World::level().setDirtyStash(Saiph::position());
				_sequence = 4;
			}
		}

	private:
		const Command _eat;
		const std::string _corpse;
		const Command _answer_no;
		const Command _answer_yes;
		const Command _escape;
	};
}
#endif
