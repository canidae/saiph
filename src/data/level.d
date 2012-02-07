module data.level;

import data.coordinate;
import data.point;
import data.tile;

class Level {
public:
	this(int level, string name, int depth) {
		_level = level;
		_name = name;
		_depth = depth;
		foreach (int r; 0 .. tiles.length) {
			foreach (int c; 0 .. tiles[r].length)
				tiles[r][c] = new Tile(Coordinate(this, Point(r, c)));
		}
	}

	@property auto level() const {
		return _level;
	}

	@property auto name() const {
		return _name;
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

	auto tileAt(Point p) const {
		return &tiles[p.row][p.col];
	}

	bool opEquals()(Level l) const {
		return level == l.level;
	}

private:
	int _level;
	string _name;
	Tile[24][80] tiles;
	int _depth;
	int _branch;
	bool _walls_diggable;
	bool _floor_diggable;
}
