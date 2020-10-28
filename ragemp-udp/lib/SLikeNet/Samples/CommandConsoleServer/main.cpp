/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant 
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2016-2017, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

/// \file
/// \brief Test the command console implementations


#include "slikenet/TCPInterface.h"
#include "slikenet/ConsoleServer.h"
#include "slikenet/commandparser.h"
#include "slikenet/TelnetTransport.h"

#include "slikenet/peerinterface.h"
#include "slikenet/LogCommandParser.h"
#include "slikenet/GetTime.h"
#include "slikenet/transport2.h"
#include "slikenet/LinuxStrings.h"
#include <stdio.h>


void TestTCPInterface(void);
void TestCommandServer(SLNet::TransportInterface *ti, unsigned short port, SLNet::RakPeerInterface *rakPeer);

int main(void)
{
	SLNet::RakPeerInterface *rakPeer = SLNet::RakPeerInterface::GetInstance();
	SLNet::SocketDescriptor sd(60000,0);
 	rakPeer->Startup(128,&sd,1);
 	rakPeer->SetMaximumIncomingConnections(128);

	SLNet::TelnetTransport tt;
	TestCommandServer(&tt, 23, rakPeer); // Uncomment to use Telnet as a client.  Telnet uses port 23 by default.

// 	SLNet::RakNetTransport2 rt2;
// 	rakPeer->AttachPlugin(&rt2);
// 	TestCommandServer(&rt2, 60000,rakPeer); // Uncomment to use RakNet as a client

	return 1;
}

void TestCommandServer(SLNet::TransportInterface *ti, unsigned short port, SLNet::RakPeerInterface *rakPeer)
{
	SLNet::ConsoleServer consoleServer;
	SLNet::RakNetCommandParser rcp;
	SLNet::LogCommandParser lcp;
	SLNet::TimeMS lastLog=0;

	printf("This sample demonstrates the command console server, which can be.\n");
	printf("a standalone application or part of your game server.  It allows you to\n");
	printf("easily parse text strings sent from a client using either secure RakNet\n");
	printf("or Telnet.\n");
	printf("See the 'CommandConsoleClient' project for the RakNet client.\n");
	printf("Difficulty: Intermediate\n\n");

	printf("Command server started on port %i.\n", port);
	consoleServer.AddCommandParser(&rcp);
	consoleServer.AddCommandParser(&lcp);
	consoleServer.SetTransportProvider(ti, port);
	consoleServer.SetPrompt("> "); // Show this character when waiting for user input
	rcp.SetRakPeerInterface(rakPeer);
	lcp.AddChannel("TestChannel");
	for(;;)
	{
		consoleServer.Update();
		// Ignore raknet packets for this sample.
		rakPeer->DeallocatePacket(rakPeer->Receive());

		if (SLNet::GetTimeMS() > lastLog + 4000)
		{
			lcp.WriteLog("TestChannel", "Test of logger");
			lastLog= SLNet::GetTimeMS();
		}

#ifdef _WIN32
		Sleep( 30 );
#else
		usleep( 30 * 1000 );
#endif
	}	
}
