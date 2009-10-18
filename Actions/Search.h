#ifndef ACTION_SEARCH_H
#define ACTION_SEARCH_H

#include "Action.h"

namespace action {

	class Search : public Action {
	public:
		static const int ID;

		Search(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _search("s", priority) {
		}

		virtual ~Search() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _search;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			if (_sequence == 0) {
				/* increase search counter on level */
				World::levels[Saiph::position().level()].increaseAdjacentSearchCount(Saiph::position());
				_sequence = 1;
			}
		}

	private:
		const Command _search;
	};
}
#endif
