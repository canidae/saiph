#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

#include "Point.h"
#include "Tile.h"
#include "Actions/Action.h"

#define MOVE_COST_MEDIAN 200
#define MOVE_WHERE_TO_GO "  Where do you want to travel to?  "

namespace action {
	class Move : public Action {
	public:
		static const int ID;

		Move(analyzer::Analyzer* analyzer, const Tile& target, int priority, bool travel = true);
		virtual ~Move();

		static int calculatePriority(int max_priority, unsigned int cost);
		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);
		virtual void failed();
		Tile& target();

	private:
		static Point _last_target;
		Tile _target;
		Command _do_travel;
		Command _travel_target;

		Point directionToPoint(unsigned char direction);
	};
}
#endif
