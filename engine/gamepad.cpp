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

std::map<int,shared_ptr<Gamepad>> Gamepad::pads;

Gamepad::Gamepad(int id): joyid(-1) {
	controller = SDL_GameControllerOpen(id);
	if (controller) {
		SDL_Joystick *joy = SDL_GameControllerGetJoystick(controller);
		joyid = SDL_JoystickInstanceID(joy);
	}
}

Gamepad::~Gamepad() {
	if (controller) {
		SDL_GameControllerClose(controller);
	}
}

void Gamepad::loadGamepads()
{
	/* Open the first available controllers. */
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		loadGamepad(i);
	}
}

void Gamepad::closeGamepads()
{
	pads.erase(pads.begin(), pads.end());
}

void Gamepad::loadGamepad(int i)
{
	const char *name = SDL_GameControllerNameForIndex(i);
	if (name) {
		printf("Opening gamepad %i (%s)\n", i, name);
	} else {
		printf("Opening gamepad %i\n", i);
	}
	if (SDL_IsGameController(i)) {
		shared_ptr<Gamepad> g = make_shared<Gamepad>(i);
		if (g->controller) {
			if (pads.find(g->joyid) == pads.end()) {
				pads[g->joyid] = g;
			} else {
				/* Already opened, avoid closing by destructor */
				printf("Already opened\n", i);
				g->controller = nullptr;
			}
		} else {
			fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
		}
	} else {
		fprintf(stderr, "Controller not supported or invalid %i\n", i);
	}
}

void Gamepad::removeGamepad(int joyid)
{
	pads.erase(joyid);
}

Uint8 Gamepad::handleEvent(SDL_Event event, int &player)
{
	switch (event.type) {
		case SDL_CONTROLLERBUTTONDOWN:
			{
				int i = 0;
				for (auto& pad: pads) {
					if (pad.first == event.cbutton.which) {
						player = i;
						return event.cbutton.button;
					}
					i++;
				}
			}
			break;
		case SDL_CONTROLLERDEVICEADDED:
			loadGamepad(event.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			removeGamepad(event.cdevice.which);
			break;
	}
	return SDL_CONTROLLER_BUTTON_MAX;
}
