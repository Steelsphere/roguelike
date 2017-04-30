#include "Game.h"

Game::Game() : _screen_width(GameObjects::screen_width), _screen_height(GameObjects::screen_height), _time(0)
{
}


Game::~Game()
{
}

void Game::init() {
	TCODConsole::setCustomFont("terminal12x12_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(_screen_width, _screen_height, "Roguelike", false, TCOD_RENDERER_GLSL);
//	TCODSystem::setFps(60);
}

void Game::start() {
	TCODConsole::root->clear();

	std::cout << "Size of one actor: " << sizeof(Actor) << std::endl;

	_level = new Level;
	_level->generate_level(1024, Level::DESERT);
	
	_player = new Player(250, 250, 0, '@', TCODColor::blue);
	Input::set_input_reciever(_player);
	_player->spawn_player_in_world();
	_camera = new Camera(_player);
	_camera->set_level(_level);
	_camera->update();

	_level->save_level_image();

	game_loop();
}

void Game::game_loop() {
	while (true) {
		_event = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &_key, &_mouse);
		
		if (_event) {
			on_event(_event);
		}
		
		if (TCODConsole::isWindowClosed()) {
			on_exit();
		}
		
		if (GameObjects::update) {
			update();
		}
		
		TCODConsole::flush();
		std::cout << "FPS: " << TCODSystem::getFps() << "\r";
		_time += 0.01f;
	}
}

void Game::on_event(TCOD_event_t e) {
	Input::input(_key);
}

void Game::on_exit() {
	exit(0);
}

void Game::update() {
	_num_actors_drawn = 0;
	std::vector<Actor*>* actors = _level->get_actors();
	
	Actor* a;
	int* ar, *cr;
	
	_camera->update();
	
	TCODMap* fov = _level->get_fov_map();
	fov->computeFov(_player->get_world_pos()[0], _player->get_world_pos()[1], 100);

	for (int i = 0; i < actors->size(); i++) {
		
		a = actors->operator[](i);
		
		ar = a->get_screen_pos();
		cr = _camera->get_screen_pos();
		
		if (ar[0] < cr[0] + _screen_width / 2 + 10 &&
			ar[1] < cr[1] + _screen_height / 2 + 10 &&
			ar[0] > cr[0] - _screen_width / 2 - 10 &&
			ar[1] > cr[1] - _screen_height / 2 - 10
			) {
				if (fov->isInFov(a->get_world_pos()[0], a->get_world_pos()[1])) {
					_num_actors_drawn++;
					a->draw();
				}
			} 
	}
//	std::cout << "Number of actors drawn " << _num_actors_drawn << "\r";
	GameObjects::update = false;
}


