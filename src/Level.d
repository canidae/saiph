import Point;
import Tile;

class Level {
public:
	this(int level, int depth) {
		_level = level;
		_depth = depth;
		for (int r = 0; r < tiles.length; ++r) {
			for (int c = 0; c < tiles[r].length; ++c)
				tiles[r][c] = new Tile(&this, Point(r, c));
		}
	}

	@property auto level() const {
		return _level;
	}

	@property auto depth() const {
		return _depth;
	}

	@property auto branch() const {
		return _branch;
	}

	@property auto branch(int branch) {
		return _branch = branch;
	}

	@property auto walls_diggable() const {
		return _walls_diggable;
	}

	@property auto walls_diggable(bool walls_diggable) {
		return _walls_diggable = walls_diggable;
	}

	@property auto floor_diggable() const {
		return _floor_diggable;
	}

	@property auto floor_diggable(bool floor_diggable) {
		return _floor_diggable = floor_diggable;
	}

	auto tileAt(ref const Point p) const {
		return &tiles[p.row][p.col];
	}

private:
	int _level;
	Tile[24][80] tiles;
	int _depth;
	int _branch;
	bool _walls_diggable;
	bool _floor_diggable;
}
