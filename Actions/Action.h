#ifndef _ACTION_H
#define _ACTION_H

#include "../Command.h"
#include "../Globals.h"

namespace Action {
	class Action {
	public:
		Action() {}
		virtual ~Action() {}

		virtual const Command &execute() = 0;
	};
}
#endif
