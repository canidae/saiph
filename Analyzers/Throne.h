#ifndef THRONE_H
#define	THRONE_H

#include "../Analyzer.h"
#include "../Saiph.h"

class Throne : public Analyzer {
	public:
		Throne(Saiph *saiph);

		void analyze(const std::string &messages);

	private:
		Saiph *saiph;
};
#endif
