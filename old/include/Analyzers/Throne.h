#ifndef ANALYZER_THRONE_H
#define ANALYZER_THRONE_H

#include "Saiph.h"
#include "Analyzers/Analyzer.h"

namespace analyzer {
	class Throne : public Analyzer {
	public:
		Throne(Saiph* saiph);

		void analyze();

	private:
		Saiph* saiph;
	};
}
#endif
