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
#ifndef __SPRITE_H
#define __SPRITE_H

#include "config.h"

class Sprite {
public:
	Sprite(SDL_Renderer* sdlRenderer, const std::string &filename);
	Sprite(SDL_Renderer* sdlRenderer, SDL_Surface *surf);
	~Sprite();
	void draw(float x, float y, float alpha=1.0, float angle=0.0, float factor=1.0) const;
	void draw_flip_h(float x, float y, float alpha=1.0, float angle=0.0, float factor=1.0) const;
	int w,h;
private:
	float factor_w,factor_h;
	unsigned int texture_id;
	SDL_Texture *sdlTexture = NULL;
	SDL_Renderer *sdlRenderer = NULL;
};

SDL_Surface *create_surface(int width,int height);

void clear_screen(SDL_Renderer* sdlRenderer);
void fill_rect(SDL_Renderer* sdlRenderer,float x,float y,float w,float h,float r,float g,float b,float alpha=1.0);
void draw_fadein(const Sprite *sprite,float x,float y,int count,int end,int delay,int mode);
void draw_fadeout(const Sprite *sprite,float x,float y,int count,int end,int delay,float n_turn=0.0,float angle=0.0);

#endif
