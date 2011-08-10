#ifndef WORLD_H
#define WORLD_H
/* buffer */
#define BUFFER_SIZE 65536
#define MAX_LEVELNAME_LENGTH 16
/* stuff for parsing attribute & status rows */
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* text used to identify --More-- and menus */
#define MORE "--More--"
#define END "(end) " // yeah, space should be there
#define PAGE "(%d of %d)"
#define PAGE_DIRTY "%*[^(]" PAGE
#define SPEED_PAUSE 0
#define SPEED_SLOW 1
#define SPEED_FAST 2

#include <list>
#include <map>
#include <string>
#include <vector>
#include <sys/time.h>
#include "Command.h"
#include "Globals.h"
#include "Level.h"
#include "Point.h"
#include "Saiph.h"
#include "Tile.h"

namespace action {
	class Action;
}
class Connection;

class World {
public:
	typedef void (*drawfunc)(void* cookie, Level& lv, const Point& pt, unsigned char& symbol, unsigned char& color);
	static char view(const Point& point);
	static int color(const Point& point);
	static const Point& cursor();
	static bool menu();
	static bool question();
	static int curPage();
	static int maxPage();
	static int turn(); // corresponds to monster_moves and the T: counter
	static unsigned int internalTurn(); // monotonically increases but corresponds to nothing in nethack
	static int subTurn(); // index of current move in current turn() or -1 if unknown
	static const std::vector<Point>& changes();
	static void forgetChanges();
	static const Coordinate& branchCoordinate(int branch);
	static Level& level();
	static Level& level(int level);
	static int findLevel(int branch, int depth);
	static const std::vector<Level>& levels();
	static int currentPriority();
	static action::Action* lastAction();
	static std::string lastData();
	static int lastActionID();
	static void init(const std::string& logfile, int connection_type);
	static void destroy();
	static void registerAnalyzer(analyzer::Analyzer* const analyzer);
	static void unregisterAnalyzer(analyzer::Analyzer* const analyzer);
	static void registerDrawFunc(char key, drawfunc fnc, void* cookie);
	static bool setAction(action::Action* action, bool deleteAction = true);
	static bool queueAction(action::Action* action);
	static unsigned char directLine(Point point, bool ignore_sinks, bool ignore_boulders, int eff_range, int danger_range);
	static std::string cursorMoves(Point source, const Point& target);
	static Tile& shortestPath(const Point& target);
	static Tile shortestPath(const Coordinate& target);
	static Tile shortestPath(unsigned char symbol);
	static void run(int init_speed);

private:
	static Connection* _connection;
	static action::Action* _action;
	static action::Action* _last_action;
	static std::list<action::Action*> _action_queue;
	static bool _changed[MAP_ROW_END + 1][MAP_COL_END + 1]; // just to prevent that same location is added twice in vector "changes"
	static char _view[ROWS][COLS + 1]; // + 1 because we'll make the last character on each line '\0' (for easier parsing)
	static int _color[ROWS][COLS]; // not used for string reading, no need for + 1
	static std::vector<Point> _changes; // list of locations changed since last "frame"
	static std::vector<Level> _levels;
	static std::string _messages;
	static bool _inverse;
	static bool _bold;
	static char _data[BUFFER_SIZE * 2]; // weird errors from valgrind, oh well, we got enough memory
	static int _data_size;
	static Point _cursor; // cursor position
	static std::string _msg_str; // helps fetching messages
	static std::string _msg_buffer; // buffers messages while executing actions
	static Point _last_menu; // needed to help detect menus that persist over turns
	static std::map<std::string, std::vector<int> > _levelmap; // used for faster map recognition
	static char _levelname[MAX_LEVELNAME_LENGTH];
	static timeval _start_time;
	static int _frame_count;
	static bool _menu;
	static bool _question;
	static int _cur_page;
	static int _max_page;
	static int _turn;
	static int _sub_turn;
	static int _min_saiph_energy;
	static int _max_saiph_energy;
	static std::vector<analyzer::Analyzer*> _analyzers;
	static int _last_action_id;
	static unsigned int _internal_turn;
	static Coordinate _branches[BRANCHES];
	static Point _cout_cursor;
	static int _cout_last_color;
	static const char *_ansi_colors[INVERSE_BOLD_WHITE+1];
	static unsigned char _shadow_map_dump[MAP_ROW_END - MAP_ROW_BEGIN + 1][MAP_COL_END - MAP_COL_BEGIN + 1][2];
	static std::string _shadow_rhs[50];
	static int _display_level;
	static std::map<char, std::pair<void*, drawfunc> > _draw_funcs;
	static std::map<char, std::pair<void*, drawfunc> >::iterator _current_draw_func;

	static void drawNormal(void*, Level&, const Point&, unsigned char&, unsigned char&);
	static void drawDirections(void*, Level&, const Point&, unsigned char&, unsigned char&);
	static void drawCosts(void*, Level&, const Point&, unsigned char&, unsigned char&);
	static void drawLight(void*, Level&, const Point&, unsigned char&, unsigned char&);

	static void dumpMap(Level& which);
	static void dumpMaps();
	static void doCommands();
	static void initTermios();
	static void endTermios();
	static void setKeyWait(bool);
	static void coutSetColor(int);
	static void coutOneChar(int,unsigned char);
	static void coutGoto(int,int);
	static void coutRhsLine(int, const std::string&);

	static void addChangedLocation(const Point& point);
	static void detectPosition();
	static bool executeCommand(const std::string& command);
	static void fetchMenu();
	static void fetchMenuText(int stoprow, int startcol, bool addspaces);
	static void fetchMessages();
	static void handleEscapeSequence(int* pos, int* color);
	static void update();
	static void refresh();
};
#endif
