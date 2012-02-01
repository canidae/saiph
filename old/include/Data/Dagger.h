#ifndef DATA_DAGGER_H
#define DATA_DAGGER_H

#include <map>
#include <string>
#include "Data/Weapon.h"

namespace data {

	class Dagger : public Weapon {
	public:
		virtual ~Dagger();

		static void init();
		static const std::map<const std::string, const Dagger*>& daggers();

	protected:
		Dagger(const std::string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Dagger* dagger);

	private:
		static std::map<const std::string, const Dagger*> _daggers;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
