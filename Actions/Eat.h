#ifndef ACTION_EAT_H
#define ACTION_EAT_H

#include "Action.h"

namespace action {
	class Eat : public Action {
	public:
		static int id;

		Eat(analyzer::Analyzer *analyzer, std::string eat_msg, unsigned char eat_with, int priority, bool should_append = true) : Action(analyzer), eat(std::string(1, 'E'), priority), message(eat_msg, PRIORITY_CONTINUE_ACTION), item(std::string(1,eat_with), PRIORITY_CONTINUE_ACTION), append(std::string(1, should_append ? 'y' : 'n'), PRIORITY_CONTINUE_ACTION) {}
		virtual ~Eat() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command eat;
		const Command message;
		const Command item;
		const Command append;
	};

	inline const Command &Eat::getCommand() {
		switch (sequence) {

		case 0:
			return eat;

		case 1:
			return item;

		case 2:
			return append;

		case 3:
			return message;

		default:
			return Action::noop;
		}
	}

	inline void Eat::updateAction(const std::string &messages) {
		if (messages.find(MESSAGE_EAT_WITH) != std::string::npos)
			sequence = 1;
		else if (messages.find(MESSAGE_EAT_ADD) != std::string::npos)
			sequence = 2;
		else if (sequence == 3 || sequence == -1)
			sequence = -1;
		else
			sequence = 3;
	}
}
#endif
