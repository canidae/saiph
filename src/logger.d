module logger;

import std.datetime;
import std.file;
import std.stdio;
import std.string;

immutable int level = 3;

class Logger {
	public:
		this(string name) {
			/* TODO: seems to be a bug in d compiler here
			   if (!(name in files))
			   files[name] = File("logs/" ~ name ~ ".log", "w");
			   file = files[name];
			 */
			file = File("logs/" ~ name ~ ".log", "w"); // TODO: remove when bug is fixed (see log(T...))
		}

		~this() {
			// TODO: what if multiple classes use same file for logging?
			file.close();
		}

		void info(T...)(lazy string message, lazy T t) {
			static if (level > 2)
				log(file, "INFO", message, t);
		}

		void warning(T...)(lazy string message, lazy T t) {
			static if (level > 1)
				log(file, "WARNING", message, t);
		}

		void error(T...)(lazy string message, lazy T t) {
			static if (level > 0)
				log(file, "ERROR", message, t);
		}

	private:
		static int entry;
		static File[string] files;
		File file;

		static void log(T...)(File file, string level, string message, T t) {
			DateTime time = cast(DateTime) Clock.currTime;
			file.writef("%06s | %s [%.1s][%10.10s] ", ++entry, time.toSimpleString(), level, file.name[indexOf(file.name, '/') + 1 .. lastIndexOf(file.name, '.')]);
			file.writefln(message, t);
		}
}
