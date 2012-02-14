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

	static void addChangedLocation(Point p, char symbol, int color) {
		/* possible optimization: make sure that a point is only added once? */
		//writef("(%s,%s) %s %s | ", p.row, p.col, symbol, color);
		_changedLocations ~= p;
	}

	static void clearChangedLocations() {
		_changedLocations.length = 0;
	}

private:
	static Level[] _levels;
	static Point[] _changedLocations;
}
