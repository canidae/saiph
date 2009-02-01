#ifndef THRONE_H
#define	THRONE_H

#include "../Analyzer.h"
#include "../Saiph.h"

class Throne : public Analyzer {
	public:
		Throne(Saiph* saiph);
		void parseMessages(const std::string& messages);
		void analyze();

	private:
		Saiph* saiph;
};
#endif
