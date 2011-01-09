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
#include "gamescreen.h"

#include "board.h"
#include "boardsurvivor.h"
#include "boardnetwork.h"
#include "utils.h"
#include "param.h"
#include "options.h"

#define FONT_COLOR { 0x77, 0xd1, 0x00, 0 }
#define SURVIVAL_TIME 3000

using namespace std;

GameScreen::GameScreen(const SpriteCollection *spr_coll,const CombinaisonCollection *cmb_coll,const std::string &ttf_path,TextureIds ids,Background *background,MusicCollection *music_coll,GAMEMODE mode,PLAYERMODE modep1,PLAYERMODE modep2) {
	font=TTF_OpenFont(ttf_path.c_str(),80);
	font_huge=TTF_OpenFont(ttf_path.c_str(),120);
	font_tiny=TTF_OpenFont(ttf_path.c_str(),40);
	if (!font || !font_huge || !font_tiny) {
		std::cerr<<"font "<<ttf_path<<" creation failed..."<<std::endl;
		return;
	}

	SDL_Color color=FONT_COLOR;
	SDL_Surface *text_p1_won_surf=TTF_RenderText_Solid(font_huge,"PL I WON!!!", color);
	SDL_Surface *text_p2_won_surf=TTF_RenderText_Solid(font_huge,"PL II WON!!!", color);
	SDL_Surface *text_key_help_surf=TTF_RenderText_Solid(font_tiny,"Space to continue ... Esc to quit", color);
	text_p1_won=new Sprite(text_p1_won_surf,ids[0]);
	text_p2_won=new Sprite(text_p2_won_surf,ids[1]);
	text_key_help=new Sprite(text_key_help_surf,ids[2]);
	skull=spr_coll->get_anim_cycle_iterator("skull",0.1);
	hand=spr_coll->get_anim_cycle_iterator("hand",0.1);
	SDL_FreeSurface(text_p1_won_surf);
	SDL_FreeSurface(text_p2_won_surf);
	SDL_FreeSurface(text_key_help_surf);

	score_id=ids[3];
	pause_id=ids[4];
	perfect_id=ids[5];
	go_id=ids[6];
	ko_id=ids[7];

	this->spr_coll=spr_coll;
	this->cmb_coll=cmb_coll;
	this->music_coll=music_coll;
	this->background=background;
	
	this->mode_p.first = (modep1!=AUTO?modep1:HUMAN);
	this->mode_p.second = (modep2!=AUTO?modep2:(mode==SURVIVOR?AI:HUMAN));
	this->mode = mode;
	
	life_bars.first = NULL;
	life_bars.second = NULL;
	foreground = NULL;
	battlefield = NULL;
	boards.first = NULL;
	boards.second = NULL;
	
	init_game();
}

GameScreen::~GameScreen() {
	delete text_p1_won;
	delete text_p2_won;
	delete text_key_help;
	
	delete life_bars.first;
	delete life_bars.second;
	delete foreground;
	delete battlefield;
	delete boards.first;
	delete boards.second;
	
 	if(TTF_WasInit()) {
 		if(font)
 			TTF_CloseFont(font);
 			font = NULL;
 		if(font_huge)
 			TTF_CloseFont(font_huge);
 			font_huge = NULL;
 		if(font_tiny)
 			TTF_CloseFont(font_tiny);
 			font_tiny = NULL;
	}

}

void GameScreen::init_game() {
	winner=PLAYER_NEUTRAL;
	p1_win=0;
	p2_win=0;
	quit_game=false;
	
	delete boards.first;
	delete boards.second;
	
	switch(mode_p.first) {
		default:
		case HUMAN:
			boards.first = new Board(spr_coll,cmb_coll,battlefield,PLAYER_1);
			break;
		case AI:
			boards.first = new BoardSurvivor(spr_coll,cmb_coll,battlefield,PLAYER_1,base_speed);
			break;
		case REMOTE:
			boards.first = new BoardNetwork(spr_coll,cmb_coll,battlefield,PLAYER_1);
			break;
		case SERVER:
			boards.first = NULL;
			break;
	}
	switch(mode_p.second) {
		default:
		case HUMAN:
			boards.second = new Board(spr_coll,cmb_coll,battlefield,PLAYER_2);
			break;
		case AI:
			boards.second = new BoardSurvivor(spr_coll,cmb_coll,battlefield,PLAYER_2,base_speed);
			break;
		case REMOTE:
			boards.second = new BoardNetwork(spr_coll,cmb_coll,battlefield,PLAYER_1);
			break;
		case SERVER:
			boards.second = NULL;
			break;
	}
}

