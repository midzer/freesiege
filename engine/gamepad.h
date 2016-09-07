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
#ifndef __GAMEPAD_H
#define __GAMEPAD_H

#include "config.h"

class Gamepad {
	/* Stores the gamepad for each player */
	static std::vector<Gamepad> pads;
	static const int NB_PLAYERS = 2;

	SDL_GameController *controller;
	SDL_JoystickID joyid;
	public:
		static void loadGamepads();
		/* Returns pushed button if any
		 * Puts player id in player */
		static Uint8 handleEvent(SDL_Event event, int &player);
		Gamepad(int id);
};

#endif
