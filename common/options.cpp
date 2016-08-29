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

#include "options.h"
#include "utils.h"
#include <fstream>

using namespace std;

bool Options::sound = true;
bool Options::fullscreen = false;
bool Options::soundConfig = true;
bool Options::fullscreenConfig = false;
Keys Options::player1keys = Keys(P1_KEY_SELECT,P1_KEY_SWAP,P1_KEY_VALIDATE,P1_KEY_LEFT,P1_KEY_RIGHT,P1_KEY_DOWN,P1_KEY_UP);
Keys Options::player2keys = Keys(P2_KEY_SELECT,P2_KEY_SWAP,P2_KEY_VALIDATE,P2_KEY_LEFT,P2_KEY_RIGHT,P2_KEY_DOWN,P2_KEY_UP);
SDL_Scancode Options::pause_key = SDL_SCANCODE_SPACE;

void Options::setSound(bool s) {
	soundConfig=s;
	setSoundSession(s);
}
void Options::setSoundSession(bool s) {
	cout << "sound : " << s << endl;
	sound = s;
}
void Options::setFullscreen(bool f) {
	fullscreen=f;
	fullscreenConfig=f;
}
bool Options::fullscreenOn() {
	return fullscreen;
}
bool Options::soundOn() {
	return sound;
}
void Options::handleArguments(int argc, char* argv[]) {
	string param;
	for (int k=1; k<argc; k++) {
		param=string(argv[k]);
		if (param=="--no-sound") sound=false;
		else if (param=="--windowed") fullscreen=false;
	}
}

void Options::save() {
	string filename = config_file();
	ofstream file(filename.c_str());
	if (file) {
		file << "sound :" << soundConfig << endl;
		file << "fullscreen :" << fullscreenConfig << endl;
		for(int i=0;i<Keys::NBKEYS;i++) {
			file << "p1 " << Keys::name(Keys::KEY(i)) << " \t:" << player1keys.keys[i] << endl;
		}
		for(int i=0;i<Keys::NBKEYS;i++) {
			file << "p2 " << Keys::name(Keys::KEY(i)) << " \t:" << player2keys.keys[i] << endl;
		}
		file << "pause :" << pause_key << endl;
	} else {
		cout << "impossible de sauvegarder le fichier de config (chemin : " << filename << ")" << endl;
	}
}

void Options::load() {
	string filename = string(config_file());
	ifstream file(filename.c_str());
	if(file) {
		file.ignore(256,':');
		file >> soundConfig;
		file.ignore(256,':');
		file >> fullscreenConfig;
		int buffer;
		for(int i=0; i<Keys::NBKEYS;i++) {
			file.ignore(256,':');
			file >> buffer;
			player1keys.keys[i] = SDL_Scancode(buffer);
		}
		if(file.eof()) {
			player1keys = Keys(P1_KEY_SELECT,P1_KEY_SWAP,P1_KEY_VALIDATE,P1_KEY_LEFT,P1_KEY_RIGHT,P1_KEY_DOWN,P1_KEY_UP);
		}
		for(int i=0; i<Keys::NBKEYS;i++) {
			file.ignore(256,':');
			file >> buffer;
			player2keys.keys[i] = SDL_Scancode(buffer);
		}
		if(file.eof()) {
			player2keys = Keys(P2_KEY_SELECT,P2_KEY_SWAP,P2_KEY_VALIDATE,P2_KEY_LEFT,P2_KEY_RIGHT,P2_KEY_DOWN,P2_KEY_UP);
		}
		file.ignore(256,':');
		file >> buffer;
		pause_key = SDL_Scancode(buffer);
		if(file.eof())
			pause_key = SDL_SCANCODE_SPACE;

		cout << "d'après le fichier de config, sound =" << soundConfig << " et fullscreen =" << fullscreenConfig << endl;
	} else {
		cout << "fichier de config \"" << config_file() << "\" inexistant, configuration par défaut appliquée" << endl;
		soundConfig = true;
		fullscreenConfig = false;
	}
	sound = soundConfig;
	fullscreen = fullscreenConfig;
}
