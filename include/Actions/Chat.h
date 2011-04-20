#ifndef ACTION_CHAT_H
#define ACTION_CHAT_H

#include "Actions/Action.h"

namespace action {

	class Chat : public Action {
	public:
		static const int ID;

		Chat(analyzer::Analyzer* analyzer, bool shop) : Action(analyzer), _do_chat("#chat\n", PRIORITY_CONTINUE_ACTION), _shop(shop) {
		}

		virtual ~Chat() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_chat;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			if (_sequence == 0)
				_sequence = 1;
			else if (_sequence == 1)
				_sequence = 2;
		}

	private:
		const Command _do_chat;
        const bool _shop;
	};
}
#endif
