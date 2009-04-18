#ifndef _ACTION_H
#define _ACTION_H

#include "../Command.h"

namespace action {
	class Action {
	public:
		static const Command noop;

		Action() : sequence(0) {}
		virtual ~Action() {}

		static int createID() {return ++counter;}
		virtual int getID() = 0;
		virtual const Command &getCommand() = 0;
		virtual void updateAction() = 0;

	protected:
		int sequence;

	private:
		static int counter;
	};
}
#endif
