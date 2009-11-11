#ifndef DATA_ATTACK_H
#define DATA_ATTACK_H

/* type */
#define AT_ANY          -1      /* fake attack; dmgtype_fromattack wildcard */
#define AT_NONE         0       /* passive monster (ex. acid blob) */
#define AT_CLAW         1       /* claw (punch, hit, etc.) */
#define AT_BITE         2       /* bite */
#define AT_KICK         3       /* kick */
#define AT_BUTT         4       /* head butt (ex. a unicorn) */
#define AT_TUCH         5       /* touches */
#define AT_STNG         6       /* sting */
#define AT_HUGS         7       /* crushing bearhug */
#define AT_SPIT         10      /* spits substance - ranged */
#define AT_ENGL         11      /* engulf (swallow or by a cloud) */
#define AT_BREA         12      /* breath - ranged */
#define AT_EXPL         13      /* explodes - proximity */
#define AT_BOOM         14      /* explodes when killed */
#define AT_GAZE         15      /* gaze - ranged */
#define AT_TENT         16      /* tentacles */
#define AT_WEAP         254     /* uses weapon */
#define AT_MAGC         255     /* uses magic spell(s) */

/* damage type */
#define AD_ANY          -1      /* fake damage; attacktype_fordmg wildcard */
#define AD_PHYS         0       /* ordinary physical */
#define AD_MAGM         1       /* magic missiles */
#define AD_FIRE         2       /* fire damage */
#define AD_COLD         3       /* frost damage */
#define AD_SLEE         4       /* sleep ray */
#define AD_DISN         5       /* disintegration (death ray) */
#define AD_ELEC         6       /* shock damage */
#define AD_DRST         7       /* drains str (poison) */
#define AD_ACID         8       /* acid damage */
#define AD_SPC1         9       /* for extension of buzz() */
#define AD_SPC2         10      /* for extension of buzz() */
#define AD_BLND         11      /* blinds (yellow light) */
#define AD_STUN         12      /* stuns */
#define AD_SLOW         13      /* slows */
#define AD_PLYS         14      /* paralyses */
#define AD_DRLI         15      /* drains life levels (Vampire) */
#define AD_DREN         16      /* drains magic energy */
#define AD_LEGS         17      /* damages legs (xan) */
#define AD_STON         18      /* petrifies (Medusa, cockatrice) */
#define AD_STCK         19      /* sticks to you (mimic) */
#define AD_SGLD         20      /* steals gold (leppie) */
#define AD_SITM         21      /* steals item (nymphs) */
#define AD_SEDU         22      /* seduces & steals multiple items */
#define AD_TLPT         23      /* teleports you (Quantum Mech.) */
#define AD_RUST         24      /* rusts armour (Rust Monster)*/
#define AD_CONF         25      /* confuses (Umber Hulk) */
#define AD_DGST         26      /* digests opponent (trapper, etc.) */
#define AD_HEAL         27      /* heals opponent's wounds (nurse) */
#define AD_WRAP         28      /* special "stick" for eels */
#define AD_WERE         29      /* confers lycanthropy */
#define AD_DRDX         30      /* drains dexterity (quasit) */
#define AD_DRCO         31      /* drains constitution */
#define AD_DRIN         32      /* drains intelligence (mind flayer) */
#define AD_DISE         33      /* confers diseases */
#define AD_DCAY         34      /* decays organics (brown Pudding) */
#define AD_SSEX         35      /* Succubus seduction (extended) */
/* If no SEDUCE then same as AD_SEDU */
#define AD_HALU         36      /* causes hallucination */
#define AD_DETH         37      /* for Death only */
#define AD_PEST         38      /* for Pestilence only */
#define AD_FAMN         39      /* for Famine only */
#define AD_SLIM         40      /* turns you into green slime */
#define AD_ENCH         41      /* remove enchantment (disenchanter) */
#define AD_CORR         42      /* corrode armor (black pudding) */

#define AD_CLRC         240     /* random clerical spell */
#define AD_SPEL         241     /* random magic spell */
#define AD_RBRE         242     /* random breath weapon */

#define AD_SAMU         252     /* hits, may steal Amulet (Wizard) */
#define AD_CURS         253     /* random curse (ex. gremlin) */

namespace data {

	class Attack {
	public:
		Attack(int type = 0, int damage_type = 0, int dices = 0, int sides = 0);
		virtual ~Attack();

		int type() const;
		int damageType() const;
		int dices() const;
		int sides() const;
		int minDamage() const;
		int avgDamage() const;
		int maxDamage() const;

	private:
		int _type;
		int _damage_type;
		int _dices;
		int _sides;
		int _min_damage;
		int _avg_damage;
		int _max_damage;
	};
}
#endif
