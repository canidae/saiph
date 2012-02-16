module data.world;

import data.level;
import data.point;
import io.log;
import std.stdio;

class World {
	public:
		static bool question;
		static bool menu;

		static void newScreen() {
			question = false;
			changedMapLocations.length = 0;
			++sequence;
		}

		static void updateScreen(Point p, char symbol, int color) {
			if (screen[p.row][p.col].sequence != sequence && (screen[p.row][p.col].symbol != symbol || screen[p.row][p.col].color != color) && insideMap(p)) {
				/* this test may theoretically fail, but it's exceptionally unlikely */
				changedMapLocations ~= p;
			}
			screen[p.row][p.col] = PointData(sequence, symbol, color);
		}

		static void updateMap() {
			/* TODO */
			foreach (p; changedMapLocations) {
				char symbol = screen[p.row][p.col].symbol;
				int color = screen[p.row][p.col].color;
			}
			for (int r = 0; r < ROWS; ++r) {
				for (int c = 0; c < COLS; ++c)
					write(screen[r][c].symbol);
				writeln();
			}
		}

	private:
		struct PointData {
			int sequence;
			char symbol;
			int color;
		}

		static Logger log;
		static Level[] levels;
		static Point[] changedMapLocations;
		static PointData[COLS][ROWS] screen; // note that array declaration is read from right to left in D, unlike left to right in C++/Java
		static int sequence;

		static this() {
			log = new Logger("world");
		}
}

public:
/* dimensions */
immutable ROWS = 24;
immutable COLS = 80;
immutable MAP_ROW_BEGIN = 1;
immutable MAP_ROW_END = 21;
immutable MAP_COL_BEGIN = 0;
immutable MAP_COL_END = 79;

bool insideMap(Point p) {
	return !(p.row < MAP_ROW_BEGIN || p.row > MAP_ROW_END || p.col < MAP_COL_BEGIN || p.col > MAP_COL_END);
}
