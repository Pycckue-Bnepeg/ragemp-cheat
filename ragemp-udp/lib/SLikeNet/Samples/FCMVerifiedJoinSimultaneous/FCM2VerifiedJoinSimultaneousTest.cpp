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

#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "slikenet/GetTime.h"
#include "slikenet/peerinterface.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/types.h"
#include "slikenet/sleep.h"
#include "slikenet/FullyConnectedMesh2.h"
#include "slikenet/ConnectionGraph2.h"
#include <assert.h>
#include "slikenet/SocketLayer.h"
#include "slikenet/Kbhit.h"
#include "slikenet/PacketLogger.h"
#include "slikenet/Gets.h"
#include "slikenet/BitStream.h"

using namespace SLNet;

#define NUM_PEERS 4
SLNet::RakPeerInterface *rakPeer[NUM_PEERS];

class FullyConnectedMesh2_UserData : public FullyConnectedMesh2
{
	virtual void WriteVJCUserData(SLNet::BitStream *bsOut) {bsOut->Write(RakString("WriteVJCUserData test"));}
	virtual void WriteVJSUserData(SLNet::BitStream *bsOut, RakNetGUID userGuid) {bsOut->Write(RakString("WriteVJSUserData test, userGuid=%s", userGuid.ToString()));}
};

int main()
{
	FullyConnectedMesh2_UserData fcm2[NUM_PEERS];

	for (unsigned short i=0; i < NUM_PEERS; i++)
	{
		rakPeer[i]= SLNet::RakPeerInterface::GetInstance();
		rakPeer[i]->AttachPlugin(&fcm2[i]);
		fcm2[i].SetAutoparticipateConnections(false);
		fcm2[i].SetConnectOnNewRemoteConnection(false, "");
		SLNet::SocketDescriptor sd;
		sd.port=60000+i;
		SLNET_VERIFY(rakPeer[i]->Startup(NUM_PEERS, &sd, 1) == RAKNET_STARTED);
		rakPeer[i]->SetMaximumIncomingConnections(NUM_PEERS);
		rakPeer[i]->SetTimeoutTime(1000, SLNet::UNASSIGNED_SYSTEM_ADDRESS);
		printf("%i. Our guid is %s\n", i, rakPeer[i]->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	}
	
	RakSleep(100);

	for (int i=1; i < NUM_PEERS; i++)
	{
		SLNET_VERIFY(rakPeer[i]->Connect("127.0.0.1", 60000, 0, 0) == CONNECTION_ATTEMPT_STARTED);
	}
	
	RakSleep(100);
	
	for (int i=1; i < NUM_PEERS; i++)
	{
		fcm2[0].StartVerifiedJoin(rakPeer[i]->GetMyGUID());
	}
	

	bool quit=false;
	SLNet::Packet *packet;
	int ch;
	while (!quit)
	{
		for (int peerIndex=0; peerIndex < NUM_PEERS; peerIndex++)
		{
			for (packet = rakPeer[peerIndex]->Receive(); packet; rakPeer[peerIndex]->DeallocatePacket(packet), packet = rakPeer[peerIndex]->Receive())
			{
				switch (packet->data[0])
				{
				case ID_FCM2_VERIFIED_JOIN_START:
					{
						printf("%s: Got ID_FCM2_VERIFIED_JOIN_START from %s. address=", rakPeer[peerIndex]->GetMyGUID().ToString(), packet->guid.ToString());
						DataStructures::List<SystemAddress> addresses;
						DataStructures::List<RakNetGUID> guids;
						DataStructures::List<BitStream*> userData;
						fcm2[peerIndex].GetVerifiedJoinRequiredProcessingList(packet->guid, addresses, guids, userData);
						for (unsigned int i=0; i < guids.Size(); i++)
						{
							printf("%s:", guids[i].ToString());
							ConnectionAttemptResult car = rakPeer[peerIndex]->Connect(addresses[i].ToString(false), addresses[i].GetPort(), 0, 0);
							switch (car)
							{
							case CONNECTION_ATTEMPT_STARTED:
								printf("CONNECTION_ATTEMPT_STARTED");
								break;
							case ALREADY_CONNECTED_TO_ENDPOINT:
								printf("ALREADY_CONNECTED_TO_ENDPOINT");
								break;
							case CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
								printf("CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS");
								break;
							default:
								printf("Other");
							}
							printf(" ");

							RakString userDataAsStr;
							userData[i]->Read(userDataAsStr);
							printf(userDataAsStr.C_String());
						}
						printf("\n");
					}
					break;

				case ID_FCM2_VERIFIED_JOIN_FAILED:
					printf("%s: ID_FCM2_VERIFIED_JOIN_FAILED from %s\n", rakPeer[peerIndex]->GetMyGUID().ToString(), packet->guid.ToString());
					break;

				case ID_FCM2_VERIFIED_JOIN_CAPABLE:
					{
						SLNet::BitStream bs(packet->data,packet->length,false);
						FullyConnectedMesh2::SkipToVJCUserData(&bs);
						RakString testStr;
						bs.Read(testStr);

						printf("%s: ID_FCM2_VERIFIED_JOIN_CAPABLE from %s\n", rakPeer[peerIndex]->GetMyGUID().ToString(), packet->guid.ToString());
						printf("STR: %s\n", testStr.C_String());
						fcm2[peerIndex].RespondOnVerifiedJoinCapable(packet, true, 0);
					}
					break;

				case ID_FCM2_VERIFIED_JOIN_ACCEPTED:
					{
						bool thisSystemAccepted;
						DataStructures::List<RakNetGUID> systemsAccepted;
						SLNet::BitStream additionalData;
						fcm2[peerIndex].GetVerifiedJoinAcceptedAdditionalData(packet, &thisSystemAccepted, systemsAccepted, &additionalData);
						if (thisSystemAccepted)
						{
							printf("%s: ID_FCM2_VERIFIED_JOIN_ACCEPTED from %s. systemsAccepted=", rakPeer[peerIndex]->GetMyGUID().ToString(), packet->guid.ToString());
							for (unsigned int i=0; i < systemsAccepted.Size(); i++)
								printf("%s ", systemsAccepted[i].ToString());
							printf("\n");
						}
						break;
					}

				case ID_FCM2_VERIFIED_JOIN_REJECTED:
					printf("%s: ID_FCM2_VERIFIED_JOIN_REJECTED from %s\n", rakPeer[peerIndex]->GetMyGUID().ToString(), packet->guid.ToString());
					rakPeer[peerIndex]->CloseConnection(packet->guid, true);
					break;

				default:
					printf("%s: %s from %s\n", rakPeer[peerIndex]->GetMyGUID().ToString(), PacketLogger::BaseIDTOString(packet->data[0]), packet->guid.ToString());
				}
			}
		}

		if (_kbhit())
		{
			ch=_getch();
			if (ch=='q' || ch=='Q')
			{
				printf("Quitting.\n");
				quit=true;
			}
		}


		RakSleep(30);
	}

	for (int i=0; i < NUM_PEERS; i++)
	{
		SLNet::RakPeerInterface::DestroyInstance(rakPeer[i]);
	}
	return 0;
}
