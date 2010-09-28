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
#include "boardnetwork.h"
#include "param.h"
#include "options.h"

	
#ifdef NET_SUPPORT

#define PORT 2222 /* A changer aussi dans les messages */
#define HANDSHAKE "HelloFS"

UDPpacket* BoardNetwork::paquet = NULL;
UDPsocket BoardNetwork::res_socket;

BoardNetwork::BoardNetwork(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll,BattleField *field,PLAYER player) : BoardAbstract(spr_coll,com_coll,field,player) {
	//init player key context
	
}

void BoardNetwork::init_network(void) {
		/* Init of SDL_net */
		if(SDLNet_Init() < 0) {
			fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

		paquet = SDLNet_AllocPacket(256);
		if(!paquet)	{
			fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		
		/* Initialisation du socket */
		res_socket = SDLNet_UDP_Open(PORT);
		if(!res_socket) {
			fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
	}

bool BoardNetwork::connect() {
	address.port=0;
	
	SDLNet_UDP_Unbind(res_socket, 0);

	if(!SDLNet_UDP_Recv(res_socket, paquet)) return false;

	//~ if(paquet->len == 1 && paquet->data[0] == 'Q') {
		//~ serveur_quitte();
		//~ deconnecte_serveur();
		//~ return false;
	//~ }

	if(strncmp((char *)paquet->data, HANDSHAKE, strlen(HANDSHAKE))) {
		/* Le client n'est pas compatible... on le jette */
		printf("Client incompatible rejeté : %s\n", (char *)paquet->data);
		return true;
	}

	memcpy(&address, &paquet->address, sizeof(IPaddress));

	int chan;
	if((chan=SDLNet_UDP_Bind(res_socket, -1, &address)) <0) {
		fprintf(stderr, "SDLNet_UDP_Bind: %s\n",SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	paquet->len = 0;
	WRITE_8('H');
	WRITE_8(player);
	if(SDLNet_UDP_Send(res_socket, chan, paquet) == 0) {
		fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}

	//SDLNet_UDP_Unbind(res_socket, chan);

	return true;
}

bool BoardNetwork::isConnected() {
	return address.host;
}

void BoardNetwork::logic(bool flowers) {
	bool keys[Keys::NBKEYS] = {false};
	char* reseau_rec; // a faire
	char* tab_joueurs; // a faire
	while(reseau__srv_rec(reseau_rec, Keys::NBKEYS+1)) {
		if(reseau_rec[0] == player) {
			for(int i=0;i<Keys::NBKEYS;i++)
				keys[i] = reseau_rec[i+1];
		}
	}

	if (!this->cursor_changed) {
		if (keys[Keys::LEFT]) this->cursor_j--;
		else if (keys[Keys::RIGHT]) this->cursor_j++;
		if (keys[Keys::UP]) this->cursor_i--;
		else if (keys[Keys::DOWN]) this->cursor_i++;
		this->cursor_changed=keys[Keys::LEFT] || keys[Keys::RIGHT] || keys[Keys::DOWN] || keys[Keys::UP];
	}
	
	if (this->cursor_i>=BOARD_H) this->cursor_i=0;
	else if (this->cursor_i<0) this->cursor_i=BOARD_H-1;
	if (this->cursor_j>=BOARD_W) this->cursor_j=0;
	else if (this->cursor_j<0) this->cursor_j=BOARD_W-1;
	
	char dummy;

	switch (this->state) {
		case IDLE:
			if (keys[Keys::SELECT] && !this->state_changed) {
				this->select_i=this->cursor_i;
				this->select_j=this->cursor_j;
				this->state=SELECTED;
				this->state_changed=true;
			} else if (keys[Keys::VALIDATE] && !this->state_changed) {
				this->state=VALIDATE;
				this->state_changed=true;
			}
			break;
		case SELECTED:
			if (keys[Keys::SWAP] && !this->state_changed) {
				dummy=this->board[cursor_i][cursor_j];
				this->board[cursor_i][cursor_j]=this->board[select_i][select_j];
				this->board[select_i][select_j]=dummy;
				compute();
				this->state=IDLE;
				this->state_changed=true;
			} else if (keys[Keys::SELECT] && !this->state_changed) {
				this->select_i=this->cursor_i;
				this->select_j=this->cursor_j;
				this->state=SELECTED;
				this->state_changed=true;
			} else if (keys[Keys::VALIDATE] && !this->state_changed) {
				this->state=VALIDATE;
				this->state_changed=true;
			}
			break;
		case VALIDATE:
			if (!this->state_changed) {
				validate();
				this->state=IDLE;
				this->state_changed=true;
			}
			break;
		default:
			std::cerr<<"unknown state: "<<this->state<<std::endl;
			break;
	}
	this->state_changed=this->state_changed && (keys[Keys::SELECT] || keys[Keys::SWAP] || keys[Keys::VALIDATE]); //reset state changed
	this->cursor_changed=this->cursor_changed && (keys[Keys::LEFT] || keys[Keys::RIGHT] || keys[Keys::DOWN] || keys[Keys::UP]); //reset cursor changed
}

void BoardNetwork::draw() {
	BoardAbstract::draw();
}

BoardNetwork::~BoardNetwork() {}

#endif
