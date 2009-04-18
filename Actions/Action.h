#ifndef _ACTION_H
#define _ACTION_H

#include "../Command.h"

namespace action {
	class Action {
	public:
		static const Command noop;

		Action() : sequence(0) {}
		virtual ~Action() {}

		virtual const Command &getCommand() {return noop;}
		virtual void updateAction() {}

	protected:
		int sequence;
	};
}
#endif
