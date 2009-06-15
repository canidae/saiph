#ifndef ANALYZER_THRONE_H
#define ANALYZER_THRONE_H

#include "Analyzer.h"
#include "../Saiph.h"

namespace analyzer {
	class Throne : public Analyzer {
	public:
		Throne(Saiph *saiph);

		void analyze();

	private:
		Saiph *saiph;
	};
}
#endif
