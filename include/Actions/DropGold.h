#ifndef ACTION_DROPGOLD_H
#define	ACTION_DROPGOLD_H

#include "Action.h"

namespace action {

	class DropGold : public Action {
	public:
		static const int ID;

		DropGold(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _drop("d", priority), _gold("$", PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~DropGold() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _drop;

			case 1:
				return _gold;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_DROP) != std::string::npos)
				_sequence = 1;
			else if (_sequence == 1)
				_sequence = 2;
		}

	private:
		const Command _drop;
		const Command _gold;
	};
}
#endif
