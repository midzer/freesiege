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

#include "boardsurvivor.h"
#include <cmath>

using namespace std;

BoardSurvivor::BoardSurvivor(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll, BattleField *field, PLAYER p, int base_speed) : Board(spr_coll, com_coll, field, p) {
	ticks = 1;
	this->level = 1;
	this->base_speed = base_speed;
}

void BoardSurvivor::newGame(BattleField *field) {
	BoardAbstract::newGame(field);
	this->ticks = 1;
}

void BoardSurvivor::logic(bool flowers) {
	if (not flowers or ticks%4==0) //the flower randomly slows the computer
	{
		int soldier_tics=(base_speed * pow(0.9,level));
		if (ticks%soldier_tics==0)
			field->spawn(SOLDIER,player);
		if (ticks%(soldier_tics*6)==0)
			field->spawn(KNIGHT,player);
		if (ticks%(soldier_tics*7)==0)
			field->spawn(VETERAN,player);
		if (ticks%(soldier_tics*20)==0)
			field->spawn(PLANT,player);
		if (ticks%(soldier_tics*24)==0)
			field->spawn(GOLEM,player);
		if (ticks%(soldier_tics*22)==0)
			field->spawn(DRAGON,player);
	}
	ticks++;
}

void BoardSurvivor::set_ai_level(MenuScreen::AILEVEL ai_level)
{
    switch(ai_level)
      {
        case 0:
            base_speed=500;
            break;
        case 1:
            base_speed=150;
            break;
        case 2:
            base_speed=90;
            break;
        case 3:
            base_speed=60;
            break;
      }
}

void BoardSurvivor::hasLost() {
	level++;
}
