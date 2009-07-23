#ifndef ACTION_THROW_H
#define ACTION_THROW_H

#include "Action.h"
#include "../Inventory.h"
#include "../World.h"

namespace action {
	class Throw : public Action {
	public:
		static int id;

		Throw(analyzer::Analyzer *analyzer, unsigned char key, unsigned char direction, int priority) : Action(analyzer), do_throw("t", priority), throw_item(std::string(1, key), PRIORITY_CONTINUE_ACTION), throw_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION) {}
		virtual ~Throw() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command do_throw;
		const Command throw_item;
		const Command throw_direction;
	};

	inline const Command &action::Throw::getCommand() {
		switch (sequence) {
		case 0:
			return do_throw;

		case 1:
			return throw_item;

		case 2:
			return throw_direction;

		default:
			return Action::noop;
		}
	}

	inline void action::Throw::updateAction(const std::string &messages) {
		if (World::question && messages.find(MESSAGE_WHAT_TO_THROW) != std::string::npos) {
			sequence = 1;
		} else if (World::question && messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
			sequence = 2;
		} else if (sequence == 2) {
			/* mark inventory dirty */
			Inventory::updated = false;
			sequence = 3;
		}
	}
}
#endif
