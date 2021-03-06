#pragma once
#define TCOD_SDL2
class GameEvent {
public:
	enum GAME_EVENT {
		NONE,
		TO_MAIN_MENU,
		STARTUP_NEW_GAME,
		STARTUP_LOAD_GAME,
		NEW_ESC_MENU,
		DELETE_ESC_MENU,
		NEW_INFO_VIEWER,
		DELETE_INFO_VIEWER,
		NEW_WORLD,
		NEW_SOLAR_SYSTEM,
		NEW_STAR_SECTOR,
		NEW_GALAXY,
		NEW_WORLD_MAP,
		ENTER_WORLD_TILE,
		ENTER_SPACESHIP,
		OPEN_INVENTORY,
		CLOSE_INVENTORY,
		UPWARDS,
		HIGHLIGHT_PLAYER,
		DEHIGHLIGHT_PLAYER,
		SAVE_SCREEN,
		LOAD_LEVEL,
		TEST_LEVEL,
		OPEN_MAP,
		CLOSE_MAP,
		MOUSE_MOVE,
		TEST_LEVEL2,
		EXIT,
	};
	enum class GAME_STATUS {
		NONE,
		INFO_VIEWER_OPEN,
	};

	GameEvent();
	~GameEvent();

	static GAME_EVENT get_event() { return _currevent; }

	static GAME_EVENT get_last_event() { return _lastevent; }

	static void set_event(GAME_EVENT ev) { if (!_locked) { _currevent = ev; _lastevent = ev; } }

	static void lock_event() { _locked = true; }

	static void unlock_event() { _locked = false; }

	static void reset_event() { _currevent = NONE; }

	static int get_param() { return _param; }

private:
	static GAME_EVENT _currevent;
	static GAME_EVENT _lastevent;
	static bool _locked;
	static int _param;
};
