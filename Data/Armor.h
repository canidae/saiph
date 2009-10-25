#ifndef DATA_ARMOR_H
#define DATA_ARMOR_H

#include <map>
#include <string>
#include "Item.h"

namespace data {

	class Armor : public Item {
	public:
		virtual ~Armor();

		static void init();
		static const std::map<const std::string, const Armor*>& armors();
		const int& slot() const;
		const int& ac() const;
		const int& mc() const;

	protected:
		Armor(const std::string& name, const int& cost, const int& weight, const int& material, const int& slot, const int& ac, const int& mc, const unsigned long long& properties);

		static void addToMap(const std::string& name, const Armor* armor);

	private:
		static std::map<const std::string, const Armor*> _armors;
		const int _slot;
		const int _ac;
		const int _mc;
	};
}
#endif
