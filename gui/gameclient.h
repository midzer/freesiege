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
#ifndef __GAMECLIENT_H
#define __GAMECLIENT_H

#include "background.h"
#include "combinaisoncollection.h"
#include "musiccollection.h"
#include "menuscreen.h"

#include <utility>
#include "boardabstract.h"

/*
 * classe pour le client
 * but : se connecter au serveur
 * faire fonctionner le handshake
 * récupérer les entrées clavier
 * les envoyer au serveur
 * récupérer l'état de notre board au minimum et des barres de vies
 * afficher le tout
 */


class GameClient {
public:

	GameClient(const SpriteCollection *spr_coll,const CombinaisonCollection *cmb_coll,const std::string &ttf_path,TextureIds ids,Background *background,MusicCollection *music_coll);
	~GameClient();
	//~ void display_game(SDL_Renderer *sdlRenderer);

	//~ void set_ai_level(MenuScreen::AILEVEL ai_level);
	void init_game();

private:
	/* sure things */
	PLAYER me; // am I right or left?
	Board* Keyboard;

	/* here? */
	void handleNetwork();
	void show_final_screen(SDL_Renderer *sdlRenderer);

	//~ std::pair<PLAYERMODE,PLAYERMODE> mode_p;
	//~ GAMEMODE mode;

	PLAYER winner;
	int p1_win;
	int p2_win;
	bool quit_game;

	std::pair<LifeBar*,LifeBar*> life_bars;
	Foreground* foreground;
	BattleField* battlefield;
	//~ std::pair<BoardAbstract*,BoardAbstract*> boards;

	const SpriteCollection *spr_coll;
	const CombinaisonCollection *cmb_coll;
	MusicCollection *music_coll;
	Background *background;

	TTF_Font *font;
	TTF_Font *font_huge;
	TTF_Font *font_tiny;

	const Sprite *text_p1_won;
	const Sprite *text_p2_won;
	const Sprite *text_key_help;
	Anim::CycleIterator skull;
	Anim::CycleIterator hand;
	TextureId score_id;
	TextureId pause_id;
};

#endif
