#ifndef ACTION_PUT_ON_H
#define ACTION_PUT_ON_H

#include "Action.h"
#include "../Inventory.h"

namespace action {

	class PutOn : public Action {
	public:
		static int id;

		PutOn(analyzer::Analyzer *analyzer, unsigned char key, int priority) : Action(analyzer), put_on("P", priority), put_on_key(std::string(1, key), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~PutOn() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command put_on;
		const Command put_on_key;
	};

	inline const Command &action::PutOn::command() {
		switch (sequence) {
		case 0:
			return put_on;

		case 1:
			return put_on_key;

		default:
			return Action::NOOP;
		}
	}

	inline void action::PutOn::update(const std::string &messages) {
		if (World::question && messages.find(MESSAGE_WHAT_TO_PUT_ON) != std::string::npos) {
			sequence = 1;
		} else if (sequence == 1) {
			/* also mark the inventory dirty when we do this */
			Inventory::updated = false;
			sequence = 2;
		}
	}
}
#endif
