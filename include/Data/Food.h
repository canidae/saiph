#ifndef DATA_FOOD_H
#define DATA_FOOD_H

#include <map>
#include <string>
#include "Data/Item.h"

#define EAT_EFFECT_ACIDIC           (1 << 0)
#define EAT_EFFECT_AGGRAVATE        (1 << 1)
#define EAT_EFFECT_DIE              (1 << 2)
#define EAT_EFFECT_DWARF            (1 << 3)
#define EAT_EFFECT_ELF              (1 << 4)
#define EAT_EFFECT_GNOME            (1 << 5)
#define EAT_EFFECT_HALLUCINOGENIC   (1 << 6)
#define EAT_EFFECT_HUMAN            (1 << 7)
#define EAT_EFFECT_LYCANTHROPY      (1 << 8)
#define EAT_EFFECT_MIMIC            (1 << 9)
#define EAT_EFFECT_PETRIFY          (1 << 10)
#define EAT_EFFECT_POISONOUS        (1 << 11)
#define EAT_EFFECT_POLYMORPH        (1 << 12)
#define EAT_EFFECT_SLIME            (1 << 13)
#define EAT_EFFECT_STUN             (1 << 14)
#define EAT_EFFECT_TELEPORTITIS     (1 << 15)
#define EAT_EFFECT_VEGAN            (1 << 16)
#define EAT_EFFECT_VEGETARIAN       (1 << 17)
#define EAT_EFFECT_STRENGTH         (1 << 18)
#define EAT_EFFECT_GAIN_LEVEL       (1 << 19)
#define EAT_EFFECT_HEAL             (1 << 20)
#define EAT_EFFECT_SPEED_TOGGLE     (1 << 21)
#define EAT_EFFECT_CURE_STONING     (1 << 22)
#define EAT_EFFECT_REDUCE_STUNNING  (1 << 23)
#define EAT_EFFECT_REDUCE_CONFUSION (1 << 24)
#define EAT_EFFECT_INVISIBILITY     (1 << 25)
#define EAT_EFFECT_CURE_BLINDNESS   (1 << 26)
#define EAT_EFFECT_CURE_SICKNESS    (1 << 27)
#define EAT_EFFECT_CURE_LYCANTHROPY (1 << 28)
#define EAT_EFFECT_NEVER_ROT        (1 << 29)
#define EAT_EFFECT_ESP              (1 << 30)
#define EAT_EFFECT_CONFUSE          (1 << 31)

namespace data {

	class Food : public Item {
	public:
		virtual ~Food();

		static void init();
		static const std::map<const std::string, const Food*>& foods();
		int nutrition() const;
		int time() const;
		int effects() const;

	protected:
		Food(const std::string& name, int cost, int weight, int material, unsigned long long properties, int nutrition, int time, int effects);

		static void addToMap(const std::string& name, const Food* food);

	private:
		static std::map<const std::string, const Food*> _foods;
		const int _nutrition;
		const int _time;
		const int _effects;

		static void create(const std::string& name, int cost, int weight, int material, unsigned long long properties, int nutrition, int time, int effects);
	};
}
#endif
