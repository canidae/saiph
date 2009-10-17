#ifndef ACTION_SELECT_H
#define ACTION_SELECT_H

#include "Action.h"

namespace action {

	class Select : public Action {
	public:
		static int id;

		Select(analyzer::Analyzer *analyzer, unsigned char key) : Action(analyzer), select(std::string(1, key), PRIORITY_SELECT_ITEM) {
		}

		virtual ~Select() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command select;
	};

	inline const Command &action::Select::command() {
		switch (sequence) {
		case 0:
			return select;

		default:
			return Action::noop;
		}
	}

	inline void action::Select::update(const std::string &) {
		++sequence;
	}
}
#endif
