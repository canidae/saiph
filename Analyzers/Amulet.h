#ifndef AMULET_H
#define AMULET_H
/* priorities */
#define AMULET_PICKUP_PRIORITY 425
#define AMULET_WEAR_PRIORITY 225

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"

/* struct for wearing amulet */
struct WearAmulet {
	int beatitude;
	std::string name;
};

class Saiph;

class Amulet : public Analyzer {
	public:
		Amulet(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		std::string command2;
		std::vector<WearAmulet> amulets;
		bool wear_amulet;
		Request req;

		void wearAmulet();
};
#endif
