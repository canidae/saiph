#ifndef ACTION_REMOVE_H
#define ACTION_REMOVE_H

#include "Action.h"
#include "../Inventory.h"

namespace action {
	class Remove : public Action {
	public:
		static int id;

		Remove(analyzer::Analyzer *analyzer, unsigned char key, int priority) : Action(analyzer), remove(REMOVE, priority), remove_key(std::string(1, key), PRIORITY_CONTINUE_ACTION) {}
		virtual ~Remove() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command remove;
		const Command remove_key;
	};
}

/* methods */
inline const Command &action::Remove::getCommand() {
	switch (sequence) {
	case 0: 
		return remove;

	case 1: 
		return remove_key;

	default:
		return Action::noop;
	}
}

inline void action::Remove::updateAction(const std::string &messages) {
	if (World::question && messages.find(MESSAGE_WHAT_TO_REMOVE) != std::string::npos) {
		sequence = 1;
	} else if (sequence == 1) {
		/* also mark the inventory dirty when we do this */
		Inventory::updated = false;
		sequence = 2;
	}
}
#endif
