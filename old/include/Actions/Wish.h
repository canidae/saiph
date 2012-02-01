#ifndef ACTION_WISH_H
#define ACTION_WISH_H

#include "Actions/Action.h"

namespace action {
	class Wish : public Action {
	public:
		static const int ID;

		Wish(analyzer::Analyzer* analyzer, unsigned char wand, int priority);
		virtual ~Wish();

		virtual int id();
		virtual bool canEngrave();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		bool _wished;
		const Command _wish;
		const Command _wish_wand;
		const Command _append;
		const Command _elbereth;
	};
}
#endif