void GameScreen::display_game(SDL_Surface *screen) {

	bool paused=false;
	
	SDL_Event event;
	SDL_Color color=FONT_COLOR;
	Uint32 ticks=SDL_GetTicks();
	
	SDL_Surface *pause_surf=TTF_RenderText_Solid(font,"GAME PAUSED",color);
	Sprite pause_sprite(pause_surf,pause_id);
	SDL_FreeSurface(pause_surf);
	
	init_game();

	while (!quit_game) {
		//game object init
        int play_ticks=1;
		bool quit=false;
		delete life_bars.first;
		delete life_bars.second;
		delete foreground;
		delete battlefield;
		
		life_bars.first = new LifeBar(spr_coll,PLAYER_1);
		life_bars.second = new LifeBar(spr_coll,PLAYER_2);
		foreground = new Foreground(spr_coll);
		battlefield = new BattleField(spr_coll,life_bars.first,life_bars.second,foreground);
				
		boards.first->newGame(battlefield);
		boards.second->newGame(battlefield);

		//main loop
		while (!quit && !quit_game) {
			//draw
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			background->draw();
			life_bars.first->draw();
			life_bars.second->draw();
			if(!paused && (mode!=CLIENT))
				battlefield->refresh();
			battlefield->draw();
			boards.first->draw();
			boards.second->draw();
			foreground->draw();
			if(paused) {
				fill_rect_opengl(0,0,SCREEN_W,SCREEN_H,0,0,0,0.7);
				pause_sprite.draw((SCREEN_W-pause_sprite.w)/2,(SCREEN_H-pause_sprite.h)/2);
			}
			SDL_GL_SwapBuffers();	
			SDL_Flip(screen);
			
			if(!paused && mode!=CLIENT) {
				//logic
				boards.first->logic(foreground->p1_flower);
				if(mode!=SURVIVOR||(play_ticks < SURVIVAL_TIME))
					boards.second->logic(foreground->p2_flower);
			}
			
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym==SDLK_ESCAPE) quit_game=true;
#ifdef DEBUG_MODE
					else if (event.key.keysym.sym==SDLK_t) life_bars.second->damage(100); //DEBUG
					else if (event.key.keysym.sym==SDLK_y) life_bars.first->damage(100);
#endif
					else if (event.key.keysym.sym==Options::pause_key) paused=!paused;
					break;
				case SDL_QUIT:			
					quit_game=true;
					break;
				default:
					break;
				}
			}

            if( (mode==SURVIVOR) &&
				!paused &&
				(play_ticks > SURVIVAL_TIME) &&
				(battlefield->get_nonplant_unit_count(PLAYER_2) == 0)
				) {
				winner=PLAYER_1;
				quit=true;
			}
			
			if(mode==CLIENT)
				handleNetwork();
			else {
				if (life_bars.first->get_life()<=0) {
					winner=PLAYER_2;
					quit=true;
				} else if (life_bars.second->get_life()<=0) {
					winner=PLAYER_1;
					quit=true;
				}
			}

			while (ticks>(SDL_GetTicks()-1000/FPS)) SDL_Delay(3);
			ticks=SDL_GetTicks();
			if(!paused)
				play_ticks++;
			
			if(!Mix_PlayingMusic()) {
				music_coll->play_random_music();
			}
		}

		show_final_screen(screen);
	}
}
		
