#ifndef DATA_MORNING_STAR_H
#define DATA_MORNING_STAR_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class MorningStar : public Weapon {
	public:
		static std::map<std::string, MorningStar*> morning_stars;

		MorningStar(const std::string& name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		virtual ~MorningStar() {
		}

		static void init();

	protected:
		static void addToMap(const std::string& name, MorningStar* morning_star);

	private:
		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
