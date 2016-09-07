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
#include "board.h"
#include "param.h"
#include "options.h"
#include "gamepad.h"

Board::Board(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll,BattleField *field,PLAYER player) : BoardAbstract(spr_coll,com_coll,field,player) {
	//init player key context
	if (player==PLAYER_1) {
		key_select=Options::player1keys.keys[Keys::SELECT];
		key_swap=Options::player1keys.keys[Keys::SWAP];
		key_validate=Options::player1keys.keys[Keys::VALIDATE];
		key_left=Options::player1keys.keys[Keys::LEFT];
		key_right=Options::player1keys.keys[Keys::RIGHT];
		key_down=Options::player1keys.keys[Keys::DOWN];
		key_up=Options::player1keys.keys[Keys::UP];
	} else {
		key_select=Options::player2keys.keys[Keys::SELECT];
		key_swap=Options::player2keys.keys[Keys::SWAP];
		key_validate=Options::player2keys.keys[Keys::VALIDATE];
		key_left=Options::player2keys.keys[Keys::LEFT];
		key_right=Options::player2keys.keys[Keys::RIGHT];
		key_down=Options::player2keys.keys[Keys::DOWN];
		key_up=Options::player2keys.keys[Keys::UP];
	}
}

void Board::logic(bool flowers) {
	const Uint8 *key=SDL_GetKeyboardState(NULL);

	if (this->cursor_changed) {
		if (!(
			key[key_left] || key[key_right] || key[key_down] || key[key_up]
			|| Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
			|| Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
			|| Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_UP)
			|| Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
			)) {
			this->cursor_changed = false;
		}
	} else {
		if (key[key_left] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
			this->cursor_j--;
			this->cursor_changed = true;
		} else if (key[key_right] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
			this->cursor_j++;
			this->cursor_changed = true;
		}
		if (key[key_up] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
			this->cursor_i--;
			this->cursor_changed = true;
		} else if (key[key_down] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
			this->cursor_i++;
			this->cursor_changed = true;
		}
	}

	if (this->cursor_i>=BOARD_H) this->cursor_i=0;
	else if (this->cursor_i<0) this->cursor_i=BOARD_H-1;
	if (this->cursor_j>=BOARD_W) this->cursor_j=0;
	else if (this->cursor_j<0) this->cursor_j=BOARD_W-1;

	char dummy;

	if (this->state_changed) {
		if (!(
			key[key_select] || key[key_swap] || key[key_validate]
			|| Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_A)
			|| Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_B)
			)) {
			this->state_changed = false;
		}
	} else {
		switch (this->state) {
			case IDLE:
				if (key[key_select] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_A)) {
					this->select_i=this->cursor_i;
					this->select_j=this->cursor_j;
					this->state=SELECTED;
					this->state_changed=true;
				} else if (key[key_validate] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_B)) {
					this->state=VALIDATE;
					this->state_changed=true;
				}
				break;
			case SELECTED:
				if (key[key_swap] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_A)) {
					dummy=this->board[cursor_i][cursor_j];
					this->board[cursor_i][cursor_j]=this->board[select_i][select_j];
					this->board[select_i][select_j]=dummy;
					compute();
					this->state=IDLE;
					this->state_changed=true;
				} else if (key[key_select] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_A)) {
					this->select_i=this->cursor_i;
					this->select_j=this->cursor_j;
					this->state=SELECTED;
					this->state_changed=true;
				} else if (key[key_validate] || Gamepad::state(player-1, SDL_CONTROLLER_BUTTON_B)) {
					this->state=VALIDATE;
					this->state_changed=true;
				}
				break;
			case VALIDATE:
				validate();
				this->state=IDLE;
				this->state_changed=true;
				break;
			default:
				std::cerr<<"unknown state: "<<this->state<<std::endl;
				break;
		}
	}
}

Board::~Board() {}
