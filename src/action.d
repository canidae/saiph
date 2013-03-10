module action;

import world;

abstract class Action {
	int priority;

	@property final string command() {
		if (_commandId < _commands.length)
			return _commands[_commandId];
		return "";
	}

	/* default update behaviour, this will almost always be overridden by actions */
	void update() {
		++_commandId;
	}

private:
	uint _commandId;
	string[] _commands;
}

class Answer : Action {
	this(string answer) {
		_commands ~= answer;
	}
}

class Search : Action {
	this() {
		_commands ~= "16s";
	}

	override void update() {
		/* TODO: update search counter on adjacent tiles on level */
		++_commandId;
	}
}
