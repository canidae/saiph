module data.level;

import data.coordinate;
import data.point;
import data.tile;
import data.world;

class Level {
	public:
		int level;
		string name;
		int depth;
		int branch;
		bool walls_diggable;
		bool floor_diggable;

		this(int level, string name, int depth) {
			this.level = level;
			this.name = name;
			this.depth = depth;
			foreach (int r; 0 .. tiles.length) {
				foreach (int c; 0 .. tiles[r].length)
					tiles[r][c] = new Tile(Coordinate(this, Point(r, c)));
			}
		}

		Tile tileAt(Point p) {
			return tiles[p.row][p.col];
		}

		bool opEquals()(Level l) {
			return level == l.level;
		}

	private:
		Tile[MAP_COL_END + 1][MAP_ROW_END + 1] tiles; // note that array declaration is read from right to left in D, unlike left to right in C++/Java
}
