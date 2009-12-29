#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

#include "Actions/Action.h"

#define MOVE_COST_MEDIAN 200
#define MOVE_WHERE_TO_GO "  Where do you want to travel to?  "

namespace action {

	class Move : public Action {
	public:
		static const int ID;

		Move(analyzer::Analyzer* analyzer, const Tile& target, int priority, bool travel = true) : Action(analyzer), _target(target), _travel_target(Action::NOOP) {
			if (_target.distance() < 3) {
				travel = false;
			} else if (_last_target == target.coordinate()) {
				travel = false;
			} else if (travel) {
				for (std::map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
					if (m->second.visible()) {
						travel = false;
						break;
					}
				}
			}
			if (travel)
				_do_travel = Command("_", priority);
			else
				_do_travel = Command(_target.direction(), priority);
		}

		virtual ~Move() {
		}

		static int calculatePriority(int max_priority, unsigned int cost) {
			return max_priority * MOVE_COST_MEDIAN / (cost + MOVE_COST_MEDIAN - 1);
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_travel;

			case 1:
				return _travel_target;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MOVE_WHERE_TO_GO) != std::string::npos) {
				_last_target = _target.coordinate();
				_travel_target = Command(World::cursorMoves(World::cursor(), _target.coordinate()) + ".", PRIORITY_CONTINUE_ACTION);
				_sequence = 1;
			} else {
				_sequence = 2;
			}
		}

		virtual void failed() {
			/* we'll assume we're moving if the command that's stuck is a direction.
			 * if not, it's probably not a big deal */
			switch (command().command()[0]) {
			case NW:
			case NE:
			case SW:
			case SE:
				/* moving diagonally failed.
				 * we could be trying to move diagonally into a door we're
				 * unaware of because of an item blocking the door symbol.
				 * make the tile UNKNOWN_TILE_DIAGONALLY_UNPASSABLE */
				World::level().setDungeonSymbol(directionToPoint((unsigned char) command().command()[0]), UNKNOWN_TILE_DIAGONALLY_UNPASSABLE);
				break;

			case N:
			case E:
			case S:
			case W:
				/* moving cardinally failed, possibly item in wall.
				 * make the tile UNKNOWN_TILE_UNPASSABLE */
				World::level().setDungeonSymbol(directionToPoint((unsigned char) command().command()[0]), UNKNOWN_TILE_UNPASSABLE);
				break;

			default:
				/* not good. we're not moving and we're stuck */
				analyzer()->actionFailed();
				break;
			}
		}

	private:
		static Point _last_target;
		Tile _target;
		Command _do_travel;
		Command _travel_target;

		Point directionToPoint(unsigned char direction) {
			/* return the position we'd be at if we do the given move */
			Point pos = Saiph::position();
			switch (direction) {
			case NW:
				pos.moveNorthwest();
				break;

			case N:
				pos.moveNorth();
				break;

			case NE:
				pos.moveNortheast();
				break;

			case E:
				pos.moveEast();
				break;

			case SE:
				pos.moveSoutheast();
				break;

			case S:
				pos.moveSouth();
				break;

			case SW:
				pos.moveSouthwest();
				break;

			case W:
				pos.moveWest();
				break;
			}
			return pos;
		}
	};
}
#endif
