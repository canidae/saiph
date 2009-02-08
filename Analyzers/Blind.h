#ifndef BLIND_H
#define BLIND_H

#include "../Analyzer.h"
#include "../Request.h"
#include "../Saiph.h"

class Blind : public Analyzer {
	public:
		Blind(Saiph *saiph);

		void analyze();
		bool request(const Request &request);
		void findBlindingTool();
		bool isBlindingTool(const Item &item);

	private:
		Saiph *saiph;
		Request req;
		bool willful_blindness;
		unsigned char blinding_tool;
};
#endif	
