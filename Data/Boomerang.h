#ifndef DATA_BOOMERANG_H
#define DATA_BOOMERANG_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Boomerang : public Weapon {
	public:
		virtual ~Boomerang();

		static void init();
		static const std::map<const std::string, const Boomerang*>& boomerangs();

	protected:
		Boomerang(const std::string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Boomerang* boomerang);

	private:
		static std::map<const std::string, const Boomerang*> _boomerangs;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
