#ifndef _REMOVE_H
#define _REMOVE_H

#include "Action.h"

namespace action {
	class Remove : public Action {
	public:
		Remove(unsigned char key, int priority);
		virtual ~Remove();

		virtual const Command &getCommand();
		virtual void updateAction();

	private:
		const Command remove;
		const Command remove_key;
	};
}
#endif
