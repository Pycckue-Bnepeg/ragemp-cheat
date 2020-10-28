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

#include "slikenet/BitStream.h"
#include <stdlib.h> // For atoi
#include <cstring> // For strlen
#include <stdio.h>
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/GetTime.h"
#include "slikenet/Gets.h"

#ifdef _WIN32
#include "slikenet/WindowsIncludes.h" // Sleep
#else
#include <unistd.h> // usleep
#include <cstdio>
#endif

int nextTest;

SLNet::RakPeerInterface *peer1= SLNet::RakPeerInterface::GetInstance();
SLNet::RakPeerInterface *peer2= SLNet::RakPeerInterface::GetInstance();

int main(void)
{
	char text[1024];
	nextTest=0;

	printf("This project tests the advertise system and offline ping messages.\n");
	printf("Difficulty: Beginner\n\n");

	peer1->SetMaximumIncomingConnections(1);
	SLNet::SocketDescriptor socketDescriptor(60001, 0);
	peer1->Startup(1, &socketDescriptor, 1);
	socketDescriptor.port=60002;
	peer2->Startup(1, &socketDescriptor, 1);
	peer1->SetOfflinePingResponse("Offline Ping Data", (int)strlen("Offline Ping Data")+1);

	printf("Peer 1 guid = %s\n", peer1->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	printf("Peer 2 guid = %s\n", peer2->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	printf("Systems started.  Waiting for advertise system packet\n");

	// Wait for connection to complete
#ifdef _WIN32
	Sleep(300);
#else
	usleep(300 * 1000);
#endif

	printf("Sending advertise system from %s\n", peer1->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	peer1->AdvertiseSystem("127.0.0.1", 60002,"hello world", (int)strlen("hello world")+1);

	while (nextTest!=2)
	{
		peer1->DeallocatePacket(peer1->Receive());
		SLNet::Packet *packet = peer2->Receive();
		if (packet)
		{
			if (packet->data[0]==ID_ADVERTISE_SYSTEM)
			{
				if (packet->length>1)
					printf("Got Advertise system with data: %s\n", packet->data+1);
				else
					printf("Got Advertise system with no data\n");
				printf("Was sent from GUID %s\n", packet->guid.ToString());

				printf("Sending ping from %s\n", peer2->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
				peer2->Ping("127.0.0.1", 60001, false);
				nextTest++;
			}
			else if (packet->data[0]==ID_UNCONNECTED_PONG)
			{
				// Peer or client. Response from a ping for an unconnected system.
				SLNet::TimeMS packetTime, dataLength;
				SLNet::TimeMS curTime = SLNet::GetTimeMS();
				SLNet::BitStream bsIn(packet->data,packet->length,false);
				bsIn.IgnoreBytes(1);
				bsIn.Read(packetTime);
				dataLength = packet->length - sizeof( unsigned char ) - sizeof(SLNet::TimeMS );
				if (peer2->IsLocalIP(packet->systemAddress.ToString(false)))
					printf("ID_UNCONNECTED_PONG from our own");
				else
					printf( "ID_UNCONNECTED_PONG from");
				printf(" %s on %p.\nPing is %i\nData is %i bytes long.\n", packet->systemAddress.ToString(), peer2, curTime-packetTime, dataLength );
				printf("Was sent from GUID %s\n", packet->guid.ToString());

				if ( dataLength > 0 )
				{
					printf( "Data is %s\n", packet->data + sizeof( unsigned char ) + sizeof(SLNet::TimeMS ) );
					RakAssert(strlen("Offline Ping Data")+1==packet->length-(sizeof( unsigned char ) + sizeof(SLNet::TimeMS )));
				}

				nextTest++;
				// ProcessUnhandledPacket(packet, ID_UNCONNECTED_PONG,interfaceType);
			}
			peer2->DeallocatePacket(packet);
		}

#ifdef _WIN32
		Sleep(30);
#else
		usleep(30 * 1000);
#endif
	}

	printf("Test complete. Press enter to quit\n");
	Gets(text,sizeof(text));

	SLNet::RakPeerInterface::DestroyInstance(peer1);
	SLNet::RakPeerInterface::DestroyInstance(peer2);

	return 0;
}
