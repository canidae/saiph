#ifndef ACTION_SEARCH_H
#define ACTION_SEARCH_H

#include "Action.h"

namespace action {

	class Search : public Action {
	public:
		static int id;

		Search(analyzer::Analyzer *analyzer, int priority) : Action(analyzer), search("s", priority) {
		}

		virtual ~Search() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command search;
	};

	inline const Command &action::Search::command() {
		switch (sequence) {
		case 0:
			return search;

		default:
			return Action::noop;
		}
	}

	inline void action::Search::update(const std::string &) {
		if (sequence == 0) {
			/* increase search counter on level */
			World::levels[Saiph::position.level()].increaseAdjacentSearchCount(Saiph::position);
			sequence = 1;
		}
	}
}
#endif
