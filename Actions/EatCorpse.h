#ifndef ACTION_EAT_CORPSE_H
#define ACTION_EAT_CORPSE_H

#include "Action.h"

namespace action {
	class EatCorpse : public Action {
	public:
		static int id;

		EatCorpse(analyzer::Analyzer *analyzer, std::string corpse, int priority) : Action(analyzer), eat(std::string(1, 'e'), priority), corpse(corpse), answer_no(std::string(1, NO), PRIORITY_CONTINUE_ACTION), answer_yes(std::string(1, YES), PRIORITY_CONTINUE_ACTION), look(":", PRIORITY_CONTINUE_ACTION), escape(std::string(1, (char) 27), PRIORITY_CONTINUE_ACTION) {}
		virtual ~EatCorpse() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command eat;
		const std::string corpse;
		const Command answer_no;
		const Command answer_yes;
		const Command look;
		const Command escape;
	};

	inline const Command &EatCorpse::getCommand() {
		switch (sequence) {

		case 0:
			return eat;

		case 1:
			return answer_no;

		case 2:
			return answer_yes;

		case 3:
			return look;

		case 4:
			return escape;

		default:
			return Action::noop;
		}
	}

	inline void EatCorpse::updateAction(const std::string &messages) {
		if (messages.find(MESSAGE_WHAT_TO_EAT) != std::string::npos) {
			/* we got "what do you want to eat?", we shouldn't get this
			 * as we want to eat a corpse on the ground. escape */
			sequence = 4;
		} else if (messages.find(MESSAGE_EAT_IT_2) != std::string::npos || messages.find(MESSAGE_EAT_ONE_2) != std::string::npos) {
			/* asking if we should eat corpse on ground, check that it's the right corpse */
			if (messages.find(corpse) != std::string::npos && (messages.find(MESSAGE_EAT_IT_1) != std::string::npos || messages.find(MESSAGE_EAT_ONE_1) != std::string::npos))
				sequence = 2;
			else
				sequence = 1;
		} else if (sequence == 1 || sequence == 2 || sequence == 4) {
			/* either ate a corpse or cancelled the eat request, make us look at ground */
			sequence = 3;
		} else if (sequence == 3) {
			/* looked at ground, action is complete */
			sequence = 5;
		}
	}
}
#endif
