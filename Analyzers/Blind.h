#ifndef BLIND_H
#define BLIND_H

#include "../Analyzer.h"
#include "../Request.h"

class Item;
class Saiph;

class Blind : public Analyzer {
	public:
		Blind(Saiph *saiph);

		void analyze(const std::string &messages);
		void complete();
		bool request(const Request &request);

	private:
		Saiph *saiph;
		Request req;
		bool willful_blindness;
		unsigned char blinding_tool;
		int blind_priority;
		int unblind_priority;

		void findBlindingTool();
		bool isBlindingTool(const Item &item);
};
#endif	
