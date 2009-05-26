#ifndef ENGRAVE_H
#define	ENGRAVE_H

#include "Action.h"

#define MESSAGE_ENGRAVE_ADD "  Do you want to add to the current engraving? "
#define MESSAGE_ENGRAVE_DUST "  What do you want to write in the dust here?  "
#define MESSAGE_ENGRAVE_DUST_ADD "  What do you want to add to the writing in the dust here?  "
#define MESSAGE_ENGRAVE_FROST "  What do you want to write in the frost here?  "
#define MESSAGE_ENGRAVE_FROST_ADD "  What do you want to add to the writing in the frost here?  "
#define MESSAGE_ENGRAVE_WITH "  What do you want to write with? "

namespace action {
	class Engrave : public Action {
	public:
		static int id;

		Engrave(analyzer::Analyzer *analyzer, std::string engrave_msg, unsigned char engrave_with,
				int priority, bool should_append = true) :
				Action(analyzer), engrave(std::string(1, 'E'), priority), message(engrave_msg, PRIORITY_CONTINUE_ACTION),
				item(std::string(1,engrave_with), PRIORITY_CONTINUE_ACTION),
				append(std::string(1, should_append ? 'y' : 'n'), PRIORITY_CONTINUE_ACTION) {}
		virtual ~Engrave() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);
	private:
		const Command engrave;
		const Command message;
		const Command item;
		const Command append;
	};

	inline const Command &Engrave::getCommand() {
		// what do you want to write with?
		// do you want to add? (maybe)
		// what do you want to add? or what do you want to write?
		switch (sequence) {
		case 0:
			return engrave;
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

	inline void Engrave::updateAction(const std::string &messages) {
		if (messages.find(MESSAGE_ENGRAVE_WITH) != std::string::npos)
			sequence = 1;
		else if (messages.find(MESSAGE_ENGRAVE_ADD) != std::string::npos)
			sequence = 2;
		else if (sequence == 3 || sequence == -1)
			sequence = -1;
		else
			sequence = 3;
	}
}
#endif
