#ifndef BLIND_H
#define BLIND_H

#include "../Saiph.h"
#include "../Request.h"
#include "../Analyzer.h"

class Blind : public Analyzer {
public:
	Blind(Saiph* saiph);
	void analyze();
	bool request(const Request &request);
	bool isBlindingTool(const unsigned char& letter);
	bool isBlindingTool(const Item& item);
private:
	Saiph* saiph;
	bool willful_blindness;
	unsigned char blinding_tool;
};

#endif	

