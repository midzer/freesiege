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
#include "sprite.h"
#include <SDL_image.h>

//helper function
int nearest_power(int number) {
	int i;
	for (i=1;i<number;i=i<<1);
	return i;
}

Sprite::Sprite(SDL_Renderer* sdlRenderer, const std::string &filename) {
	this->sdlRenderer = sdlRenderer;
	SDL_Surface *load_surf=IMG_Load(filename.c_str());
	SDL_SetColorKey(load_surf,SDL_TRUE,SDL_MapRGB(load_surf->format,0xff,0x00,0xff));
	sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, load_surf);
	SDL_FreeSurface(load_surf);
	if (!sdlTexture) {
		throw "fail";
	}
	SDL_QueryTexture(sdlTexture, NULL, NULL, &w, &h);
}

Sprite::Sprite(SDL_Renderer* sdlRenderer, SDL_Surface *load_surf) {
	this->sdlRenderer = sdlRenderer;
	sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, load_surf);
	if (!sdlTexture) {
		throw "fail";
	}
	SDL_QueryTexture(sdlTexture, NULL, NULL, &w, &h);
}

Sprite::~Sprite() {
	SDL_DestroyTexture(sdlTexture);
}

void clear_screen(SDL_Renderer* sdlRenderer) {
	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
}

void Sprite::draw(float x, float y, float alpha, float angle, float factor) const {
	SDL_Rect destR;
	destR.x = x;
	destR.y = y;
	destR.w = w*factor;
	destR.h = h*factor;

	SDL_RenderCopyEx(sdlRenderer, sdlTexture, NULL, &destR, angle, NULL, SDL_FLIP_NONE);
}

void Sprite::draw_flip_h(float x, float y, float alpha, float angle, float factor) const {
	SDL_Rect destR;
	destR.x = x;
	destR.y = y;
	destR.w = w*factor;
	destR.h = h*factor;

	SDL_RenderCopyEx(sdlRenderer, sdlTexture, NULL, &destR, angle, NULL, SDL_FLIP_HORIZONTAL);
}

void fill_rect(SDL_Renderer* sdlRenderer,float x,float y,float w,float h,float r,float g,float b,float alpha) {
	SDL_Rect dstrect;
	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = w;
	dstrect.h = h;

	SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(sdlRenderer, r*255, g*255, b*255, alpha*255);
	SDL_RenderFillRect(sdlRenderer, &dstrect);
}

void draw_fadein(const Sprite *sprite,float x,float y,int count,int end,int delay,int mode) {
	if (count>end-delay && count<=end) {
		float factor=float(count-end+delay)/(delay+1);
		sprite->draw(x-((end-count)*10*(1-mode)),y-((end-count)*10*mode),factor,0,1);
	} else if (count>end) {
		sprite->draw(x,y,1.0,0);
	}
}

void draw_fadeout(const Sprite *sprite,float x,float y,int count,int end,int delay,float n_turn,float angle) {
	if (count<=end-delay) {
		sprite->draw(x,y,1.0,angle);
	} else if (count<=end) {
		float factor=1-float(count-end+delay)/(delay+1);
		sprite->draw(x,y,factor,angle+360*n_turn*factor,factor);
	}
}
