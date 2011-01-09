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
#ifdef NET_SUPPORT
#ifndef __BOARDNETWORK_H
#define __BOARDNETWORK_H

#include "combinaisoncollection.h"
#include "battlefield.h"
#include "boardabstract.h"
#include <SDL_types.h>
#include <SDL_net.h>

#define WRITE_8(x) paquet->data[paquet->len++] = (x)

class BoardNetwork : public BoardAbstract{
public:
	BoardNetwork(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll,BattleField *field,PLAYER player);
	~BoardNetwork();
	void draw();
	void logic(bool flowers);
	
	static void init_network(void);
	
	bool reseau__srv_rec(char* chaine, Uint8 n) {
		if(SDLNet_UDP_Recv(res_socket, paquet) && (paquet->channel == 1 || paquet->channel == 2) && paquet->len <= n) {
			memcpy(chaine, paquet->data, paquet->len);
			return true;
		}
		return false;
	}
	
	bool connect();
	bool isConnected();
	
protected:
	IPaddress address;

	static UDPpacket* paquet;
	static UDPsocket res_socket;
};	

#endif
#endif
