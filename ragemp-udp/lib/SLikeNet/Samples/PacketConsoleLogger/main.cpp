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

#include "slikenet/TelnetTransport.h"
#include "slikenet/ConsoleServer.h"
#include "slikenet/LogCommandParser.h"
#include "slikenet/PacketConsoleLogger.h"
#include "slikenet/peerinterface.h"
#include "slikenet/sleep.h"
#include <stdio.h>
#include "slikenet/Getche.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/Kbhit.h"

using namespace SLNet;

void main(void)
{
	printf("Shows how to connect telnet to read PacketLogger output from RakPeer.\n");

	RakPeerInterface *rakPeer = SLNet::RakPeerInterface::GetInstance();
	TelnetTransport tt;
	ConsoleServer consoleServer;
	LogCommandParser lcp;
	PacketConsoleLogger pcl;
	pcl.SetLogCommandParser(&lcp);
	consoleServer.AddCommandParser(&lcp);
	consoleServer.SetTransportProvider(&tt, 23);
	rakPeer->AttachPlugin(&pcl);

	SLNet::SocketDescriptor sd(0,0);
	SLNet::StartupResult sr = rakPeer->Startup(32, &sd, 1);
	(void) sr;
	RakAssert(sr==RAKNET_STARTED);

	printf("Use telnet 127.0.0.1 23 to connect from the command window\n");
	printf("Use 'Turn Windows features on and off' with 'Telnet Client' if needed.\n");
	printf("Once telnet has connected, type 'Logger subscribe'\n");
	printf("Press any key in this window once you have done all this.\n");
	SLNet::Packet *packet;
	while (!_kbhit())
	{
		consoleServer.Update();
		RakSleep(30);
	}

	SLNet::ConnectionAttemptResult car = rakPeer->Connect("natpunch.slikesoft.com", 61111, 0, 0);
	(void) car;
	for(;;)
	{
		for (packet=rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet=rakPeer->Receive())
		{
		}

		consoleServer.Update();
		RakSleep(30);
	}	
}
