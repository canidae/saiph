#ifndef DATA_AXE_H
#define DATA_AXE_H

#include <map>
#include <string>
#include "Data/Weapon.h"

namespace data {

	class Axe : public Weapon {
	public:
		virtual ~Axe();

		static void init();
		static const std::map<const std::string, const Axe*>& axes();

	protected:
		Axe(const std::string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Axe* axe);

	private:
		static std::map<const std::string, const Axe*> _axes;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
