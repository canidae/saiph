module data.level;

import data.coordinate;
import data.point;
import data.tile;

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
			foreach (int r; 0 .. _tiles.length) {
				foreach (int c; 0 .. _tiles[r].length)
					_tiles[r][c] = new Tile(Coordinate(this, Point(r, c)));
			}
		}

		auto tileAt(Point p) {
			return _tiles[p.row][p.col];
		}

		bool opEquals()(Level l) {
			return level == l.level;
		}

	private:
		Tile[24][80] _tiles;
}
