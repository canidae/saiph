#ifndef ACTION_SELECT_H
#define ACTION_SELECT_H

#include "Action.h"

namespace action {

	class Select : public Action {
	public:
		static const int ID;

		Select(analyzer::Analyzer* analyzer, unsigned char key) : Action(analyzer), _select(key, PRIORITY_SELECT_ITEM) {
		}

		virtual ~Select() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _select;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			if (_sequence == 0)
				_sequence = 1;
		}

	private:
		const Command _select;
	};
}
#endif
