/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant 
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2016-2019, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

// ----------------------------------------------------------------------
// RakNet version 1.0
// Filename ChatExample.cpp
// Very basic chat engine example
// ----------------------------------------------------------------------

#include "slikenet/MessageIdentifiers.h"

#include "slikenet/peerinterface.h"
#include "slikenet/statistics.h"
#include "slikenet/types.h"
#include "slikenet/BitStream.h"
#include "slikenet/PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <limits> // used for std::numeric_limits
#include "slikenet/types.h"
#include "slikenet/Kbhit.h"
#ifdef _WIN32
#include "slikenet/WindowsIncludes.h" // Sleep
#else
#include <unistd.h> // usleep
#endif
#include "slikenet/Gets.h"
#include "slikenet/linux_adapter.h"
#include "slikenet/osx_adapter.h"

#if LIBCAT_SECURITY==1
#include "slikenet/SecureHandshake.h" // Include header for secure handshake
#endif

// We copy this from Multiplayer.cpp to keep things all in one file for this example
unsigned char GetPacketIdentifier(SLNet::Packet *p);

int main(void)
{
	SLNet::RakNetStatistics *rss;
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	SLNet::RakPeerInterface *client= SLNet::RakPeerInterface::GetInstance();
//	client->InitializeSecurity(0,0,0,0);
	//SLNet::PacketLogger packetLogger;
	//client->AttachPlugin(&packetLogger);

	
	// Holds packets
	SLNet::Packet* p;

	// GetPacketIdentifier returns this
	unsigned char packetIdentifier;

	// Just so we can remember where the packet came from
	bool isServer;

	// Record the first client that connects to us so we can pass it to the ping function
	SLNet::SystemAddress clientID= SLNet::UNASSIGNED_SYSTEM_ADDRESS;

	// Crude interface

	// Holds user data
	char ip[64], serverPort[30], clientPort[30];

	// A client
	isServer=false;

	printf("This is a sample implementation of a text based chat client.\n");
	printf("Connect to the project 'Chat Example Server'.\n");
	printf("Difficulty: Beginner\n\n");

	// Get our input
	puts("Enter the client port to listen on");
	Gets(clientPort,sizeof(clientPort));
	if (clientPort[0]==0)
		strcpy_s(clientPort, "0");
	const int intClientPort = atoi(clientPort);
	if ((intClientPort < 0) || (intClientPort > std::numeric_limits<unsigned short>::max())) {
		printf("Specified client port %d is outside valid bounds [0, %u]", intClientPort, std::numeric_limits<unsigned short>::max());
		return 1;
	}

	puts("Enter IP to connect to");
	Gets(ip, sizeof(ip));
	client->AllowConnectionResponseIPMigration(false);
	if (ip[0]==0)
		strcpy_s(ip, "127.0.0.1");
	// strcpy_s(ip, "natpunch.slikesoft.com");
	
		
	puts("Enter the port to connect to");
	Gets(serverPort,sizeof(serverPort));
	if (serverPort[0]==0)
		strcpy_s(serverPort, "1234");
	int intServerPort = atoi(serverPort);
	if ((intServerPort < 0) || (intServerPort > std::numeric_limits<unsigned short>::max())) {
		printf("Specified server port %d is outside valid bounds [0, %u]", intServerPort, std::numeric_limits<unsigned short>::max());
		return 2;
	}

	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads
	SLNet::SocketDescriptor socketDescriptor(static_cast<unsigned short>(intClientPort),0);
	socketDescriptor.socketFamily=AF_INET;
	client->Startup(8,&socketDescriptor, 1);
	client->SetOccasionalPing(true);


#if LIBCAT_SECURITY==1
	char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
	FILE *fp;
	fopen_s(&fp,"publicKey.dat","rb");
	fread(public_key,sizeof(public_key),1,fp);
	fclose(fp);

	SLNet::PublicKey pk;
	pk.remoteServerPublicKey=public_key;
	pk.publicKeyMode= SLNet::PKM_USE_KNOWN_PUBLIC_KEY;
	client->Connect(ip, static_cast<unsigned short>(intServerPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"), &pk);
#else
	SLNet::ConnectionAttemptResult car = client->Connect(ip, static_cast<unsigned short>(intServerPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));
	RakAssert(car== SLNet::CONNECTION_ATTEMPT_STARTED);
#endif

	printf("\nMy IP addresses:\n");
	unsigned int i;
	for (i=0; i < client->GetNumberOfAddresses(); i++)
	{
		printf("%i. %s\n", i+1, client->GetLocalIP(i));
	}

	printf("My GUID is %s\n", client->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	puts("'quit' to quit. 'stat' to show stats. 'ping' to ping.\n'disconnect' to disconnect. 'connect' to reconnnect. Type to talk.");
	
	char message[2048];

	// Loop for input
	for(;;)
	{
		// This sleep keeps RakNet responsive
#ifdef _WIN32
		Sleep(30);
#else
		usleep(30 * 1000);
#endif


		if (_kbhit())
		{
			// Notice what is not here: something to keep our network running.  It's
			// fine to block on Gets or anything we want
			// Because the network engine was painstakingly written using threads.
			Gets(message,sizeof(message));

			if (strcmp(message, "quit")==0)
			{
				puts("Quitting.");
				break;
			}

			if (strcmp(message, "stat")==0)
			{
				
				rss=client->GetStatistics(client->GetSystemAddressFromIndex(0));
				StatisticsToString(rss, message, 2048, 2);
				printf("%s", message);
				printf("Ping=%i\n", client->GetAveragePing(client->GetSystemAddressFromIndex(0)));
			
				continue;
			}

			if (strcmp(message, "disconnect")==0)
			{
				printf("Enter index to disconnect: ");
				char str[32];
				Gets(str, sizeof(str));
				if (str[0]==0)
					strcpy_s(str,"0");
				int index = atoi(str);
				client->CloseConnection(client->GetSystemAddressFromIndex(index),false);
				printf("Disconnecting.\n");
				continue;
			}

			if (strcmp(message, "shutdown")==0)
			{
				client->Shutdown(100);
				printf("Shutdown.\n");
				continue;
			}

			if (strcmp(message, "startup")==0)
			{
				bool b = client->Startup(8,&socketDescriptor, 1)== SLNet::RAKNET_STARTED;
				if (b)
					printf("Started.\n");
				else
					printf("Startup failed.\n");
				continue;
			}


			if (strcmp(message, "connect")==0)
			{
				printf("Enter server ip: ");
				Gets(ip, sizeof(ip));
				if (ip[0]==0)
					strcpy_s(ip, "127.0.0.1");

				printf("Enter server port: ");				
				Gets(serverPort,sizeof(serverPort));
				if (serverPort[0]==0)
					strcpy_s(serverPort, "1234");
				intServerPort = atoi(serverPort);
				if ((intServerPort < 0) || (intServerPort > std::numeric_limits<unsigned short>::max())) {
					printf("Specified server port %d is outside valid bounds [0, %u]", intServerPort, std::numeric_limits<unsigned short>::max());
					return 2;
				}

#if LIBCAT_SECURITY==1
				bool b = client->Connect(ip, static_cast<unsigned short>(intServerPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"), &pk)== SLNet::CONNECTION_ATTEMPT_STARTED;
#else
				bool b = client->Connect(ip, static_cast<unsigned short>(intServerPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"))== SLNet::CONNECTION_ATTEMPT_STARTED;
#endif

				if (b)
					puts("Attempting connection");
				else
				{
					puts("Bad connection attempt.  Terminating.");
					exit(1);
				}
				continue;
			}

			if (strcmp(message, "ping")==0)
			{
				if (client->GetSystemAddressFromIndex(0)!= SLNet::UNASSIGNED_SYSTEM_ADDRESS)
					client->Ping(client->GetSystemAddressFromIndex(0));

				continue;
			}

			if (strcmp(message, "getlastping")==0)
			{
				if (client->GetSystemAddressFromIndex(0)!= SLNet::UNASSIGNED_SYSTEM_ADDRESS)
					printf("Last ping is %i\n", client->GetLastPing(client->GetSystemAddressFromIndex(0)));

				continue;
			}

			// message is the data to send
			// strlen(message)+1 is to send the null terminator
			// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
			// RELIABLE_ORDERED means make sure the message arrives in the right order
			client->Send(message, (int) strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}

		// Get a packet from either the server or the client

		for (p=client->Receive(); p; client->DeallocatePacket(p), p=client->Receive())
		{
			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", p->guid.g);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n"); 
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;			
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
				printf("My external address is %s\n", client->GetExternalID(p->systemAddress).ToString(true));
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", p->systemAddress.ToString(true));
				break;
			default:
				// It's a client, so just show the message
				printf("%s\n", p->data);
				break;
			}
		}
	}

	// Be nice and let the server know we quit.
	client->Shutdown(300);

	// We're done with the network
	SLNet::RakPeerInterface::DestroyInstance(client);

	return 0;
}

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(SLNet::Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(SLNet::MessageID) + sizeof(SLNet::Time));
		return (unsigned char) p->data[sizeof(SLNet::MessageID) + sizeof(SLNet::Time)];
	}
	else
		return (unsigned char) p->data[0];
}