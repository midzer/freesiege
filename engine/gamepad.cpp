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

#include "gamepad.h"

using namespace std;

std::vector<Gamepad> Gamepad::pads;

Gamepad::Gamepad(int id): joyid(-1) {
	controller = SDL_GameControllerOpen(id);
	if (controller) {
		SDL_Joystick *joy = SDL_GameControllerGetJoystick(controller);
		joyid = SDL_JoystickInstanceID(joy);
	}
}

void Gamepad::loadGamepads()
{
	/* Open the first available controllers. */
	for (int i = 0; ((i < SDL_NumJoysticks()) && (i < NB_PLAYERS)); ++i) {
		if (SDL_IsGameController(i)) {
			Gamepad g(i);
			if (g.controller) {
				pads.push_back(g);
			} else {
				fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
}

Uint8 Gamepad::handleEvent(SDL_Event event, int &player)
{
	switch (event.type) {
		case SDL_CONTROLLERBUTTONDOWN:
			for(int i = 0; ((i < NB_PLAYERS) && (i < pads.size())); ++i) {
				if (pads[i].joyid == event.cbutton.which) {
					player = i;
					return event.cbutton.button;
				}
			}
			break;
	}
	return SDL_CONTROLLER_BUTTON_MAX;
/*Gestion des évènements:
Joystick débranché, retiré du tableau et fermé, réassigne un pad au joueur
Joystick branché, si joueur sans pad, on assigne et on ouvre (sinon non?)*/
}
