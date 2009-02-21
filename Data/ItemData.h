#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <string>

/* TODO: material? */

class ItemData {
public:
	ItemData(const std::string& name, int base_cost, int weight, bool magic);
	std::string getName() const {return name;}
	int getBaseCost() const {return base_cost;}
	int getWeight() const {return weight;}
	bool isMagic() const {return magic;}
	/* returns the item's symbol, which is unique enough */
	virtual char getClass()=0 const;
	virtual ~ItemData() {}
private:
	const std::string name;
	const int base_cost;
	const int weight;
	const bool magic;
};

#endif ITEMDATA_H