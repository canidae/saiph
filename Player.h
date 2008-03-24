using namespace std;

struct Attributes {
	int strength;
	int dexterity;
	int constitution;
	int intelligence;
	int wisdom;
	int charisma;
	int alignment; // -1 chaotic, 0 neutral, 1 lawful
};

struct Life {
	int hitpoints;
	int hitpoints_max;
	int power;
	int power_max;
	int armor_class;
	int experience;
};

struct Position {
	int col;
	int row;
	int dlvl;
	int zorkmids;
	int turn;
};

class Player {
	public:
		/* variables */
		Attributes attributes;
		Life life;
		Position position;

		/* constructors */
		Player();

		/* destructors */
		~Player();

		/* methods */

	private:
};
