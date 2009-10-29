#ifndef ACTION_ENHANCE_H
#define ACTION_ENHANCE_H

#include "Action.h"

namespace action {

	class Enhance : public Action {
	public:
		static const int ID;

		Enhance(analyzer::Analyzer* analyzer) : Action(analyzer), _do_enhance("#enhance\n", PRIORITY_CONTINUE_ACTION), _enhance_a("a", PRIORITY_CONTINUE_ACTION), _close(std::string(1, CLOSE_PAGE), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Enhance() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_enhance;

			case 1:
				return _enhance_a;

			case 2:
				return _close;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			if (World::menu())
				++_sequence;
		}

	private:
		const Command _do_enhance;
		const Command _enhance_a;
		const Command _close;
	};
}
#endif
