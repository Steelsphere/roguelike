#pragma once
#define TCOD_SDL2

#include "Actor.h"
#include "Tile.h"
#include "Vec2.h"

#include <FastNoise.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>

class Camera;

class Level {
public:
	struct Chunk {
		struct ChunkTile {
			ChunkTile(Actor* a) {
				actor = a;
			}
			ChunkTile(Actor* a, const Vec2& pos) {
				actor = a;
				relpos = pos;
			}

			Actor* actor;
			Vec2 relpos;
		};

		Chunk() {}
		Chunk(int x, int y) {
			pos.x = x;
			pos.y = y;
		}
		void load_chunk(const Camera& camera);
		void unload_chunk();
		Vec2 get_midpoint();

		std::vector<ChunkTile> chunktiles;
		Vec2 pos;
		bool loaded = false;
	};

	enum LEVEL_TYPE {
		NONE,
		TEST,
		GALAXY,
		STAR_SECTOR,
		SOLAR_SYSTEM,
		SPACE,

		// ALL LEVEL TYPES GREATER THAN WORLD_MAP ARE SURFACE LEVELS
		WORLD_MAP,

		GRASSLAND,
		FOREST,
		HILLS,
		DESERT,
		SNOWY_TAIGA,
		OCEAN,
	};

	enum GENERATION_FLAG {
		TEMPERATE_FLORA,
		DESERT_FLORA,
		COLD_FLORA,
	};

	int id;

	Level();
	~Level();

	void generate_level(int size, LEVEL_TYPE type);

	std::vector<Actor*>* get_actors() { return &_actors; }

	void set_actors(std::vector<Actor*>* v) { _actors = (*v); }

	std::vector<std::vector<std::vector<Actor*>>>* get_actor_map() { return &_map; }

	static TCODMap* get_fov_map() { return _fovmap; }

	void generate_terrain(float frequency, float water_threshold, float terrain_threshold, float beach_size,
		Tile::TILE_TYPE water = Tile::WATER,
		Tile::TILE_TYPE terrain = Tile::GRASS,
		Tile::TILE_TYPE wall = Tile::DIRT_WALL,
		Tile::TILE_TYPE beach = Tile::SAND);

	void generate_trees(std::uniform_int_distribution<int> r, Level::GENERATION_FLAG flag = Level::TEMPERATE_FLORA);

	void generate_flora(std::uniform_int_distribution<int> r, Level::GENERATION_FLAG flag = Level::TEMPERATE_FLORA);

	void generate_space();

	void generate_space_obj(std::uniform_int_distribution<int> r, LEVEL_TYPE type);

	void generate_structures(int num);

	void update();

	void update_tile(int x, int y, int z);

	void save_level_image(const std::string path);

	void save_level_file(const std::string path);

	int get_size() { return _width + _height; }

	static Level* load_level_file(const std::string path);

	LEVEL_TYPE get_type() { return _type; }

	void set_type(LEVEL_TYPE type) { _type = type; }

	std::string get_savedir() { return _savedir; }

	void set_savedir(const std::string& dir) { _savedir = dir; }

	void generate_test_level();

	void set_size(int s) { _width = s / 2; _height = s / 2; }

	void generate_chunks();

	void update_chunks(const Camera& camera);

	std::vector<Actor*> get_loaded_actors();

	void chunk_add_actor(Actor* a);

	void chunk_delete_actor(Actor* a);

private:
	int _width, _height;
	std::vector<Actor*> _actors;
	std::vector<std::vector<std::vector<Actor*>>> _map;
	static TCODMap* _fovmap;
	FastNoise _noise;
	LEVEL_TYPE _type;
	std::string _savedir = "";
	std::vector<Chunk> _chunks;
	std::vector<Chunk*> _loaded_chunks;
};
