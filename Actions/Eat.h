#ifndef ACTION_EAT_H
#define ACTION_EAT_H

#include "Action.h"

namespace action {
	class Eat : public Action {
	public:
		static int id;

		Eat(analyzer::Analyzer *analyzer, unsigned char key, int priority) : Action(analyzer), eat(std::string(1, 'e'), priority), item(std::string(1, key), PRIORITY_CONTINUE_ACTION), answer_no(std::string(1, NO), PRIORITY_CONTINUE_ACTION) {}
		virtual ~Eat() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command eat;
		const Command item;
		const Command answer_no;
	};

	inline const Command &Eat::getCommand() {
		switch (sequence) {

		case 0:
			return eat;

		case 1:
			return item;

		case 2:
			return append;

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
