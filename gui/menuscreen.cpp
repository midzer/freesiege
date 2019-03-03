//	This file is part of freesiege program <http://freesiege.sourceforge.net>.
//	Copyright (C) 2007 Pierre Gueth <pierregueth@users.sourceforge.net>
//	                   Joel Schaerer <joelschaerer@users.sourceforge.net>
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "menuscreen.h"

#include "param.h"
#include "options.h"
#include "gamepad.h"

#define MENUSCREEN_BASE_Y 0
#define MENUSCREEN_BASE_X 0
#define MENUSCREEN_FREE_FRAME_COUNT 125
#define MENUSCREEN_SIEGE_FRAME_COUNT 150
#define MENUSCREEN_DELAY 50

#define D_SHIFT 0.008;
#define CASTLE_SHIFT 150
#define CASTLE_BIAS 45
#define HILL_SHIFT 30
#define HILL_BIAS 50

using namespace std;

MenuScreen::MenuScreen(const SpriteCollection *spr_coll,const string &ttf_path,SDL_Renderer *sdlRenderer) {
	this->spr_coll=spr_coll;
	this->ttf_path=ttf_path;
	this->ids=ids;

	normal_font		= TTF_OpenFont(ttf_path.c_str(),MENU_NORMAL_H);
	selected_font	= TTF_OpenFont(ttf_path.c_str(),MENU_SELECTED_H);
	if (!normal_font || !selected_font) {
		std::cerr<<"font "<<ttf_path<<" creation failed..."<<std::endl;
		throw "font "+ttf_path+" creation failed...";
	}

	//main menu
	Menu::Titles titles;
	//titles.push_back("human versus cpu");
	titles.push_back("Human versus human");
	titles.push_back("Survival mode");
	titles.push_back("Demo");
	titles.push_back("Patterns");
	titles.push_back("Options");
	titles.push_back("Quit");
	main_menu=new Menu(titles,normal_font,selected_font,sdlRenderer);

	//option menu
	titles.clear();
    ai_level=NORMAL;
	titles.push_back("AI Level: Knight");
	titles.push_back("Keys");
	titles.push_back(Options::soundOn()?"Sound: on":"Sound: off");
	titles.push_back(Options::fullscreenOn()?"Fullscreen":"Windowed");
	titles.push_back("Return");
	option_menu=new Menu(titles,normal_font,selected_font,sdlRenderer);

	key_menu=NULL;
	refresh_key_menu(sdlRenderer);

	menus.push(main_menu);
}

void MenuScreen::refresh_key_menu(SDL_Renderer *sdlRenderer) {
	Menu::Titles titles;
	for(int i=0;i<Keys::NBKEYS;i++) {
		titles.push_back(Keys::name(Keys::KEY(i))+" PI:"+string(SDL_GetKeyName(SDL_GetKeyFromScancode(Options::player1keys.keys[i])))+" PII:"+string(SDL_GetKeyName(SDL_GetKeyFromScancode(Options::player2keys.keys[i]))));
	}
	if(key_menu)
		delete key_menu;
	key_menu=new Menu(titles,normal_font,selected_font,sdlRenderer);
}

MenuScreen::~MenuScreen() {
	delete option_menu;
	delete main_menu;
	delete key_menu;

 	if(TTF_WasInit()) {
 		if(normal_font)
 			TTF_CloseFont(normal_font);
		normal_font = NULL;
 		if(selected_font)
 			TTF_CloseFont(selected_font);
		selected_font = NULL;
	}
}

Sprite* MenuScreen::load_message(SDL_Renderer* sdlRenderer, string msg) {
	SDL_Color normal_color=MENU_NORMAL_COLOR;
	SDL_Surface *normal_surf;
	normal_surf=TTF_RenderText_Solid(normal_font,msg.c_str(),normal_color);
	return new Sprite(sdlRenderer,normal_surf);
}

