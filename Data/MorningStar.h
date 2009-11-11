#ifndef DATA_MORNINGSTAR_H
#define DATA_MORNINGSTAR_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class MorningStar : public Weapon {
	public:
		virtual ~MorningStar();

		static void init();
		static const std::map<const std::string, const MorningStar*>& morningStars();

	protected:
		MorningStar(const std::string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const MorningStar* morning_star);

	private:
		static std::map<const std::string, const MorningStar*> _morning_stars;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
