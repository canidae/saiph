#ifndef ACTION_PUT_ON_H
#define ACTION_PUT_ON_H

#include "Actions/Action.h"

namespace action {
	class PutOn : public Action {
	public:
		static const int ID;

		PutOn(analyzer::Analyzer* analyzer, unsigned char key, int priority);
		virtual ~PutOn();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _put_on;
		const Command _put_on_key;
	};
}
#endif
