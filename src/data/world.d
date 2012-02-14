module data.world;

import data.level;
import data.point;
import std.stdio;

class World {
	public:
		/* dimensions */
		immutable ROWS = 24;
		immutable COLS = 80;
		/* colors & attributes */
		immutable NO_COLOR = 0;
		immutable BOLD = 1;
		immutable INVERSE = 7;
		immutable BOLD_OFFSET = 60;
		immutable INVERSE_OFFSET = 10;

		static void clearChangedLocation() {
			changedLocations.length = 0;
			++sequence;
		}

		static void addChangedLocation(Point p, char symbol, int color) {
			if (pointData[p.row][p.col].sequence == sequence) {
				/* this test may theoretically fail, but it's exceptionally unlikely */
				changedLocations ~= p;
			}
			pointData[p.row][p.col] = PointData(sequence, symbol, color);
		}

		static void updateMap() {
			/* TODO */
			foreach (p; changedLocations) {
				char symbol = pointData[p.row][p.col].symbol;
				int color = pointData[p.row][p.col].color;
			}
		}

	private:
		static Level[] levels;
		static Point[] changedLocations;
		static PointData[ROWS][COLS] pointData;
		static int sequence;
}

private:
	struct PointData {
		int sequence;
		char symbol;
		int color;
	}
