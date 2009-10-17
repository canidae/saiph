#ifndef ACTION_ANSWER_H
#define ACTION_ANSWER_H

#include "Action.h"

namespace action {

	class Answer : public Action {
	public:
		static const int ID;

		Answer(analyzer::Analyzer *analyzer, const std::string &answer) : Action(analyzer), _answer(answer, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Answer() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command &command() {
			switch (_sequence) {
			case 0:
				return _answer;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string &) {
			if (_sequence == 0)
				_sequence = 1;
		}

	private:
		const Command _answer;
	};
}
#endif
