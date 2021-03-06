#pragma once
#define TCOD_SDL2

#include "Actor.h"
#include "Level.h"
#include "GUI.h"
#include "GameObjects.h"

#include <vector>
#include <map>
#include <string>

class Player;
class Camera;
class GameObjects {
public:
	enum Size {
		SMALL = 256,
		MEDIUM = 512,
		LARGE = 1024,
	};

	static int screen_width, screen_height;
	static int ticks;
	static int level_id_to_load;
	static int new_level_id;
	static int old_level_id;
	static int galaxy_size;

	static float time;

	static bool update;
	static bool player_controlled;
	static bool new_turn;

	static Log* log;
	static Camera* camera;

	static std::string savegame_path;
	static std::map<std::string, std::pair<int, int>> map_dir;

	template <typename T>
	static Actor* create_actor_instance() { return new T; }

	static std::map<std::string, Actor*(*)()> type_map;

	static Player* find_player(Level* level);

	static bool file_in_filesystem(const std::string& path, const std::string& file);

	static bool is_directory_empty(const std::string& path);

	static int num_files_in_directory(const std::string& path);

	static std::string get_description(Actor* a);

	template <typename T>
	static void replace_by(T*& a, T* b) {
		delete a;
		a = b;
	}
};
