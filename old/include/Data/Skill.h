#ifndef DATA_SKILL_H
#define DATA_SKILL_H

#include "Globals.h"

#include <string>
#include <vector>
#include <map>

#define P_NONE                   0
#define P_DAGGER                 1
#define P_KNIFE                  2
#define P_AXE                    3
#define P_PICK_AXE               4
#define P_SHORT_SWORD            5
#define P_BROAD_SWORD            6
#define P_LONG_SWORD             7
#define P_TWO_HANDED_SWORD       8
#define P_SCIMITAR               9
#define P_SABER                 10
#define P_CLUB                  11  /* Heavy-shafted bludgeon */
#define P_MACE                  12
#define P_MORNING_STAR          13  /* Spiked bludgeon */
#define P_FLAIL                 14  /* Two pieces hinged or chained together */
#define P_HAMMER                15  /* Heavy head on the end */
#define P_QUARTERSTAFF          16  /* Long-shafted bludgeon */
#define P_POLEARMS              17
#define P_SPEAR                 18
#define P_JAVELIN               19
#define P_TRIDENT               20
#define P_LANCE                 21
#define P_BOW                   22
#define P_SLING                 23
#define P_CROSSBOW              24
#define P_DART                  25
#define P_SHURIKEN              26
#define P_BOOMERANG             27
#define P_WHIP                  28
#define P_UNICORN_HORN          29  /* last weapon */
#define P_FIRST_WEAPON          P_DAGGER
#define P_LAST_WEAPON           P_UNICORN_HORN
#define P_ATTACK_SPELL          30
#define P_HEALING_SPELL         31
#define P_DIVINATION_SPELL      32
#define P_ENCHANTMENT_SPELL     33
#define P_CLERIC_SPELL          34
#define P_ESCAPE_SPELL          35
#define P_MATTER_SPELL          36
#define P_FIRST_SPELL           P_ATTACK_SPELL
#define P_LAST_SPELL            P_MATTER_SPELL
#define P_BARE_HANDED_COMBAT    37
#define P_MARTIAL_ARTS          P_BARE_HANDED_COMBAT    /* Role distinguishes */
#define P_TWO_WEAPON_COMBAT     38      /* Finally implemented */
#define P_RIDING                39      /* How well you control your steed */
#define P_LAST_H_TO_H           P_RIDING
#define P_FIRST_H_TO_H          P_BARE_HANDED_COMBAT
#define P_NUM_SKILLS            (P_LAST_H_TO_H+1)

#define P_UNSKILLED              1
#define P_BASIC                  2
#define P_SKILLED                3
#define P_EXPERT                 4
#define P_MASTER                 5       /* Unarmed combat/martial arts only */
#define P_GRAND_MASTER           6       /* Unarmed combat/martial arts only */

namespace data {
	class Skill {
	public:
		static void init();

		static int parse(const std::string& name);
		static const std::string& name(int skill);
		static const std::vector<std::string>& names();
		static int levelParse(const std::string& name);
		static const std::string& levelName(int level);
		static const std::vector<std::string>& levelNames();

		static int roleMax(int role, int skill);

	private:
		static std::vector<std::string> _names;
		static std::vector<std::string> _level_names;
		static std::map<int,std::vector<int> > _role_maxima;
	};
}

#endif
