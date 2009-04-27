#ifndef _FIGHT_H
#define _FIGHT_H

#include "Action.h"
#include "../Globals.h"

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

	default:
		return Action::noop;
	}
}

inline void action::Fight::updateAction(const std::string &) {
	++sequence;
}
#endif
