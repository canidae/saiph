#ifndef AMULET_H
#define AMULET_H

#include <string>
#include <vector>
#include "../Analyzer.h"
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
