#ifndef ACTION_PRAY_H
#define ACTION_PRAY_H

#include "Actions/Action.h"

#define PRAY_PRAYER_TIMEOUT 1000
#define PRAY_MAJOR_TROUBLE 800

namespace action {
	class Pray : public Action {
	public:
		static const int ID;

		Pray(analyzer::Analyzer* analyzer, int priority);
		virtual ~Pray();

		static bool isSafeToPray();
		static bool isSafeToPrayMajorTrouble();
		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _pray;
	};
}
#endif
