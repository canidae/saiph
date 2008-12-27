#ifndef RING_H
#define RING_H

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"

/* struct for wearing ring */
struct WearRing {
	int beatitude;
	std::string name;
};

class Saiph;

class Ring : public Analyzer {
	public:
		Ring(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		std::string command2;
		std::vector<WearRing> rings;
		bool wear_ring;
		Request req;

		void wearRing();
};
#endif
