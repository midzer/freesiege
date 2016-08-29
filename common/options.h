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
#ifndef __OPTIONS_H
#define __OPTIONS_H

#include "config.h"

class Keys {
	public:
		enum KEY {SELECT, SWAP, VALIDATE, LEFT, RIGHT, DOWN, UP, NBKEYS};
		Keys(SDL_Scancode se,SDL_Scancode sw,SDL_Scancode va,SDL_Scancode le,SDL_Scancode ri,SDL_Scancode d,SDL_Scancode up) {
			keys[SELECT] = se;
			keys[SWAP] = sw;
			keys[VALIDATE] = va;
			keys[LEFT] = le;
			keys[RIGHT] = ri;
			keys[DOWN] = d;
			keys[UP]= up;
		};
		SDL_Scancode keys[NBKEYS];
		static std::string name(KEY key) {
			switch(key) {
				case SELECT: return "select";
				case SWAP: return "swap";
				case VALIDATE: return "validate";
				case LEFT: return "left";
				case RIGHT: return "right";
				case DOWN: return "down";
				case UP: return "up";
				case NBKEYS: return "number of keys";
			}
		}
};

class Options {
	public:
		static void save();
		static void load();
		static void setSound(bool s);
		static void setSoundSession(bool s);
		static void setFullscreen(bool f);
		static void handleArguments(int argc, char* argv[]);
		static bool fullscreenOn();
		static bool soundOn();
		static Keys player1keys;
		static Keys player2keys;
		static SDL_Scancode pause_key;
	private:
		static bool soundConfig;		//what is in the config file
		static bool fullscreenConfig;	//what is in the config file
		static bool fullscreen;			//the value for this instance (can be modified by command line arguments)
		static bool sound;				//the value for this instance (can be modified by command line arguments)
};

#endif
