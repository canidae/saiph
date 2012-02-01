#ifndef ACTION_REMOVE_H
#define ACTION_REMOVE_H

#include "Actions/Action.h"

namespace action {
	class Remove : public Action {
	public:
		static const int ID;

		Remove(analyzer::Analyzer* analyzer, unsigned char key, int priority);
		virtual ~Remove();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _remove;
		const Command _remove_key;
	};
}
#endif
