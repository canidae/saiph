module io.log;

import std.datetime;
import std.file;
import std.stdio;

immutable int level = 3;

class Logger {
public:
	this(string name) {
		this.name = name;
	}

	~this() {
		close(name);
	}

	/* TODO: variadic parameters */
	void info(lazy string message) {
		static if (level > 2)
			log(name, "INFO", message);
	}

	void warning(lazy string message) {
		static if (level > 1)
			log(name, "WARNING", message);
	}

	void error(lazy string message) {
		static if (level > 0)
			log(name, "ERROR", message);
	}

private:
	static int entry;
	static File[string] files;
	string name;

	static void log(string name, string level, string message) {
		if (!(name in files))
			files[name] = File("logs/" ~ name, "w");
		DateTime time = cast(DateTime) Clock.currTime;
		files[name].writefln("%06s | %s [%7s] %s", entry, time.toSimpleString(), level, message);
	}

	static void close(string name) {
		if (name in files) {
			files[name].close();
			files.remove(name);
		}
	}
}
