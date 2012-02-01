#ifndef DATA_HELMET_H
#define DATA_HELMET_H

#include <map>
#include <string>
#include "Data/Armor.h"

namespace data {

	class Helmet : public Armor {
	public:
		virtual ~Helmet();

		static void init();
		static const std::map<const std::string, const Helmet*>& helmets();

	protected:
		Helmet(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		static void addToMap(const std::string& name, const Helmet* helmet);

	private:
		static std::map<const std::string, const Helmet*> _helmets;

		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
