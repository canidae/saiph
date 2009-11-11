#ifndef DATA_SHIRT_H
#define DATA_SHIRT_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {

	class Shirt : public Armor {
	public:
		virtual ~Shirt();

		static void init();
		static const std::map<const std::string, const Shirt*>& shirts();

	protected:
		Shirt(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		static void addToMap(const std::string& name, const Shirt* shirt);

	private:
		static std::map<const std::string, const Shirt*> _shirts;

		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
