#ifndef ANALYZER_BLIND_H
#define ANALYZER_BLIND_H

#include "Request.h"
#include "Analyzers/Analyzer.h"

class Item;
class Saiph;

namespace analyzer {
	class Blind : public Analyzer {
	public:
		Blind(Saiph* saiph);

		void parseMessages(const std::string& messages);
		void analyze();
		void complete();
		bool request(const Request& request);

	private:
		Saiph* saiph;
		Request req;
		bool willful_blindness;
		unsigned char blinding_tool;
		int blind_priority;
		int unblind_priority;

		void findBlindingTool();
		bool isBlindingTool(const Item& item);
	};
}
#endif
