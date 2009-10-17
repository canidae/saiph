#ifndef ACTION_SELECT_MULTIPLE_H
#define ACTION_SELECT_MULTIPLE_H

#include <vector>
#include "Action.h"

namespace action {

	class SelectMultiple : public Action {
	public:
		static const int ID;

		SelectMultiple(analyzer::Analyzer *analyzer, const std::vector<std::string> &keys) : Action(analyzer), _keys(keys) {
		}

		virtual ~SelectMultiple() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command &command() {
			if (_sequence < (int) _keys.size())
				_tmp = Command(_keys[_sequence], PRIORITY_SELECT_ITEM);
			else if (_sequence == (int) _keys.size())
				_tmp = Command(std::string(1, CLOSE_PAGE), PRIORITY_CLOSE_SELECT_PAGE);
			else
				_tmp = Action::NOOP;
			return _tmp;
		}

		virtual void update(const std::string &) {
			++_sequence;
		}

	private:
		const std::vector<std::string> _keys;
		Command _tmp;
	};
}
#endif
