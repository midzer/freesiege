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

BoardAbstract::BoardAbstract(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll,BattleField *field,PLAYER player) {
	//init player key context
	this->player=player;

	//init sprites stuff
	this->spr_coll=spr_coll;
	this->bit_wood=spr_coll->get_sprite("wood");
	this->bit_steel=spr_coll->get_sprite("steel");
	this->bit_magic=spr_coll->get_sprite("magic");
	this->bit_fire=spr_coll->get_sprite("fire");
	this->bit_cursor=spr_coll->get_sprite("cursor");
	this->frame=spr_coll->get_sprite("frame");
	
	//init combinaisons stuff
	this->com_coll=com_coll;
	
	newGame(field);
}

void BoardAbstract::newGame(BattleField *field) {
	//init board randomly
	for (int i=0;i<BOARD_H;i++) {
		for (int j=0;j<BOARD_W;j++) {
			switch (rand()%4) {
			case 0:
				this->board[i][j]=WOOD;
				break;
			case 1:
				this->board[i][j]=FIRE;
				break;
			case 2:
				this->board[i][j]=STEEL;
				break;
			case 3:
			default:
				this->board[i][j]=MAGIC;
			}
		}
	}
	
	//init board stuff
	this->cursor_i=0;
	this->cursor_j=0;
	this->select_i=-1;
	this->select_j=-1;
	this->state=IDLE;
	this->state_changed=false;
	this->cursor_changed=false;
	
	//init battle field stuff
	this->field=field;
	compute();
	
}

void BoardAbstract::compute() {
	compute_combi_str();
	compute_match();
}

void BoardAbstract::compute_match() {
	int y=0;
	const CombinaisonCollection::Coll::const_iterator end=this->com_coll->coll.end();
	CombinaisonCollection::Coll::const_iterator iter=this->com_coll->coll.begin();
	Combinaison combi(this->combi_str);
	matched.clear();
	while (iter!=end) {
		if (combi.match(*iter->first)) {
			matched.push_back(*iter);
			combi.merge(*iter->first);
		}
		y+=10;
		iter++;
	}
}

void BoardAbstract::compute_combi_str() {
	combi_str="";
	for (int i=BOARD_H-2; i<BOARD_H; i++) {
		for (int j=0;j<BOARD_W;j++) {
			combi_str+=this->board[i][j];
		}
	}
}

void BoardAbstract::draw_background(float offset) {
	//draw elements
	float x,y;
	y=SPACING/2+BOARD_BORDER;
	frame->draw(offset,BOARD_BORDER+3*ELEM_H+3*SPACING);
	for (int i=0;i<BOARD_H;i++) {
		x=SPACING/2+offset;
		for (int j=0;j<BOARD_W;j++) {
			switch (this->board[i][j]) {
			case WOOD:
				bit_wood->draw(x,y);
				break;
			case MAGIC:
				bit_magic->draw(x,y);
				break;
			case STEEL:
				bit_steel->draw(x,y);
				break;
			case FIRE:
				bit_fire->draw(x,y);
				break;
			default:
				break;
			}
			x+=ELEM_W+SPACING;
		}
		y+=ELEM_H+SPACING;
	}
	
	//draw matched sprite
	Combinaison *combi;
	for (CombinaisonCollection::Coll::const_iterator iter=matched.begin(); iter!=matched.end(); iter++) {
		combi=iter->first;
		spr_coll->get_sprite(*iter->second)->draw(SPACING/2+offset+combi->start_j*(ELEM_W+SPACING),SPACING/2+BOARD_BORDER+(combi->start_i+BOARD_H-2)*(ELEM_H+SPACING));
	}
}

void BoardAbstract::validate() {
	if (!matched.empty()) {
		//out the created unit
		const CombinaisonCollection::Coll::const_iterator end=this->matched.end();
		CombinaisonCollection::Coll::const_iterator iter=this->matched.begin();
		//std::cout<<"spawning for player"<<player<<":";
		while (iter!=end) {
			//std::cout<<" "<<*iter->second;
			std::string *name=iter->second;
			if (*name=="soldier") field->spawn(SOLDIER,player);
			else if (*name=="druid") field->spawn(DRUID,player);
			else if (*name=="knight") field->spawn(KNIGHT,player);
			else if (*name=="golem") field->spawn(GOLEM,player);
			else if (*name=="plant") field->spawn(PLANT,player);
			else if (*name=="dragon") field->spawn(DRAGON,player);
			else if (*name=="marion") field->spawn(FLOWER,player);
			else if (*name=="veteran") field->spawn(VETERAN,player);
			else if (*name=="ram") field->spawn(RAM,player);
			else {
              std::cout<<"WARNING default spawing..."<<std::endl;
              field->spawn(SOLDIER,player);
            }
			iter++;
		}
		//std::cout<<std::endl;

		//scrool board and ...
		for (int i=BOARD_H-1;i>=2;i--) {
			for (int j=0;j<BOARD_W;j++) {
				this->board[i][j]=this->board[i-2][j];
			}
		}
		//generate two line randomly
		for (int i=0;i<2;i++) {
			for (int j=0;j<BOARD_W;j++) {
				switch (rand()%4) {
				case 0:
					this->board[i][j]=WOOD;
					break;
				case 1:
					this->board[i][j]=FIRE;
					break;
				case 2:
					this->board[i][j]=STEEL;
					break;
				case 3:
				default:
					this->board[i][j]=MAGIC;
				}
			}
		}
		
		//put board up to date
		compute();
	} //else std::cout<<"nothing to validate for player"<<player<<" ..."<<std::endl;
}

BoardAbstract::~BoardAbstract() {}

void BoardAbstract::draw() {	
	int offset;
	if (player==PLAYER_1) offset=BOARD_BORDER;
	else offset=SCREEN_W-BOARD_W*(ELEM_H+SPACING)-BOARD_BORDER;
	
	draw_background(offset);
	bit_cursor->draw(offset+this->cursor_j*(ELEM_W+SPACING),BOARD_BORDER+this->cursor_i*(ELEM_H+SPACING));
	if (this->state==SELECTED) bit_cursor->draw(offset+this->select_j*(ELEM_W+SPACING),BOARD_BORDER+this->select_i*(ELEM_H+SPACING));
}
