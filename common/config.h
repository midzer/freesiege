//	This file is part of freesiege program <http://freesiege.sourceforge.net>.
//	Copyright (C) 2007 Pierre Gueth <pierregueth@users.sourceforge.net>
//	                   Joel Schaerer <joelschaerer@users.sourceforge.net>
//                     Bertrand Delhay <zboud@users.sourceforge.net>
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
#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef unsigned int TextureId;
typedef TextureId* TextureIds;
#define N_TEXTURE 512

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stack>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <set>
#include <fstream>
#include <ctime>

#ifdef WIN32
	#include <windows.h>
	#include <SDL.h>
	#include <SDL_mixer.h>
	#include <SDL_ttf.h>
	#include <SDL_image.h>
	#include <SDL_OpenGL.h>
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_mixer.h>
	#include <SDL/SDL_ttf.h>
	#include <SDL/SDL_image.h>
	#include <GL/gl.h>
#endif



#define P2_KEY_SELECT SDLK_o	//key used to select the first element
#define P2_KEY_SWAP SDLK_o	//key used to select the second element and swap it with the first one
#define P2_KEY_VALIDATE SDLK_p	//key used to validate combinaisons and to spawn units on the battlefield
#define P2_KEY_LEFT SDLK_LEFT
#define P2_KEY_RIGHT SDLK_RIGHT
#define P2_KEY_DOWN SDLK_DOWN
#define P2_KEY_UP SDLK_UP

#ifdef WIN32
	#define P1_KEY_SELECT SDLK_q	//key used to select the first element
	#define P1_KEY_SWAP SDLK_q	//key used to select the second element and swap it with the first one
	#define P1_KEY_VALIDATE SDLK_w	//key used to validate combinaisons and to spawn units on the battlefield
#else
	#define P1_KEY_SELECT SDLK_a	//key used to select the first element
	#define P1_KEY_SWAP SDLK_a	//key used to select the second element and swap it with the first one
	#define P1_KEY_VALIDATE SDLK_z	//key used to validate combinaisons and to spawn units on the battlefield
#endif
#define P1_KEY_LEFT SDLK_s
#define P1_KEY_RIGHT SDLK_f
#define P1_KEY_DOWN SDLK_d
#define P1_KEY_UP SDLK_e

#endif
