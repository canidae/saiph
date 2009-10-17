#ifndef ACTION_LOOT_H
#define ACTION_LOOT_H

#include "Action.h"

namespace action {

	class Loot : public Action {
	public:
		static const int ID;

		Loot(analyzer::Analyzer *analyzer, int priority) : Action(analyzer), _loot(",", priority) {
		}

		virtual ~Loot() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command &command() {
			switch (_sequence) {
			case 0:
				return _loot;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string &) {
			if (_sequence == 0)
				_sequence = 1;
		}

	private:
		const Command _loot;
	};
}
#endif
