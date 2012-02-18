module data.level;

import data.coordinate;
import data.point;
import data.tile;
import data.world;

class Level {
	public:
		int id;
		string name;
		int depth;
		int branch;
		bool walls_diggable;
		bool floor_diggable;

		this(string name, int depth) {
			this.id = idSequence++;
			this.name = name;
			this.depth = depth;
			foreach (int r; MAP_ROW_BEGIN .. MAP_ROW_END + 1) {
				foreach (int c; MAP_COL_BEGIN .. MAP_COL_END + 1)
					tiles[r][c] = new Tile(Coordinate(this, Point(r, c)));
			}
		}

		Tile tileAt(Point p) {
			return tiles[p.row][p.col];
		}

		bool opEquals()(Level l) {
			return id == l.id;
		}

		int opCmp()(Level l) {
			return id - l.id;
		}

	private:
		static int idSequence;
		Tile[MAP_COL_END + 1][MAP_ROW_END + 1] tiles; // note that array declaration is read from right to left in D, unlike left to right in C++/Java
}
