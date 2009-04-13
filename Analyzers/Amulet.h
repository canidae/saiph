#ifndef AMULET_H
#define AMULET_H

#include <string>
#include <vector>
#include "Analyzer.h"
#include "../Request.h"

class Saiph;

namespace analyzer {
	class Amulet : public Analyzer {
	public:
		Amulet(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		Request req;

		void wearAmulet();
	};
}
#endif
