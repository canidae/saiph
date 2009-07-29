#ifndef ACTION_EAT_CORPSE_H
#define ACTION_EAT_CORPSE_H

#include <string>
#include "Action.h"

namespace action {
	class EatCorpse : public Action {
	public:
		static int id;

		EatCorpse(analyzer::Analyzer *analyzer, std::string corpse, int priority) : Action(analyzer), eat(std::string(1, 'e'), priority), corpse(corpse), answer_no(std::string(1, NO), PRIORITY_CONTINUE_ACTION), answer_yes(std::string(1, YES), PRIORITY_CONTINUE_ACTION) {}
		virtual ~EatCorpse() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command eat;
		const std::string corpse;
		const Command answer_no;
		const Command answer_yes;
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
			return answer_yes;

		default:
			return Action::noop;
		}
	}

	inline void EatCorpse::updateAction(const std::string &messages) {
		if (messages.find(MESSAGE_WHAT_TO_EAT_CORPSE) != std::string::npos) {
			sequence = 3;
		} else if (messages.find(MESSAGE_EAT_IT_2) != std::string::npos || messages.find(MESSAGE_EAT_ONE_2) != std::string::npos) {
			/* TODO */
			sequence = 2;
		} else if (sequence == 1 || sequence == 2) {
			sequence = 4;
		}
	}
}
#endif
