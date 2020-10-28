/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant 
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2017, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

#include "slikenet/peerinterface.h"
#include "slikenet/FileOperations.h"
#include "slikenet/memoryoverride.h"
#include "slikenet/ConsoleServer.h"
#include "slikenet/LogCommandParser.h"
#include "slikenet/commandparser.h"
#include "slikenet/PacketLogger.h"
#include "slikenet/DS_List.h"
#include "slikenet/SocketLayer.h"
#include "slikenet/sleep.h"
#include "slikenet/TCPInterface.h"

using namespace SLNet;

void* MyMalloc (size_t size)
{
	return malloc(size);
}

void* MyRealloc (void *p, size_t size)
{
	return realloc(p,size);
}

void MyFree (void *p)
{
	free(p);
}

// This project is used to test the DLL system to make sure necessary classes are exported
int main()
{
	// Just test allocation and deallocation across the DLL.  If it crashes it failed, otherwise it worked.
	ConsoleServer* a=ConsoleServer::GetInstance( );
	LogCommandParser* c=LogCommandParser::GetInstance( );
	PacketLogger* d=PacketLogger::GetInstance( );
	RakNetCommandParser* e=RakNetCommandParser::GetInstance( );
	RakPeerInterface * f=RakPeerInterface::GetInstance( );
	TCPInterface *g=TCPInterface::GetInstance();
	SystemAddress sa = UNASSIGNED_SYSTEM_ADDRESS;

	SocketDescriptor sd(5555,0);
	if(f->Startup(32,&sd,1) != RAKNET_STARTED) {		
			printf("NetworkNode::startup(): failed to start server\n");
			return 0;
		}
	f->SetMaximumIncomingConnections(32);
	
	DataStructures::List<RakNetSocket2* > sockets;
	f->GetSockets(sockets);
	f->ReleaseSockets(sockets);

	// See memoryoverride.h
	SetMalloc(MyMalloc);
	SetRealloc(MyRealloc);
	SetFree(MyFree);

	char *cArray = SLNet::OP_NEW_ARRAY<char>(10,_FILE_AND_LINE_);
	SLNet::OP_DELETE_ARRAY(cArray,_FILE_AND_LINE_);

	DataStructures::List<int> intList;
	intList.Push(5, _FILE_AND_LINE_ );
	
	f->GetMTUSize(SLNet::UNASSIGNED_SYSTEM_ADDRESS);
	SystemAddress p1;
	SystemAddress p2;
	p1=p2;

	RakSleep(300);


	ConsoleServer::DestroyInstance( a );
	LogCommandParser::DestroyInstance( c );
	PacketLogger::DestroyInstance( d );
	RakNetCommandParser::DestroyInstance( e );
	SLNet::RakPeerInterface::DestroyInstance( f );
	TCPInterface::DestroyInstance( g );

	return 0;
}

