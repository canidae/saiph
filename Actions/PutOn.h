#ifndef _PUT_ON_H
#define _PUT_ON_H

#include "Action.h"

namespace action {
	class PutOn : public Action {
	public:
		static const int id;

		PutOn(unsigned char key, int priority);
		virtual ~PutOn() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command put_on;
		const Command put_on_key;
	};
}
#endif
