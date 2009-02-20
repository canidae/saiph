#ifndef MONSTER_H
#define MONSTER_H

#include <limits.h>
#include <map>
#include <string>
#include <vector>
#include "Globals.h"

struct MonsterAttack {
	int type;
	int damage_type;
	int dice;
	int sides;
};

struct MonsterData {
	std::string name;
	unsigned char symbol;
	int difficulty;
	int move_rate;
	int armor_class;
	int magic_resistance;
	int alignment;
	int geno_flags;
	MonsterAttack attack[6];
	int weight;
	int nutrition;
	int extension;
	int sounds;
	int size;
	int resistances;
	int resistances_conferred;
	int m1;
	int m2;
	int m3;
	int color;
};

class Monster {
	public:
		unsigned char symbol;
		int color;
		int last_seen;
		bool visible;
		int attitude;
		bool shopkeeper;
		bool priest;
		bool minotaur;

		static std::vector<MonsterData> monsters;

		Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0, int last_seen = -1, bool visible = false, int attitude = ATTITUDE_UNKNOWN, bool shopkeeper = false, bool priest = false, bool minotaur = false);

	private:
		static bool initialized;
		static int monster_symbol_mapping[UCHAR_MAX + 1][INVERSE_BOLD_WHITE + 1];
		static std::map<string, int> monster_name_mapping;

		Monster(std::string name, int difficulty, int move_rate, int armor_class, int magic_resistance, int alignment, int geno_flags, MonsterAttack ma1, MonsterAttack ma2, MonsterAttack ma3, MonsterAttack ma4, MonsterAttack ma5, MonsterAttack ma6, int weight, int nutrition, int extension, int sounds, int size, int resistances, int resistances_conferred, int m1, int m2, int m3, int color);
};
#endif
