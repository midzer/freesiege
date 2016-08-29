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
#include "menu.h"

#include "param.h"

Menu::Menu(const Titles &titles,TTF_Font *normal_font,TTF_Font *selected_font,SDL_Renderer* sdlRenderer) :normal_font(normal_font), selected_font(selected_font) {

	int n=0; float y=MENU_Y;

	for (Titles::const_iterator title=titles.begin(); title!=titles.end(); title++) {
		TitleSprite title_sprite;
		title_sprite.n=n;
		title_sprite.y=y;
		title_sprite.title=*title;
		title_sprite.spr_normal=NULL;
		title_sprite.spr_selected=NULL;

		title_sprites.push_back(title_sprite);
		n++;
		y+=MENU_SPACING;
	}
	update_text(sdlRenderer);
	//initialisation
	selected=title_sprites.begin();
}

Menu::~Menu() {
	for (TitleSprites::iterator iter=title_sprites.begin(); iter!=title_sprites.end(); iter++) {
		delete iter->spr_normal;
		delete iter->spr_selected;
	}
}

void Menu::update_text(SDL_Renderer* sdlRenderer)
{
	SDL_Color normal_color=MENU_NORMAL_COLOR;
	SDL_Color selected_color=MENU_SELECTED_COLOR;
	SDL_Surface *normal_surf;
	SDL_Surface *selected_surf;
	for (TitleSprites::iterator iter=title_sprites.begin(); iter!=title_sprites.end(); iter++) {
		if (iter->title != iter->rendered_text) {
			delete iter->spr_normal;
			delete iter->spr_selected;
			normal_surf=TTF_RenderText_Solid(normal_font,iter->title.c_str(),normal_color);
			selected_surf=TTF_RenderText_Solid(selected_font,iter->title.c_str(),selected_color);

			iter->spr_normal	= new Sprite(sdlRenderer,normal_surf);
			iter->spr_selected	= new Sprite(sdlRenderer,selected_surf);

			SDL_FreeSurface(normal_surf);
			SDL_FreeSurface(selected_surf);
			iter->rendered_text = iter->title;
		}
	}

}

void Menu::draw(SDL_Renderer* sdlRenderer) {
	update_text(sdlRenderer);
	for (TitleSprites::const_iterator iter=title_sprites.begin(); iter!=title_sprites.end(); iter++) {
		Sprite *spr_current;
		if (iter!=selected) {
			spr_current=iter->spr_normal;
		} else {
			spr_current=iter->spr_selected;
			fill_rect(sdlRenderer, (SCREEN_W-spr_current->w)/2-5,iter->y-spr_current->h/2,spr_current->w+10,spr_current->h-5,1,1,1,0.7);
		}
		spr_current->draw((SCREEN_W-spr_current->w)/2,iter->y-spr_current->h/2);
	}
}

void Menu::next() {
	selected++;
	if (selected==title_sprites.end()) selected=title_sprites.begin();
}

void Menu::previous() {
	if (selected==title_sprites.begin()) selected=title_sprites.end();
	selected--;
}

Menu::TitleSprite* Menu::get_selected() const {
	return &*selected; //FIXME
}
