#ifndef THRONE_H
#define	THRONE_H

#include "../Analyzer.h"
#include "../Saiph.h"

class Throne : public Analyzer {
	public:
		Throne(Saiph *saiph);

		void analyze();

	private:
		Saiph *saiph;
};
#endif
