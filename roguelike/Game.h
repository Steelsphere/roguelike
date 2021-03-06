#pragma once
#define TCOD_SDL2

#include <libtcod.h>
#include <string>

class Level;
class Camera;
class World;
class Player;
class LightSystem;
class Time;
class Log;
class Status;
class MainMenu;
class ESCMenu;
class InfoViewer;
class InventoryPanel;
class Map;
class LoadingScreen;
class SimulationScreen;
class TextBox;
class SelectionBox;

class Game {
public:
	Game();

	~Game();

	void init();

	void start();

	void game_loop();

	void on_event(TCOD_event_t e);

	void game_event();

	void update();

	void update_gui(bool all = false);

	void update_characters();

	void level_setup();

	void loading_screen();

	void exit_game();

	void startup_new_game();

	void startup_load_game();

	void new_main_menu();

	void destroy_main_menu();

	void new_esc_menu();

	void destroy_esc_menu();

	void destroy_garbage();

	void new_info_viewer();

	void destroy_info_viewer();

	void new_world();

	void new_solar_system();

	void new_star_sector();

	void new_galaxy();

	void new_world_map();

	void enter_world_tile();

	void enter_spaceship();

	void open_inventory();

	void close_inventory();

	void upwards();

	void to_galaxy();

	void to_star_sector();

	void to_solar_system();

	void to_world_map();

	void highlight_player();

	void dehighlight_player();

	void save_screen();

	void load_level();

	void save_level();

	template<typename T>
	void fix_tile_id();

	void test_level();

	void open_map();

	void close_map();

	void generate_factions();

	void textbox_game_loop(TextBox* tb);

	void mouse_move();

	void selection_game_loop(SelectionBox* gui);

	void simulation_gui_thread(int simturns);

	void simulation_sim_thread(int simturns);

	void test_level2();

	Level* get_current_level() { return _level; }

private:

	int _screen_width, _screen_height, _num_actors_drawn, _num_updates, _turn;
	int _galaxy_id = 0; int _starsector_id = 0; int _solarsystem_id = 0; int _world_id = 0; int _simturn = 0;
	bool _sim = false;

	TCOD_key_t _key;
	TCOD_mouse_t _mouse;
	TCOD_event_t _event;

	std::string _savegame_directory;

	// Level stuff
	Level* _level = nullptr;
	Camera* _camera = nullptr;
	World* _world = nullptr;

	// Player stuff
	Player* _player = nullptr;
	Player* _dummy = nullptr;
	Level* _spaceship = nullptr;

	// Systems
	LightSystem* _lightsystem = nullptr;
	Time* _time = nullptr;

	// GUI stuff
	Log* _log = nullptr;
	Status* _status = nullptr;
	MainMenu* _MainMenu = nullptr;
	ESCMenu* _ESCMenu = nullptr;
	InfoViewer* _info_viewer = nullptr;
	InventoryPanel* _inv_panel = nullptr;
	Map* _gui_map = nullptr;
	LoadingScreen* _loadingscreen = nullptr;
	SimulationScreen* _sgui = nullptr;
};

extern Game game;