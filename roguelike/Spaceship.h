#pragma once

#define FREIGHTER_MAX_CARGO 1000

#include "Actor.h"
#include "Economy.h"

class StarSector;
class Faction;

class Spaceship : public Actor {
public:
	Spaceship() {}
	Spaceship(char c, StarSector* s, Faction* f);
	~Spaceship();

	virtual void update();
	void path_to_location(int x, int y);
	void follow_path();

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;

	Economy::Goods cost;
	std::vector<Actor*> path;
	Faction* faction;
};

class Freighter : public Spaceship {
public:
	enum FREIGHTER_ACTION {
		NONE,
		PICKUP,
		UNLOAD,
		IDLE
	};

	Freighter() {}
	Freighter(StarSector* s, Faction* f);

	virtual void update() override;

	void route(StarSector* start, StarSector* end, Economy::Goods g);

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;

	Economy::Goods cargo;
	FREIGHTER_ACTION action = NONE;

	StarSector* load_dest = nullptr;
	StarSector* unload_dest = nullptr;

private:
	Economy::Goods _willpickup;
};

class Scout : public Spaceship {
public:
	enum SCOUT_ACTION {
		NONE,
		SCOUT_BEGIN,
		SCOUTING
	};

	Scout() {}
	Scout(StarSector* s, Faction* f);

	virtual void update() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;

	SCOUT_ACTION action = NONE;
};

class Warship : public Spaceship {
public:
	enum MILSHIP_ACTION {
		NONE,
		PATROL_BEGIN,
		PATROLLING,
		ATTACK_BEGIN,
		ATTACKING,
		DEFEND_BEGIN,
		DEFENDING
	};

	Warship() {}
	Warship(StarSector* s, Faction* f);

	virtual void update() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;

	MILSHIP_ACTION action = NONE;

private:
	std::vector<StarSector*> _patrol_points;
};
