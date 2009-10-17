#ifndef ACTION_REST_H
#define ACTION_REST_H

#include "Action.h"

namespace action {

	class Rest : public Action {
	public:
		static int id;

		Rest(analyzer::Analyzer *analyzer, int priority) : Action(analyzer), rest("16s", priority) {
		}

		virtual ~Rest() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command rest;
	};

	inline const Command &action::Rest::getCommand() {
		switch (sequence) {
		case 0:
			return rest;

		default:
			return Action::noop;
		}
	}

	inline void action::Rest::updateAction(const std::string &) {
		++sequence;
	}
}
#endif