void GameScreen::show_final_screen(SDL_Surface *screen) {
	SDL_Color color=FONT_COLOR;
	int ticks = SDL_GetTicks();
	
	//final screen
	
	const Sprite *winning_message=NULL;
	switch (winner) {
	case PLAYER_1:
		p1_win++;
		winning_message=text_p1_won;
        boards.first->hasWon();
        boards.second->hasLost();
		break;
	case PLAYER_2:
		p2_win++;
		winning_message=text_p2_won;
        boards.first->hasLost();
        boards.second->hasWon();
		break;
	default:
		break;
	}
	
	Sprite *go_sprite = NULL,*ko_sprite = NULL,*score_sprite = NULL;
	bool ko;
	if(mode==SURVIVOR) {
		//render score
		SDL_Surface *score_surf=TTF_RenderText_Solid(font,("Level " + number_as_roman(boards.second->getLevel())+" cleared!!!").c_str(),color);
		score_sprite = new Sprite(score_surf,score_id);
		SDL_FreeSurface(score_surf);
		SDL_Surface *go_surf=TTF_RenderText_Solid(font,"GAME OVER!",color);
		go_sprite = new Sprite(go_surf,go_id);
		SDL_FreeSurface(go_surf);
		SDL_Surface *ko_surf=TTF_RenderText_Solid(font,"KO!",color);
		ko_sprite = new Sprite(ko_surf,ko_id);
		SDL_FreeSurface(ko_surf);
		ko=(life_bars.second->get_life() == 0);
	} else if(mode==VERSUS) {
		//render score
		SDL_Surface *score_surf=TTF_RenderText_Solid(font,(number_as_roman(p1_win)+" : "+number_as_roman(p2_win)).c_str(),color);
		score_sprite = new Sprite(score_surf,score_id);
		SDL_FreeSurface(score_surf);
	}
	SDL_Surface *perfect_surf=TTF_RenderText_Solid(font,"PERFECT!",color);
	Sprite perfect_sprite(perfect_surf,perfect_id);
	SDL_FreeSurface(perfect_surf);

	bool quit=false;
	bool perfect=((life_bars.first->get_life() == LIFE_FACTOR)||(life_bars.second->get_life() == LIFE_FACTOR));
	while (!quit && !quit_game) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//background
		background->draw();
		life_bars.first->draw();
		life_bars.second->draw();
		battlefield->refresh();
		battlefield->draw();
		boards.first->draw();
		boards.second->draw();
		foreground->draw();
		
		//overlay
		fill_rect_opengl(0,0,SCREEN_W,SCREEN_H,0,0,0,0.7);
		text_key_help->draw((SCREEN_W-text_key_help->w)/2,SCREEN_H-50-text_key_help->h);
		if(mode==VERSUS) {
			winning_message->draw((SCREEN_W-winning_message->w)/2,50);
			score_sprite->draw((SCREEN_W-score_sprite->w)/2,(SCREEN_H-score_sprite->h)/2);
		}

		const Sprite* current_skull=skull.get_next_bitmap();
		const Sprite* current_hand=hand.get_next_bitmap();
		if(mode==VERSUS) {
			if (winner==PLAYER_2) {
				current_skull->draw(SCREEN_W/2-score_sprite->w/2-current_skull->w,(SCREEN_H-current_skull->h)/2+10);
				current_hand->draw(SCREEN_W/2+score_sprite->w/2,(SCREEN_H-current_hand->h)/2+10);
			} else {
				current_hand->draw(SCREEN_W/2-score_sprite->w/2-current_hand->w,(SCREEN_H-current_hand->h)/2+10);
				current_skull->draw(SCREEN_W/2+score_sprite->w/2,(SCREEN_H-current_skull->h)/2+10);
			}
		} else if (mode == SURVIVOR) {
			if (winner==PLAYER_2) {
                go_sprite->draw((SCREEN_W-go_sprite->w)/2,50);
				current_skull->draw(SCREEN_W/2-current_skull->w/2,(SCREEN_H-current_skull->h)/2);
			} else {
                if (perfect)
                  {
                    if (ko) {
                        current_hand->draw(SCREEN_W/2-current_hand->w/2-160,(SCREEN_H-current_hand->h)/2);
                        current_hand->draw(SCREEN_W/2-current_hand->w/2,(SCREEN_H-current_hand->h)/2);
                        current_hand->draw(SCREEN_W/2-current_hand->w/2+160,(SCREEN_H-current_hand->h)/2);
                        ko_sprite->draw((SCREEN_W-ko_sprite->w)/2,60+score_sprite->h+perfect_sprite.h);
                    }
                    else
                      {
                        current_hand->draw(SCREEN_W/2-current_hand->w-perfect_sprite.w/2,(SCREEN_H-current_hand->h)/2);
                        current_hand->draw(SCREEN_W/2+perfect_sprite.w/2,(SCREEN_H-current_hand->h)/2);
                      }
                    perfect_sprite.draw((SCREEN_W-perfect_sprite.w)/2,60+score_sprite->h);
                  }
                else if (ko)
                  {
                    current_hand->draw(SCREEN_W/2-current_hand->w-ko_sprite->w/2,(SCREEN_H-current_hand->h)/2);
                    current_hand->draw(SCREEN_W/2+ko_sprite->w/2,(SCREEN_H-current_hand->h)/2);
                    ko_sprite->draw((SCREEN_W-ko_sprite->w)/2,60+score_sprite->h);
                  }
                else
                    current_hand->draw(SCREEN_W/2-current_hand->w/2,(SCREEN_H-current_hand->h)/2);
                score_sprite->draw((SCREEN_W-score_sprite->w)/2,50);
			}
		}

		SDL_GL_SwapBuffers();
		SDL_Flip(screen);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_ESCAPE) quit_game=true; //quit game
				if (event.key.keysym.sym==SDLK_SPACE) quit=true;
				break;
			case SDL_QUIT:			
				quit_game=true;
				break;
			default:
				break;
			}
		}

		SDL_Delay(10);
		while (ticks>(SDL_GetTicks()-1000/FPS))
			SDL_Delay(3);
		ticks=SDL_GetTicks();
	}
    
    delete go_sprite,ko_sprite,score_sprite;
    
    if ( (winner==PLAYER_2) && (mode == SURVIVOR) ) quit_game=true;
}

void GameScreen::set_ai_level(MenuScreen::AILEVEL ai_level)
{
    switch(ai_level)
      {
        case 0:
            base_speed=500;
            break;
        case 1:
            base_speed=150;
            break;
        case 2:
            base_speed=90;
            break;
        case 3:
            base_speed=60;
            break;
      }
}

void GameScreen::handleNetwork() {
	#warning non implémenté
}
