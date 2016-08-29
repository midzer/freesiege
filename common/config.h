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
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_image.h>
#endif



#define P2_KEY_SELECT SDL_SCANCODE_O	//key used to select the first element
#define P2_KEY_SWAP SDL_SCANCODE_O	//key used to select the second element and swap it with the first one
#define P2_KEY_VALIDATE SDL_SCANCODE_P	//key used to validate combinaisons and to spawn units on the battlefield
#define P2_KEY_LEFT SDL_SCANCODE_LEFT
#define P2_KEY_RIGHT SDL_SCANCODE_RIGHT
#define P2_KEY_DOWN SDL_SCANCODE_DOWN
#define P2_KEY_UP SDL_SCANCODE_UP

#define P1_KEY_SELECT SDL_SCANCODE_Q	//key used to select the first element
#define P1_KEY_SWAP SDL_SCANCODE_Q	//key used to select the second element and swap it with the first one
#define P1_KEY_VALIDATE SDL_SCANCODE_W	//key used to validate combinaisons and to spawn units on the battlefield
#define P1_KEY_LEFT SDL_SCANCODE_S
#define P1_KEY_RIGHT SDL_SCANCODE_F
#define P1_KEY_DOWN SDL_SCANCODE_D
#define P1_KEY_UP SDL_SCANCODE_E

#endif
