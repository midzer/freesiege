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
#include "background.h"
#include "battlefield.h"
#include "utils.h"
#include "param.h"

SDL_mutex *counter_reset_mutex;
int valid_fps;
int current_fps;

Uint32 counter_reset_callback(Uint32 interval,void *param) {
	SDL_LockMutex(counter_reset_mutex);
		valid_fps=current_fps;
		current_fps=0;
		std::cout<<"FPS "<<valid_fps<<std::endl;
	SDL_UnlockMutex(counter_reset_mutex);

	return interval;
}

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
int main(int argc, char* argv[]) {
	init_random_gen();
	//SDL init;
	std::cout<<"init sdl"<<std::endl;
	if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER)==-1) {
		std::cerr<<"sdl init failed..."<<SDL_GetError()<<std::endl;
		return 1;
	}

	IMG_Init(IMG_INIT_PNG);

	std::cout<<"init video"<<std::endl;
	SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, SDL_WINDOW_RESIZABLE, &sdlWindow, &sdlRenderer);

	if (!sdlWindow || !sdlRenderer) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_RenderSetLogicalSize(sdlRenderer, SCREEN_W, SCREEN_H);
	SDL_SetWindowTitle(sdlWindow, "FreeSiege Fight test");

	//object init
	std::string base_dir=get_base_dir();
	SpriteCollection spr_coll(base_dir+"sprites.cfg",base_dir+"anims.cfg",base_dir,sdlRenderer);
	std::cout<<spr_coll<<std::endl;

	Background background(&spr_coll);
	Foreground foreground(&spr_coll);
	LifeBar life_bar1(&spr_coll,PLAYER_1);
	LifeBar life_bar2(&spr_coll,PLAYER_2);
	BattleField battlefield(&spr_coll,&life_bar1,&life_bar2,&foreground);

	//mutex timer init
	counter_reset_mutex=SDL_CreateMutex();
	SDL_TimerID counter_reset_id=SDL_AddTimer(1000,counter_reset_callback,NULL);

	bool quit=false;
	SDL_Event event;
	Uint32 ticks=SDL_GetTicks();
	while (!quit) {
		//draw
		clear_screen(sdlRenderer);
		background.draw();
		life_bar1.draw(sdlRenderer);
		life_bar2.draw(sdlRenderer);
		battlefield.refresh();
		battlefield.draw();
		foreground.draw();
		SDL_RenderPresent(sdlRenderer);
		//logic

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode!=SDL_SCANCODE_ESCAPE) {
					switch (event.key.keysym.scancode) {
					//unit spawn keys
					case SDL_SCANCODE_Q:
						battlefield.spawn(SOLDIER,PLAYER_1);
						break;
					case SDL_SCANCODE_A:
						battlefield.spawn(SOLDIER,PLAYER_2);
						break;
					case SDL_SCANCODE_W:
						battlefield.spawn(DRUID,PLAYER_1);
						break;
					case SDL_SCANCODE_S:
						battlefield.spawn(DRUID,PLAYER_2);
						break;
					case SDL_SCANCODE_E:
						battlefield.spawn(KNIGHT,PLAYER_1);
						break;
					case SDL_SCANCODE_D:
						battlefield.spawn(KNIGHT,PLAYER_2);
						break;
					case SDL_SCANCODE_R:
						battlefield.spawn(GOLEM,PLAYER_1);
						break;
					case SDL_SCANCODE_F:
						battlefield.spawn(GOLEM,PLAYER_2);
						break;
					case SDL_SCANCODE_T:
						battlefield.spawn(PLANT,PLAYER_1);
						break;
					case SDL_SCANCODE_G:
						battlefield.spawn(PLANT,PLAYER_2);
						break;
					case SDL_SCANCODE_Y:
						battlefield.spawn(DRAGON,PLAYER_1);
						break;
					case SDL_SCANCODE_H:
						battlefield.spawn(DRAGON,PLAYER_2);
						break;
					case SDL_SCANCODE_U:
						battlefield.spawn(FLOWER,PLAYER_2);
						break;
					case SDL_SCANCODE_J:
						battlefield.spawn(FLOWER,PLAYER_1);
						break;
					case SDL_SCANCODE_O:
						battlefield.spawn(VETERAN,PLAYER_2);
						break;
					case SDL_SCANCODE_L:
						battlefield.spawn(VETERAN,PLAYER_1);
						break;
					default:
						break;
					}
					break;
				}
			case SDL_QUIT:
				quit=true;
				break;
			default:
				break;
			}
		}

		while (ticks>(SDL_GetTicks()-1000/FPS)) SDL_Delay(3);
		ticks=SDL_GetTicks();

		current_fps++;
	}

	SDL_RemoveTimer(counter_reset_id);
	SDL_DestroyMutex(counter_reset_mutex);

	IMG_Quit();

	SDL_Quit();
	return 0;
}
