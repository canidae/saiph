#ifndef BLIND_H
#define BLIND_H

#include "../Analyzer.h"
#include "../Request.h"

class Item;
class Saiph;

class Blind : public Analyzer {
	public:
		Blind(Saiph *saiph);

		void parseMessages(const std::string &messages);
		void analyze();
		void complete();
		bool request(const Request &request);

	private:
		Saiph *saiph;
		Request req;
		bool willful_blindness;
		unsigned char blinding_tool;
<<<<<<< HEAD:Analyzers/Blind.h
		int blind_priority, unblind_priority;
=======
		int blind_priority;
		int unblind_priority;
>>>>>>> 0a81528b6310b228c32cb497e281392a09f5b404:Analyzers/Blind.h

		void findBlindingTool();
		bool isBlindingTool(const Item &item);
};
#endif	
