#ifndef WAND_H
#define WAND_H

#include <string>
#include "../Analyzer.h"
#include "../Request.h"

class Saiph;

class Wand : public Analyzer {
	public:
		Wand(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		Request req;
};
#endif
