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
#ifndef __MENU_H
#define __MENU_H

#include "sprite.h"

#define MENU_Y 350
#define MENU_SELECTED_H 55
#define MENU_SELECTED_COLOR { 0x10, 0x10, 0x10, 0 }
#define MENU_NORMAL_H 36
#define MENU_NORMAL_COLOR { 0x00, 0x00, 0x00, 0 }
#define MENU_SPACING 40

class Menu {
public:
	typedef std::vector<std::string> Titles;
	struct TitleSprite {
		int n;
		float y;
		std::string title;
		std::string rendered_text;
		Sprite *spr_normal;
		Sprite *spr_selected;
	};

	Menu(const Titles &titles,TTF_Font *normal_font,TTF_Font *selected_font,SDL_Renderer* sdlRenderer);
	~Menu();
	void draw(SDL_Renderer* sdlRenderer);
	void next();
	void previous();
	TitleSprite* get_selected() const;
private:
	typedef std::vector<TitleSprite> TitleSprites;

	void update_text(SDL_Renderer* sdlRenderer);
	std::string ttf_path;
	TitleSprites title_sprites;
	TitleSprites::iterator selected;

	TTF_Font *normal_font;
	TTF_Font *selected_font;
};

#endif