bool MenuScreen::display_menu(SDL_Renderer *sdlRenderer,SDL_Window *sdlWindow,SELECTION &selection) {
	Uint32 ticks=SDL_GetTicks();
	Uint32 frame_count=0;

	//logo sprites
	const Sprite *logo_free=spr_coll->get_sprite("logo_free");
	const Sprite *logo_siege=spr_coll->get_sprite("logo_siege");
	//~ const Sprite *logo_sword=spr_coll->get_sprite("logo_sword");
	//background sprites
	const Sprite *back_hill=spr_coll->get_sprite("title_foreground");
	const Sprite *back_castle=spr_coll->get_sprite("title_castle");
	const Sprite *back_sky=spr_coll->get_sprite("title_sky");

	unique_ptr<Sprite> msgSprite = nullptr;

	SDL_Event event;
	float shift=1.0;
	bool waiting_key=false;
	int current_player=0;

	while (true) {
		Menu *current_menu=menus.top();

		clear_screen(sdlRenderer);
		back_sky->draw(0,0);

		back_castle->draw(0,CASTLE_BIAS+CASTLE_SHIFT*shift);
		back_hill->draw(0,HILL_BIAS+HILL_SHIFT*shift);
		shift-=D_SHIFT;
		if (shift<0) shift=0;

		draw_fadein(logo_free,MENUSCREEN_BASE_X,MENUSCREEN_BASE_Y,frame_count,MENUSCREEN_FREE_FRAME_COUNT,MENUSCREEN_DELAY, 0);
		draw_fadein(logo_siege,SCREEN_W-logo_siege->w-MENUSCREEN_BASE_X,MENUSCREEN_BASE_Y,frame_count,MENUSCREEN_SIEGE_FRAME_COUNT,MENUSCREEN_DELAY, 1);

		if(waiting_key && msgSprite) {
			msgSprite->draw((SCREEN_W-msgSprite->w)/2,MENU_Y);
		} else {
			current_menu->draw(sdlRenderer);
		}
		SDL_RenderPresent(sdlRenderer);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					if(waiting_key) {
						if(current_player==0) {
							Options::player1keys.keys[current_menu->get_selected()->n] = event.key.keysym.scancode;
							current_player++;
							msgSprite.reset(load_message(sdlRenderer, "Key "+Keys::name(Keys::KEY(current_menu->get_selected()->n))+" for player II?"));
						} else {
							Options::player2keys.keys[current_menu->get_selected()->n] = event.key.keysym.scancode;
							current_player=0;
							waiting_key=false;
							refresh_key_menu(sdlRenderer);
							menus.pop();
							menus.push(key_menu);
						}
					} else {
						switch (event.key.keysym.scancode) {
							case SDL_SCANCODE_ESCAPE:
								if (!menus.empty()) menus.pop();
								if (menus.empty()) {
									selection=QUIT;
									return true;
								}
								break;
							case SDL_SCANCODE_RETURN:
							case SDL_SCANCODE_SPACE:
								if (handle_action(sdlRenderer,sdlWindow, current_menu, selection, waiting_key, msgSprite)) {
									return (selection == QUIT);
								}
								break;
							case SDL_SCANCODE_UP:
								current_menu->previous();
								break;
							case SDL_SCANCODE_DOWN:
								current_menu->next();
								break;
							default:
								break;
						}
					}
					break;
				case SDL_CONTROLLERDEVICEADDED:
				case SDL_CONTROLLERDEVICEREMOVED:
				case SDL_CONTROLLERAXISMOTION:
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
					int player;
					switch(Gamepad::handleEvent(event, player)) {
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							current_menu->previous();
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
							current_menu->next();
							break;
						case SDL_CONTROLLER_BUTTON_A:
							if (handle_action(sdlRenderer,sdlWindow, current_menu, selection, waiting_key, msgSprite)) {
								return (selection == QUIT);
							}
							break;
						case SDL_CONTROLLER_BUTTON_B:
							if (!menus.empty()) menus.pop();
							if (menus.empty()) {
								selection=QUIT;
								return true;
							}
							break;
						//~ todo: trouver comment appeler le même code que pour les interactions clavier (à priori suffit de passer en fonctions)
					}
					break;
				case SDL_QUIT:
					return true;
					break;
				default:
					break;
			}
		}

		while (ticks>(SDL_GetTicks()-1000/FPS)) SDL_Delay(3);
		ticks=SDL_GetTicks();
		frame_count++;
	}

	return true;
}

bool MenuScreen::handle_action (SDL_Renderer *sdlRenderer, SDL_Window *sdlWindow, Menu *current_menu, SELECTION &selection, bool &waiting_key, unique_ptr<Sprite> &msgSprite)
{
	if (current_menu==main_menu) {
		switch (current_menu->get_selected()->n) {
			case 0://2 players versus
				selection=TWO_PLAYERS;
				return true;
				break;
			case 1://training mode
				selection=TRAINING;
				return true;
				break;
			case 2://combinaisons
				selection=EXHIBITION;
				return true;
				break;
			case 3://combinaisons
				selection=COMBINAISONS;
				return true;
				break;
			case 4://options
				menus.push(option_menu);
				break;
			case 5://quit
				selection=QUIT;
				return true;
				break;
			default:
				break;
		}
	} else if (current_menu==option_menu) {
		switch (current_menu->get_selected()->n) {
			case 0://ai level
				switch (ai_level) {
					case EASY:
						ai_level=NORMAL;
						current_menu->get_selected()->title="AI Level: Knight";
						break;
					case NORMAL:
						ai_level=HARD;
						current_menu->get_selected()->title="AI Level: Galaad the Righteous";
						break;
					case HARD:
						ai_level=EXTREME;
						current_menu->get_selected()->title="AI Level: Morgoth the Cruel";
						break;
					case EXTREME:
						ai_level=EASY;
						current_menu->get_selected()->title="AI Level: Farmer";
						break;
				}
				break;
			case 1://keys
				menus.push(key_menu);
				break;
			case 2://Sound
				if (Options::soundOn())
				{
					Mix_PauseMusic();
					Options::setSound(false);
					current_menu->get_selected()->title="Sound: off";
				} else {
					Mix_ResumeMusic();
					Options::setSound(true);
					current_menu->get_selected()->title="Sound: on";
				}
				break;
			case 3://Fullscreen
				if (Options::fullscreenOn())
				{
					SDL_SetWindowFullscreen(sdlWindow, 0);
					Options::setFullscreen(false);
					current_menu->get_selected()->title="Windowed";
				} else {
					SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
					Options::setFullscreen(true);
					current_menu->get_selected()->title="Fullscreen";
				}
				break;
			case 4://return
				menus.pop();
				break;
			default:
				break;
		}
	} else if (current_menu==key_menu) {
		waiting_key = true;
		msgSprite.reset(load_message(sdlRenderer, "Key "+Keys::name(Keys::KEY(current_menu->get_selected()->n))+" for player I ?"));
	}
	return false;
}
