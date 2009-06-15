#ifndef DATA_SABER_H
#define DATA_SABER_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Saber : public Weapon {
	public:
		static std::map<std::string, Saber *> sabers;

		Saber(const std::string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2);
		virtual ~Saber() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Saber *saber);

	private:
		static void create(const std::string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
