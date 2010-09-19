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
#ifndef __BOARD_SURVIVOR_H
#define __BOARD_SURVIVOR_H

#include "board.h"
#include "menuscreen.h"

class BoardSurvivor : public Board {
	public:
		BoardSurvivor(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll, BattleField *field, PLAYER p, int base_speed=150);
		void draw() {};
		void logic(bool flowers);
		void set_ai_level(MenuScreen::AILEVEL ai_level);
		void hasWin();
		int getLevel() {return level;}
	private:
		int ticks;
		int level;
		int base_speed;
};


#endif
