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

#include "utils.h"
#include "param.h"
#include "gamescreen.h"
#include "menuscreen.h"
#include "combinaisonscreen.h"
#include "musiccollection.h"
#include "options.h"


SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;

bool display_usage=false;
void usage() {
	if (display_usage) return;
	std::cout<<"freesiege [--windowed] [--no-sound] [--help]"<<std::endl;
	std::cout<<"  --windowed: launch the game using a 800x600 window"<<std::endl;
	std::cout<<"  --no-sound: turn off sound"<<std::endl;
	std::cout<<"  --help: display this help"<<std::endl;
	display_usage=true;
	exit(0);
}

int main(int argc, char* argv[]) {
	//parameters parsing
	Options::load();
	std::string param;
	for (int k=1; k<argc; k++) {
		param=std::string(argv[k]);
		if (param=="--no-sound") ;
		else if (param=="--windowed") ;
		else if (param=="--help") usage();
		else {
			std::cerr<<"unknown parameter: "<<argv[k]<<std::endl;
			usage();
		}
	}
	Options::handleArguments(argc,argv);

	//random init
	init_random_gen();

	//SDL init;
	if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER)==-1) {
		std::cerr<<"sdl init failed..."<<SDL_GetError()<<std::endl;
		return 1;
	}

	IMG_Init(IMG_INIT_PNG);

	int flags = SDL_WINDOW_RESIZABLE;
	if (Options::fullscreenOn())
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, flags, &sdlWindow, &sdlRenderer);

	if (!sdlWindow || !sdlRenderer) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_RenderSetLogicalSize(sdlRenderer, SCREEN_W, SCREEN_H);
	SDL_SetWindowIcon(sdlWindow, IMG_Load(FREESIEGE_ICON_PATH));
	SDL_SetWindowTitle(sdlWindow, "FreeSiege");
	SDL_ShowCursor(SDL_DISABLE);

	if (TTF_Init()==-1) {
		std::cerr<<"ttf init failed..."<<TTF_GetError()<<std::endl;
		return 1;
	}

	if (Options::soundOn()) {
		if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1) {
			std::cerr<<"mixer init failed..."<<Mix_GetError()<<std::endl;
			Options::setSoundSession(false);
		}
	}

	std::string base_dir=get_base_dir();
	//object init
	CombinaisonCollection cmb_coll(base_dir+"combi.cfg");
	std::cout<<cmb_coll<<std::endl;
	MusicCollection music_coll(base_dir+"musics.cfg",base_dir);
	std::cout<<music_coll<<std::endl;
	SpriteCollection spr_coll(base_dir+"sprites.cfg",base_dir+"anims.cfg",base_dir,sdlRenderer);
	std::cout<<spr_coll<<std::endl;

	const std::string font_name="Swift.ttf";
	Background background(&spr_coll);
	GameScreen training_screen(&spr_coll,&cmb_coll,base_dir+font_name,sdlRenderer,&background,&music_coll,GameScreen::SURVIVOR);
	GameScreen exhibition_screen(&spr_coll,&cmb_coll,base_dir+font_name,sdlRenderer,&background,&music_coll,GameScreen::VERSUS,GameScreen::AI,GameScreen::AI);
	GameScreen game_screen(&spr_coll,&cmb_coll,base_dir+font_name,sdlRenderer,&background,&music_coll,GameScreen::VERSUS);
	CombinaisonScreen combi_screen(&spr_coll,&cmb_coll,base_dir+font_name,sdlRenderer);
	MenuScreen menu_screen(&spr_coll,base_dir+font_name,sdlRenderer);

	//main loop
	music_coll.play_music("intro");
	MenuScreen::SELECTION selection=MenuScreen::QUIT;
	while (!menu_screen.display_menu(sdlRenderer, sdlWindow, selection)) {
		if (selection==MenuScreen::TWO_PLAYERS) {
			music_coll.play_random_music();
			game_screen.display_game(sdlRenderer);
			music_coll.play_music("intro");
		} else if (selection==MenuScreen::COMBINAISONS) {
			music_coll.play_music("combi");
			combi_screen.display_combinaisons(sdlRenderer);
			music_coll.play_music("intro");
		} else if (selection==MenuScreen::EXHIBITION) {
			music_coll.play_random_music();
			exhibition_screen.set_ai_level(menu_screen.ai_level);
			exhibition_screen.display_game(sdlRenderer);
			music_coll.play_music("intro");
		} else if (selection==MenuScreen::TRAINING) {
			music_coll.play_random_music();
			training_screen.set_ai_level(menu_screen.ai_level);
			training_screen.display_game(sdlRenderer);
			music_coll.play_music("intro");
		} else std::cerr<<"unknown selection "<<selection<<std::endl;
	}


	Options::save();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
