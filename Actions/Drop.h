#ifndef ACTION_DROP_H
#define	ACTION_DROP_H

#include "Action.h"

namespace action {

	class Drop : public Action {
	public:
		static const int ID;

		Drop(analyzer::Analyzer* analyzer, const int& priority) : Action(analyzer), _drop("D", priority) {
		}

		virtual ~Drop() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _drop;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			if (_sequence == 0) {
				analyzer()->actionCompleted();
				_sequence = 1;
			}
		}

	private:
		const Command _drop;
	};
}
#endif
