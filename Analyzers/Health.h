#ifndef ANALYZER_HEALTH_H
#define ANALYZER_HEALTH_H

#include "Analyzer.h"

namespace analyzer {
	class Health : public Analyzer {
	public:
		Health();

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		bool resting;
	};
}
#endif
