#ifndef ACTION_FIGHT_H
#define ACTION_FIGHT_H

#include "Action.h"
#include "../Globals.h"
#include "../World.h"

#define MESSAGE_REALLY_ATTACK "Really attack"

namespace action {
	class Fight : public Action {
	public:
		static int id;

		Fight(analyzer::Analyzer *analyzer, unsigned char direction, int priority, bool attack_friendly = true) : Action(analyzer), fight(std::string(1, 'F').append(std::string(1, direction)), priority), attack_friendly(attack_friendly ? YES : NO, PRIORITY_CONTINUE_ACTION) {}
		virtual ~Fight() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command fight;
		const Command attack_friendly;
	};
}

/* methods */
inline const Command &action::Fight::getCommand() {
	switch (sequence) {
	case 0: 
		return fight;

	case 1:
		return attack_friendly;

	default:
		return Action::noop;
	}
}

inline void action::Fight::updateAction(const std::string &messages) {
	if (sequence == 0) {
		if (World::question && messages.find(MESSAGE_REALLY_ATTACK) != std::string::npos)
			sequence = 1;
		else
			sequence = 2;
	}
}
#endif
