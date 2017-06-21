#include "GUI.h"
#include "GameObjects.h"
#include "Input.h"
#include "Actor.h"

#include <algorithm>
#include <cstring>
#include <cmath>

std::vector<GUI*> GUI::_buffer;

GUI::GUI() {}

GUI::GUI(int x, int y, int w, int h, std::vector<Text> text) : 
_x(x), _y(y), _width(w), _height(h), _text(text), _transparency(1.0f), _update(true)
{
	_cons = new TCODConsole(w, h);
	_buffer.push_back(this);
}

GUI::~GUI()
{
	delete _cons;
	
	_buffer.erase(std::remove(_buffer.begin(), _buffer.end(), this), _buffer.end());
	
	TCODConsole::root->rect(_x, _y, _width, _height, true, TCOD_BKGND_ALPHA(0.0f));
	GameObjects::update = true;
	std::cout << "GUIs: " << _buffer.size() << std::endl;
}

void GUI::draw(bool force) {
	if (_update || force) {
		switch (_type) {
		case FILLED_BACKGROUND:
			_cons->rect(0, 0, _width, _height, true);
			break;
		case FILLED_BORDERED_BACKGROUND:
			_cons->printFrame(0, 0, _width, _height, true);
			break;
		}
		for (int i = 0; i < _text.size(); i++) {
			_cons->setColorControl(TCOD_COLCTRL_1, _text[i].color, TCODColor::black);
			_cons->printRect(_text[i].x, _text[i].y, _text[i].w, _text[i].h, (std::string("%c") + _text[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
		}
		_cons->flush();
		TCODConsole::blit(_cons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, _transparency);
		_update = false;
	}
}

void GUI::position_text() {
	for (int i = 0; i < _text.size(); i++) {
		_text[i].x += _x;
		_text[i].y += _y;
	}
}

Message_Box::Message_Box(std::string text, bool nocontinue) : GUI(GameObjects::screen_width/2 - 13, GameObjects::screen_height/2 - 10, 24, 12, std::vector<Text>()) {
	Text top = { 6, 0, 12, 2, "Announcement", TCODColor::red };
	Text message = { 6, 4, 12, 12, text, TCODColor::white };
	if (!nocontinue) {
		Text bottom = { 6, 10, 12, 2, "Press enter to continue", TCODColor::red };
		_text.push_back(bottom);
	}
	_text.push_back(top);
	_text.push_back(message);
	
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
	if (nocontinue) {
		Input::set_mode(Input::NONE);
	}
	else {
		Input::set_mode(Input::ENTER_TO_CONTINUE);
	}
}

void Message_Box::draw(bool force) {
	GUI::draw(force);
	if (Input::get_mode() == Input::NORMAL) {
		Message_Box::~Message_Box();
	}
}

//GUI(0, GameObjects::screen_height - GameObjects::screen_height / 8, GameObjects::screen_width / 2, GameObjects::screen_width / 11, std::vector<Text>())
Log::Log() : GUI(GameObjects::screen_width - 30, 0, 30, GameObjects::screen_height / 2, std::vector<Text>()) {
	Text name = { 1, 0, 3, 1, "Log", TCODColor::red };
	_text.push_back(name);
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
}

void Log::message(const std::string& message, TCODColor color) {
	for (int i = 1; i < _text.size(); i++) {
		_text[i].y--;
		if (_text[i].y <= 0) {
			_text.erase(_text.begin() + i);
		}
	}
	int h = 1;
	int str_s = message.length();
	while (true) {
		str_s -= _width - 4;
		if (str_s <= 0) {
			break;
		}
		h++;
	}
	Text m = { 1, _height - 2, _width - 2, h, "> " + message, color };
	_text.push_back(m);
	while (_text.back().y + _text.back().h > _height - 1) {
		for (int i = 1; i < _text.size(); i++) {
				_text[i].y -= m.h;
				if (_text[i].y <= 0) {
					_text.erase(_text.begin() + i);
				}
		}
	}
	_update = true;
}

//GUI(GameObjects::screen_width / 2, GameObjects::screen_height - GameObjects::screen_height / 8, GameObjects::screen_width / 2, GameObjects::screen_width / 11, std::vector<Text>())
Status::Status() : GUI(GameObjects::screen_width - 30, GameObjects::screen_height / 2, 30, (GameObjects::screen_height / 2) + 1, std::vector<Text>()) {
	Text name = { 1, 0, 6, 1, "Status", TCODColor::red };
	_text.push_back(name);
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
	_update = true;
}

SelectionBox::SelectionBox() {}

SelectionBox::SelectionBox(int x, int y, int w, int h, std::vector<Text> text) : GUI(x, y, w, h, text) {}

void SelectionBox::draw(bool force) {
	
	if (_update || force) {
		for (int i = 0; i < _mtext.size(); i++) {
			_cons->setColorControl(TCOD_COLCTRL_1, _mtext[i].color, TCODColor::black);
			_cons->printRect(_mtext[i].x, _mtext[i].y, _mtext[i].w, _mtext[i].h, (std::string("%c") + _mtext[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
		}

		GUI::draw(force);
	}
	
	if (Input::get_last_key().vk == TCODK_DOWN && Input::get_last_key().pressed) {
		for (int i = 0; i < _mtext.size(); i++) {
			if (_mtext[i].selected) {
				_mtext[i].selected = false;
				if (i == _mtext.size() - 1) {
					_mtext[0].selected = true;
				}
				else {
					_mtext[i + 1].selected = true;
				}
				set_selector();
				_update = true;
				break;
			}
		}
	}
	if (Input::get_last_key().vk == TCODK_UP && Input::get_last_key().pressed) {
		for (int i = 0; i < _mtext.size(); i++) {
			if (_mtext[i].selected) {
				_mtext[i].selected = false;
				if (i == 0) {
					_mtext[_mtext.size() - 1].selected = true;
				}
				else {
					_mtext[i - 1].selected = true;
				}
				set_selector();
				_update = true;
				break;
			}
		}
	}
	if (Input::get_last_key().vk == TCODK_ENTER && Input::get_last_key().pressed) {
		for (int i = 0; i < _mtext.size(); i++) {
			if (_mtext[i].selected) {
				GameEvent::set_event(_mtext[i].action);
			}
		}
	}
}

void SelectionBox::position_text() {
	GUI::position_text();
	for (int i = 0; i < _mtext.size(); i++) {
		_mtext[i].x += _x;
		_mtext[i].y += _y;
	}
}

void SelectionBox::set_selector() {
	for (MText& i : _mtext) {
		if (i.selected) {
			i.color = TCODColor::yellow;
		}
		else {
			i.color = TCODColor::white;
		}
	}
}

MainMenu::MainMenu() : SelectionBox(0, 0, GameObjects::screen_width, GameObjects::screen_height, std::vector<Text>()) {
	front();
}

void MainMenu::front() {
	_text.clear();
	_mtext.clear();

	Text title = { (_width / 2) - 50, 5, 100, 50, 
		"`#######                                                `#####    `##\n"
		"`##    `##                                            `##    `##  `##\n"
		"`##    `##     `##       `##   `##  `##  `###          `##      `######   `###     `### `####\n"
		"`##`##       `##  `##  `##  `##`##  `## `#  `##          `##      `##   `##  `##  `##   `##\n"
		"`##  `##    `##    `##`##   `##`##  `##`##### `#            `##   `##  `##   `##  `##     `###\n"
		"`##    `##   `##  `##  `##  `##`##  `##`#             `##    `##  `##  `##   `##  `##       `##\n"
		"`##      `##   `##         `##   `##`## `#####          `#####     `##   `## `###`###   `## `##\n"
		"`##                       `##                                                             `##\n",
		TCODColor::red };
	_text.push_back(title);

	Text pic = { (_width / 2) - 5, 14, 20, 15,
"     #     \n"
"     #     \n"
"    ###    \n"
"   #####   \n"
"###########\n"
" ######### \n"
"  #######  \n"
"   #####   \n"
"   #####   \n"
"   ## ##   \n"
"  ##   ##  \n",
	TCODColor::white };

	_text.push_back(pic);

	MText n1 = { (_width / 2) - 5, 28, 20, 1, "New Game", TCODColor::white, 1, GameEvent::STARTUP_NEW_GAME };
	MText n2 = { (_width / 2) - 5, 30, 20, 1, "Load Game", TCODColor::white, 0, GameEvent::STARTUP_LOAD_GAME };
	MText n3 = { (_width / 2) - 5, 32, 20, 1, "Exit", TCODColor::white, 0, GameEvent::EXIT };
	
	_mtext.push_back(n1); _mtext.push_back(n2); _mtext.push_back(n3);

	set_selector();
	Input::set_mode(Input::NONE);
}

void MainMenu::draw(bool force) {
	_update = true;
	SelectionBox::draw(force);
	if (TCODSystem::getFps() != 0) {
		if (GameObjects::ticks % 60 == 0 && _state == FRONT) {
			_text[1].color = TCODColor(std::cos(GameObjects::time) * 255, std::sin(GameObjects::time) * 255, std::tan(GameObjects::time) * 255);
		}
	}
}

ESCMenu::ESCMenu() : SelectionBox(GameObjects::screen_width / 2 - 13, GameObjects::screen_height / 2 - 10, 24, 12, std::vector<Text>()) {
	MText n1 = { (_width / 2) - 5, 2, _width / 2, 1, "Main Menu", TCODColor::white, 1, GameEvent::TO_MAIN_MENU };
	MText n2 = { (_width / 2) - 5, 3, _width / 2, 1, "Exit", TCODColor::white, 0, GameEvent::EXIT };

	_mtext.push_back(n1); _mtext.push_back(n2);

	set_selector();
	Input::set_mode(Input::ESC);
}

InfoViewer::InfoViewer(Actor* aref) : GUI(0, (GameObjects::screen_height - 10), 20, 10, std::vector<Text>()) {
	Text title = { (_width / 2) - 4, 0, _width / 2, 1, "Info", TCODColor::red };
	Text c1 = { 2, 2, _width - 2, 1, "", TCODColor::white };
	Text c2 = { 2, 3, _width - 2, 1, "", TCODColor::white };
	Text c3 = { 2, 4, _width - 2, 1, "", TCODColor::white };
	Text c4 = { 2, 5, _width - 2, 1, "", TCODColor::white };
	Text c5 = { 2, 6, _width - 2, 1, "", TCODColor::white };
	Text c6 = { 2, 7, _width - 2, 1, "", TCODColor::white };
	_text.push_back(title); 
	_text.push_back(c1); _text.push_back(c2); _text.push_back(c3);
	_text.push_back(c4); _text.push_back(c5); _text.push_back(c6);
	
	_type = FILLED_BORDERED_BACKGROUND;
	_transparency = 0.9;
	_actor = aref;
}

void InfoViewer::draw(bool force) {
	if (_update || force) {
		GUI::draw(force);
		std::vector<Actor*> actors = Actor::get_actors(_actor->get_world_pos()[0], _actor->get_world_pos()[1], 0);
		
		if (actors.size() == 0) {
			for (int i = 1; i < _text.size() - 1; i++) {
				_text[i].str = "";
				return;
			}
		}

		for (int i = actors.size() - 1, j = 1;
			i >= 0 && j < _text.size();
			i--, j++) {
		
			Actor* a = actors[i];

			if (!a->is_in_fov()) {
				_text[j].str = "";
				continue;
			}

			char c = a->get_char();
			std::string name = a->get_name();

			_text[j].str = std::string(1, c) + " " + name;
		}
		for (int i = _text.size() - 1; i > actors.size(); i--) {
			_text[i].str = "";
		}
	}
}