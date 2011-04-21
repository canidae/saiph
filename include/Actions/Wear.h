#ifndef ACTION_WEAR_H
#define	ACTION_WEAR_H

#include "Actions/Action.h"

namespace action {
	class Wear : public Action {
	public:
		static const int ID;

		Wear(analyzer::Analyzer* analyzer, unsigned char key, int priority);
		virtual ~Wear();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _wear;
		const Command _wear_key;
	};
}
#endif
