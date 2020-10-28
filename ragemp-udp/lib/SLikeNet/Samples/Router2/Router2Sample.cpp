/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant 
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2016-2018, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

#include "slikenet/peerinterface.h"
#include <stdio.h>
#include "slikenet/Kbhit.h"
#include <string.h>
#include <stdlib.h>
#include <limits> // used for std::numeric_limits
#include "slikenet/BitStream.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/Router2.h"
#include "slikenet/sleep.h"
#include "slikenet/GetTime.h"
#include "slikenet/Rand.h"
#include "slikenet/assert.h"
#include "slikenet/SocketLayer.h"
#include "slikenet/Getche.h"
#include "slikenet/Gets.h"
#include "slikenet/linux_adapter.h"
#include "slikenet/osx_adapter.h"

using namespace SLNet;

// Global just to make the sample easier to write
RakNetGUID endpointGuid;
RakPeerInterface *endpoint=0, *router=0;
RakPeerInterface *rakPeer;
Router2 *router2Plugin;

void ReadAllPackets(void)
{
	char str[64], str2[64];
	Packet *packet;
	for (packet=rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet=rakPeer->Receive())
	{
		packet->guid.ToString(str, 64);
		packet->systemAddress.ToString(true,str2,static_cast<size_t>(64));
		if (packet->data[0]==ID_NEW_INCOMING_CONNECTION)
		{
			printf("ID_NEW_INCOMING_CONNECTION from %s on %s\n", str, str2);
		}
		else if (packet->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
		{
			printf("ID_CONNECTION_REQUEST_ACCEPTED from %s on %s\n", str, str2);
		}
		else if (packet->data[0]==ID_ROUTER_2_FORWARDING_NO_PATH)
		{
			printf("No path to endpoint exists. Routing failed.\n");
		}
		else if (packet->data[0]==ID_CONNECTION_LOST)
		{
			printf("ID_CONNECTION_LOST from %s\n", str);
		}
		else if (packet->data[0]==ID_USER_PACKET_ENUM+1)
		{
			printf("Got ID_USER_PACKET_ENUM from %s\n", str);
		}
		else if (packet->data[0]==ID_ROUTER_2_FORWARDING_ESTABLISHED)
		{
			SLNet::BitStream bs(packet->data, packet->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			bs.Read(endpointGuid);
			printf("Routing through %s to %s successful. Connecting.\n", str, endpointGuid.ToString());
			unsigned short sourceToDestPort;
			bs.Read(sourceToDestPort);
			char ipAddressString[32];
			packet->systemAddress.ToString(false, ipAddressString,static_cast<size_t>(32));
			rakPeer->Connect(ipAddressString, sourceToDestPort, 0,0);
		}
		else if (packet->data[0]==ID_ROUTER_2_REROUTED)
		{
			// You could read the endpointGuid and sourceToDestPoint if you wanted to
			SLNet::BitStream bs(packet->data, packet->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			RakNetGUID endpointGuid2;
			bs.Read(endpointGuid2);
			endpointGuid2.ToString(str, 64);
			SystemAddress intermediateAddress=packet->systemAddress;
			unsigned short port;
			bs.Read(port);
			intermediateAddress.SetPortHostOrder(port);

			char str3[32];
			intermediateAddress.ToString(true, str3, static_cast<size_t>(32));
			printf("Connection to %s rerouted through %s\n", str, str3);

			// Test sending a message to the endpoint
			SLNet::BitStream bsOut;
			MessageID id = ID_USER_PACKET_ENUM+1;
			bsOut.Write(id);
			rakPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,endpointGuid2,false);
		}
	}
}

int main(void)
{
	printf("Demonstration of the router2 plugin.\n");
	printf("The router2 plugin allows you to connect to a system, routing messages through\n");
	printf("a third system. This is useful if you can connect to the second system, but not\n");
	printf("the third, due to NAT issues.\n");
	printf("1. Start 4 instances, A, B, C, D\n");
	printf("2. Connect A to B and C, B to C, and D to B and C.\n");
	printf("3. On A press 'r' to start routing to D\n");
	printf("4. Once connected, close the router that did the routing to reroute.\n");
	printf("Difficulty: Advanced\n\n");;

	endpointGuid=UNASSIGNED_RAKNET_GUID;
	char str[64], str2[64];
	rakPeer= SLNet::RakPeerInterface::GetInstance();

	rakPeer->SetMaximumIncomingConnections(32);
	SocketDescriptor sd(0,0);
	rakPeer->Startup(32,&sd,1);
	printf("Enter 'c' to connect, 'r' to start routing, 'q' to quit.\n");

	rakPeer->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS).ToString(str, 64);
	printf("My GUID is %s\n", str);
	printf("Started on port %i\n", rakPeer->GetSocket(UNASSIGNED_SYSTEM_ADDRESS)->GetBoundAddress().GetPort());

	rakPeer->SetTimeoutTime(3000,UNASSIGNED_SYSTEM_ADDRESS);
	router2Plugin = new Router2;
//	Router2DebugInterface r2di;
//	router2Plugin->SetDebugInterface(&r2di);
	rakPeer->AttachPlugin(router2Plugin);
	router2Plugin->SetMaximumForwardingRequests(1);

	printf("Sample running. Press 'q' to quit\n");
	for(;;)
	{
		if (_kbhit())
		{
			int ch=_getch();
			if (ch=='q')
				break;
			if (ch=='r')
			{
				do
				{
					printf("Enter destination guid: ");
					Gets(str2,sizeof(str2));
				} while (str2[0]==0);				

				RakNetGUID destinationGuid;
				destinationGuid.FromString(str2);
				router2Plugin->EstablishRouting(destinationGuid);
			}
			if (ch=='c')
			{
				printf("Enter IP address to connect to: ");
				Gets(str,sizeof(str));
				if (str[0]==0)
					strcpy_s(str, "127.0.0.1");
				int intRemotePort = 0;
				do
				{
					printf("Enter port to connect to: ");
					Gets(str2,sizeof(str2));
					intRemotePort = atoi(str2);
					if ((intRemotePort < 0) || (intRemotePort > std::numeric_limits<unsigned short>::max())) {
						printf("Specified remote port %d is outside valid bounds [0, %u]", intRemotePort, std::numeric_limits<unsigned short>::max());
						intRemotePort = 0;
					}
				} while (intRemotePort == 0);

				// Connect
				rakPeer->Connect(str, static_cast<unsigned short>(intRemotePort),0,0);
			}
		}

		RakSleep(30);
		ReadAllPackets();
	}

	SLNet::RakPeerInterface::DestroyInstance(rakPeer);
	delete router2Plugin;
}
