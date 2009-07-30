#ifndef ACTION_SELECT_MULTIPLE_H
#define ACTION_SELECT_MULTIPLE_H

#include <vector>
#include "Action.h"

namespace action {
	class SelectMultiple : public Action {
	public:
		static int id;

		SelectMultiple(analyzer::Analyzer *analyzer, std::vector<std::string> keys) : Action(analyzer), keys(keys) {}
		virtual ~SelectMultiple() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const std::vector<std::string> keys;
		Command tmp;
	};

	inline const Command &action::SelectMultiple::getCommand() {
		if (sequence < (int) keys.size())
			tmp = Command(keys[sequence], PRIORITY_SELECT_ITEM);
		else if (sequence == (int) keys.size())
			tmp = Command(std::string(1, CLOSE_PAGE), PRIORITY_CLOSE_SELECT_PAGE);
		else
			tmp = Action::noop;
		return tmp;
	}

	inline void action::SelectMultiple::updateAction(const std::string &) {
		++sequence;
	}
}
#endif
