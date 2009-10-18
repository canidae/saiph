#ifndef ACTION_REST_H
#define ACTION_REST_H

#include "Action.h"

namespace action {

	class Rest : public Action {
	public:
		static const int ID;

		Rest(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _rest("16s", priority) {
		}

		virtual ~Rest() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _rest;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			++_sequence;
		}

	private:
		const Command _rest;
	};
}
#endif
