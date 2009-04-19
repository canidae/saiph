#ifndef _REMOVE_H
#define _REMOVE_H

#include "Action.h"

namespace action {
	class Remove : public Action {
	public:
		static const int id;

		Remove(unsigned char key, int priority);
		virtual ~Remove() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command remove;
		const Command remove_key;
	};
}
#endif
