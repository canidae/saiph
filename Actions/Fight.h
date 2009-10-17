#ifndef ACTION_FIGHT_H
#define ACTION_FIGHT_H

#include "Action.h"

namespace action {

	class Fight : public Action {
	public:
		static int id;

		Fight(analyzer::Analyzer *analyzer, unsigned char direction, int priority, bool attack_friendly = true) : Action(analyzer), fight(std::string(1, 'F').append(std::string(1, direction)), priority), attack_friendly(std::string(1, attack_friendly ? YES : NO), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Fight() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command fight;
		const Command attack_friendly;
	};

	inline const Command &action::Fight::command() {
		switch (sequence) {
		case 0:
			return fight;

		case 1:
			return attack_friendly;

		default:
			return Action::noop;
		}
	}

	inline void action::Fight::update(const std::string &messages) {
		if (sequence == 0) {
			if (messages.find(MESSAGE_REALLY_ATTACK) != std::string::npos)
				sequence = 1;
			else
				sequence = 2;
		}
	}
}
#endif
