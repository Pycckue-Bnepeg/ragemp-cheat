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
#include "slikenet/Rand.h"
#include "slikenet/peerinterface.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/FullyConnectedMesh2.h"
#include "slikenet/TeamManager.h"
#include "slikenet/Kbhit.h"
#include "slikenet/sleep.h"
#include "slikenet/types.h"
#include "slikenet/BitStream.h"
#include "slikenet/SocketLayer.h"
#include "slikenet/ReplicaManager3.h"
#include "slikenet/NetworkIDManager.h"

using namespace SLNet;

// Used by TeamManager to call SetHost() automatically when the current host peer drops
// Also used to determine who the host is for the purposes of serializing objects to new systems
FullyConnectedMesh2 *fullyConnectedMesh2;
// UDP network communication
RakPeerInterface *rakPeer;
// Maintains pointers to TM_Team and TM_TeamMember, which contain team related functionality
TeamManager *teamManager;
// Used by ReplicaManager3 (below) for object lookup
NetworkIDManager *networkIDManager;

// class Team is implemented as a static object
// A static object is one that already exists on systems before connection, as opposed to being created on demand.
// see Help/replicamanager3.html under the topic Static Objects for what to return from the implemented interfaces
class Team : public Replica3
{
public:
	Team()
	{
		tmTeam.SetOwner(this);
	}
	virtual ~Team()
	{
	}
	virtual void WriteAllocationID(SLNet::Connection_RM3 *destinationConnection, SLNet::BitStream *allocationIdBitstream) const
	{
		// unused parameters
		(void)destinationConnection;
		(void)allocationIdBitstream;
	}
	virtual RM3ConstructionState QueryConstruction(SLNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3)
	{
		// unused parameters
		(void)destinationConnection;
		(void)replicaManager3;

		if (fullyConnectedMesh2->IsConnectedHost())
			return RM3CS_ALREADY_EXISTS_REMOTELY;
		return RM3CS_ALREADY_EXISTS_REMOTELY_DO_NOT_CONSTRUCT;
	}
	virtual bool QueryRemoteConstruction(SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)sourceConnection;

		return false;
	}
	virtual void SerializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
	{
		// unused parameters
		(void)constructionBitstream;
		(void)destinationConnection;
	}
	virtual bool DeserializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)constructionBitstream;
		(void)sourceConnection;

		return true;
	}
	virtual void SerializeConstructionExisting(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
	{
		// unused parameters
		(void)destinationConnection;

		tmTeam.SerializeConstruction(constructionBitstream);
	}
	virtual void DeserializeConstructionExisting(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)sourceConnection;

		tmTeam.DeserializeConstruction(teamManager, constructionBitstream);
	}
	virtual void SerializeDestruction(SLNet::BitStream *destructionBitstream, SLNet::Connection_RM3 *destinationConnection)
	{
		// unused parameters
		(void)destructionBitstream;
		(void)destinationConnection;
	}
	virtual bool DeserializeDestruction(SLNet::BitStream *destructionBitstream, SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)destructionBitstream;
		(void)sourceConnection;

		return true;
	}
	virtual SLNet::RM3ActionOnPopConnection QueryActionOnPopConnection(SLNet::Connection_RM3 *droppedConnection) const
	{
		// unused parameters
		(void)droppedConnection;

		return RM3AOPC_DO_NOTHING;
	}
	virtual void DeallocReplica(SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)sourceConnection;
	}
	virtual SLNet::RM3QuerySerializationResult QuerySerialization(SLNet::Connection_RM3 *destinationConnection)
	{
		// unused parameters
		(void)destinationConnection;

		if (fullyConnectedMesh2->IsConnectedHost())
			return RM3QSR_CALL_SERIALIZE;
		return RM3QSR_DO_NOT_CALL_SERIALIZE;
	}
	virtual RM3SerializationResult Serialize(SLNet::SerializeParameters *serializeParameters)
	{
		serializeParameters->outputBitstream[0].WriteCompressed(teamName);
		return RM3SR_BROADCAST_IDENTICALLY;
	}
	virtual void Deserialize(SLNet::DeserializeParameters *deserializeParameters)
	{
		deserializeParameters->serializationBitstream[0].ReadCompressed(teamName);
	}

	// The actual team data
	TM_Team tmTeam;

	// Example of user data not managed by TeamManager
	RakString teamName;
};

// User represents a player in the game
// Each system has one user. This user is replicated to other systems.
class User : public Replica3
{
public:
	User()
	{
		tmTeamMember.SetOwner(this);
	}
	virtual ~User()
	{
	}
	virtual void WriteAllocationID(SLNet::Connection_RM3 *destinationConnection, SLNet::BitStream *allocationIdBitstream) const
	{
		// unused parameters
		(void)destinationConnection;

		allocationIdBitstream->Write("User");
	}
	virtual RM3ConstructionState QueryConstruction(SLNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3)
	{
		// unused parameters
		(void)replicaManager3;

		return QueryConstruction_PeerToPeer(destinationConnection);
	}
	virtual bool QueryRemoteConstruction(SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)sourceConnection;

		return true;
	}
	virtual void SerializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
	{
		// unused parameters
		(void)destinationConnection;

		// teamMember must be serialized later than teams. This is accomplished by registering teams first with ReplicaManager3
		tmTeamMember.SerializeConstruction(constructionBitstream);
	}
	virtual bool DeserializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)sourceConnection;

		return tmTeamMember.DeserializeConstruction(teamManager, constructionBitstream);
	}
	virtual void SerializeDestruction(SLNet::BitStream *destructionBitstream, SLNet::Connection_RM3 *destinationConnection)
	{
		// unused parameters
		(void)destructionBitstream;
		(void)destinationConnection;
	}
	virtual bool DeserializeDestruction(SLNet::BitStream *destructionBitstream, SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)destructionBitstream;
		(void)sourceConnection;

		return true;
	}
	virtual SLNet::RM3ActionOnPopConnection QueryActionOnPopConnection(SLNet::Connection_RM3 *droppedConnection) const
	{
		return QueryActionOnPopConnection_PeerToPeer(droppedConnection);
	}
	virtual void DeallocReplica(SLNet::Connection_RM3 *sourceConnection)
	{
		// unused parameters
		(void)sourceConnection;

		delete this;
	}
	virtual SLNet::RM3QuerySerializationResult QuerySerialization(SLNet::Connection_RM3 *destinationConnection)
	{
		return QuerySerialization_PeerToPeer(destinationConnection);
	}
	virtual RM3SerializationResult Serialize(SLNet::SerializeParameters *serializeParameters)
	{
		serializeParameters->outputBitstream[1].Write(userName);
		return RM3SR_BROADCAST_IDENTICALLY;
	}
	virtual void Deserialize(SLNet::DeserializeParameters *deserializeParameters)
	{
		if (deserializeParameters->bitstreamWrittenTo[1])
			deserializeParameters->serializationBitstream[1].Read(userName);
	}

	void PrintTeamStatus(void)
	{
		if (tmTeamMember.GetCurrentTeamCount()==0)
		{
			printf("On 0 teams. noTeamId=%i ", tmTeamMember.GetNoTeamId());
		}
		else
		{
			printf("On %i teams: ", tmTeamMember.GetCurrentTeamCount());
			for (unsigned int i=0; i < tmTeamMember.GetCurrentTeamCount(); i++)
			{
				Team *t = (Team *) tmTeamMember.GetCurrentTeamByIndex(i)->GetOwner();
				printf("%s ", t->teamName.C_String());
			}
		}
		TeamSelection requestedTeam = tmTeamMember.GetRequestedTeam();
		if (requestedTeam.joinTeamType==JOIN_ANY_AVAILABLE_TEAM)
		{
			printf("Requested any available");
		}
		else if (requestedTeam.joinTeamType==JOIN_SPECIFIC_TEAM)
		{
			printf("Requested ");
			Team *t = (Team *) requestedTeam.teamParameter.specificTeamToJoin->GetOwner();
			printf("team %s ", t->teamName.C_String());
		}
		else
		{
			printf("No team requests.");
		}
	}
	// Team data managed by the TeamManager plugin
	TM_TeamMember tmTeamMember;

	// Example of user data not managed by TeamManager
	RakString userName;
};

// Required by ReplicaManager3
class SampleConnectionRM3 : public Connection_RM3
{
public:
	SampleConnectionRM3(const SystemAddress &_systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress, _guid)
	{
	}
	virtual ~SampleConnectionRM3()
	{
	}
	virtual Replica3 *AllocReplica(SLNet::BitStream *allocationIdBitstream, ReplicaManager3 *replicaManager3)
	{
		// unused parameters
		(void)replicaManager3;

		RakString objectType;
		allocationIdBitstream->Read(objectType);
		if (objectType=="User")
			return new User;
		return 0;
	}
};

// Required to derive from ReplicaManager3 as a class factory pattern to create SampleConnectionRM3
class SampleRM3 : public ReplicaManager3
{
public:
	SampleRM3()
	{
	}
	virtual ~SampleRM3()
	{
	}
	virtual Connection_RM3* AllocConnection(const SystemAddress &systemAddress, RakNetGUID rakNetGUID) const
	{
		return new SampleConnectionRM3(systemAddress,rakNetGUID);
	}
	virtual void DeallocConnection(Connection_RM3 *connection) const
	{
		delete connection;
	}
};

// Instance of ReplicaManager3
SampleRM3 *replicaManager3;

// Helper function
// ReplicaManager3 does not add connections until we know who the host is via FullyConnectedMesh2
// This function takes all FullyConnectedMesh2 connections and registers them with ReplicaManager3 once the host is determined.
void RegisterFCM2Participants(void)
{
	if (fullyConnectedMesh2->GetConnectedHost()!=UNASSIGNED_RAKNET_GUID)
	{
		DataStructures::List<RakNetGUID> participantList;
		fullyConnectedMesh2->GetParticipantList(participantList);
		for (unsigned int i=0; i < participantList.Size(); i++)
		{
			Connection_RM3 *connection = replicaManager3->AllocConnection(rakPeer->GetSystemAddressFromGuid(participantList[i]), participantList[i]);
			if (replicaManager3->PushConnection(connection)==false)
				replicaManager3->DeallocConnection(connection);
			teamManager->GetWorldAtIndex(0)->AddParticipant(participantList[i]);
		}
	}
};

enum TeamType
{
	PLAYER_TEAM_1, // Plays game
	PLAYER_TEAM_2, // Plays game
	REFEREE_TEAM, // Only 1 person, only join on specific request
	TEAM_TYPES_COUNT
};

void PrintCommands(void)
{
	printf("A. Request any team\n");
	printf("B. Request specific team\n");
	printf("C. Request team switch\n");
	printf("D. Cancel request team\n");
	printf("E. Leave specific team\n");
	printf("F. Leave all teams\n");
	printf("G. Set team member limit\n");
	printf("H. Turn on balance teams setting\n");
	printf("I. Turn off balance teams setting\n");
	printf("Press space to display state\n");
}

int main(void)
{
	printf("This project demonstrates an in-game lobby using the team manager plugin.\n");
	printf("Difficulty: Intermediate\n\n");

	rakPeer= SLNet::RakPeerInterface::GetInstance();
	fullyConnectedMesh2=FullyConnectedMesh2::GetInstance();
	teamManager=TeamManager::GetInstance();
	networkIDManager = NetworkIDManager::GetInstance();
	replicaManager3=new SampleRM3;

	// test offline mode
	/*
	TM_TeamMember tmTeamMember;
	TM_Team tmTeam;
	teamManager->AddWorld(0);
	teamManager->GetWorldAtIndex(0)->ReferenceTeam(&tmTeam,1,false);
	teamManager->GetWorldAtIndex(0)->ReferenceTeamMember(&tmTeamMember,0);
	tmTeamMember.RequestTeam(TeamSelection::AnyAvailable());
	RakAssert(tmTeam.GetTeamMembersCount()==1);
	tmTeam.LeaveTeam(&tmTeamMember, 255);
	RakAssert(tmTeam.GetTeamMembersCount()==0);
	*/

	rakPeer->AttachPlugin(fullyConnectedMesh2);
	rakPeer->AttachPlugin(teamManager);
	rakPeer->AttachPlugin(replicaManager3);
	rakPeer->AttachPlugin(fullyConnectedMesh2);

	// Make it so all new connections are registered with FullyConnectedMesh2
	fullyConnectedMesh2->SetAutoparticipateConnections(true);
	// Allocate a world instance to be used for team operations
	teamManager->AddWorld(0);
	// Tell ReplicaManager3 which networkIDManager to use for object lookup, used for automatic serialization
	replicaManager3->SetNetworkIDManager(networkIDManager);
	// Tell ReplicaManager3 and TeamManager to not automatically add new connections, because we wait for host calculation to complete from FullyConnectedMesh2 first
	replicaManager3->SetAutoManageConnections(false,true);
	teamManager->SetAutoManageConnections(false);

	// Just setup user data as an example
	Team teams[TEAM_TYPES_COUNT];
	teams[(int)PLAYER_TEAM_1].teamName="Player_Team_1";
	teams[(int)PLAYER_TEAM_2].teamName="Player_Team 2";
	teams[(int)REFEREE_TEAM].teamName="Referee_Team";

	for (unsigned int i=0; i < TEAM_TYPES_COUNT; i++)
	{
		// Static objects require additional setup before calling reference.
		teams[i].SetNetworkIDManager(networkIDManager);
		teams[i].SetNetworkID(i); // NetworkID value doesn't matter, just needs to be unique
		// We serialize teams before team members, this is required by TeamManager during remote object construction. Serialization occurs in the order that Reference() is called on the object
		replicaManager3->Reference(&teams[i]);

		// Register the team with the teamManager plugin
		// Do not apply team balancing to the referee team
		bool balancingAppliesToThisTeam = i!=REFEREE_TEAM;
		teamManager->GetWorldAtIndex(0)->ReferenceTeam(&teams[i].tmTeam,teams[i].GetNetworkID(),balancingAppliesToThisTeam);
		if (i==REFEREE_TEAM)
			teams[i].tmTeam.SetMemberLimit(1,0);
		else
			teams[i].tmTeam.SetMemberLimit(2,0);
	}

	// Only join the referee team on specific request
	teams[REFEREE_TEAM].tmTeam.SetJoinPermissions(ALLOW_JOIN_SPECIFIC_TEAM);

	// Setup my own
	User *user = new User;
	user->userName = rakPeer->GetMyGUID().ToString();

	// Inform ReplicaManager3 of my user
	replicaManager3->Reference(user);
	// Inform TeamManager of my user's team member info
	teamManager->GetWorldAtIndex(0)->ReferenceTeamMember(&user->tmTeamMember,user->GetNetworkID());
	
	// Startup RakNet
	SLNet::SocketDescriptor sd;
	sd.socketFamily=AF_INET; // Only IPV4 supports broadcast on 255.255.255.255
	sd.port=60000;
	while (IRNS2_Berkley::IsPortInUse(sd.port, sd.hostAddress, sd.socketFamily, SOCK_DGRAM)==true)
		sd.port++;
	SLNET_VERIFY(rakPeer->Startup(8, &sd, 1) == RAKNET_STARTED);
	rakPeer->SetMaximumIncomingConnections(8);
	rakPeer->SetTimeoutTime(30000, SLNet::UNASSIGNED_SYSTEM_ADDRESS);
	printf("Our guid is %s\n", rakPeer->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	printf("Started on %s\n", rakPeer->GetMyBoundAddress().ToString(true));

	for (unsigned short i=0; i < 32; i++)
	{
		if (rakPeer->GetInternalID(SLNet::UNASSIGNED_SYSTEM_ADDRESS,0).GetPort()!=60000+i)
			rakPeer->AdvertiseSystem("255.255.255.255", 60000+i, 0,0,0);
	}

	PrintCommands();

	bool success;
	bool quit=false;
	int ch;
	Packet *packet;
	while (!quit)
	{
		for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_DISCONNECTION_NOTIFICATION:
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				{
					printf("ID_NEW_INCOMING_CONNECTION from %s. guid=%s.\n", packet->systemAddress.ToString(true), packet->guid.ToString());
					// Add mid-game joins to ReplicaManager3 as long as we know who the host is
					if (fullyConnectedMesh2->GetConnectedHost()!=UNASSIGNED_RAKNET_GUID)
					{
						SLNET_VERIFY(replicaManager3->PushConnection(replicaManager3->AllocConnection(packet->systemAddress, packet->guid)));
						teamManager->GetWorldAtIndex(0)->AddParticipant(packet->guid);
					}
				}
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					printf("ID_CONNECTION_REQUEST_ACCEPTED from %s. guid=%s.\n", packet->systemAddress.ToString(true), packet->guid.ToString());
					// Add mid-game joins to ReplicaManager3 as long as we know who the host is
					if (fullyConnectedMesh2->GetConnectedHost()!=UNASSIGNED_RAKNET_GUID)
					{
						SLNET_VERIFY(replicaManager3->PushConnection(replicaManager3->AllocConnection(packet->systemAddress, packet->guid)));
						teamManager->GetWorldAtIndex(0)->AddParticipant(packet->guid);
					}
				}
				break;
			case ID_CONNECTION_LOST:
				printf("ID_CONNECTION_LOST\n");
				break;
			case ID_ADVERTISE_SYSTEM:
				if (packet->guid!=rakPeer->GetMyGUID())
					rakPeer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(),0,0);
				break;
			case ID_FCM2_NEW_HOST:
				{
					if (packet->guid==rakPeer->GetMyGUID())
						printf("Got new host (ourselves)");
					else
						printf("Got new host %s, GUID=%s", packet->systemAddress.ToString(true), packet->guid.ToString());
					SLNet::BitStream bs(packet->data,packet->length,false);
					bs.IgnoreBytes(1);
					RakNetGUID oldHost;
					bs.Read(oldHost);
					// If the old host is different, then this message was due to losing connection to the host.
					if (oldHost!=packet->guid)
						printf(". Oldhost Guid=%s\n", oldHost.ToString());
					else
						printf("\n");

					if (oldHost==UNASSIGNED_RAKNET_GUID)
					{
						// First time calculated host. Add existing connections to ReplicaManager3
						RegisterFCM2Participants();
					}
				}
				break;
			case ID_TEAM_BALANCER_TEAM_ASSIGNED:
				{
					printf("ID_TEAM_BALANCER_TEAM_ASSIGNED for ");
					TM_World *world;
					TM_TeamMember *teamMember;
					teamManager->DecodeTeamAssigned(packet, &world, &teamMember);
					printf("worldId=%i teamMember=%s\n", world->GetWorldId(), ((User*)teamMember->GetOwner())->userName.C_String());
				}
				break;
			case ID_TEAM_BALANCER_REQUESTED_TEAM_FULL:
				{
					printf("ID_TEAM_BALANCER_REQUESTED_TEAM_FULL\n");
				}
				break;
			case ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED:
				{
					printf("ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED\n");
				}
				break;
			case ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED:
				{
					printf("ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED\n");
				}
				break;
			}
		}

		if (_kbhit())
		{
			ch=_getch();

			if (ch=='A' || ch=='a')
			{
				printf("Request any team\n");
				success = user->tmTeamMember.RequestTeam(TeamSelection::AnyAvailable());
				printf("Success=%i\n", success);
			}
			if (ch=='B' || ch=='b')
			{
				printf("Request specific team\n");
				char buff1[256];
				printf("Enter team index (0-2): ");
				gets_s(buff1);
				if (buff1[0]!=0 && buff1[0]>='0' && buff1[0]<='2')
				{
					success = user->tmTeamMember.RequestTeam(TeamSelection::SpecificTeam(&(teams[buff1[0]-'0'].tmTeam)));
					printf("Success=%i\n", success);
				}
				else
				{
					printf("Aborted\n");
				}
			}
			if (ch=='C' || ch=='c')
			{
				printf("Request team switch\n");
				char buff1[256];
				printf("Enter team index to join (0-2): ");
				gets_s(buff1);
				char buff2[256];
				printf("Enter team index to leave (0-2) or leave empty for all: ");
				gets_s(buff2);
				if (buff1[0]!=0 && buff1[0]>='0' && buff1[0]<='2' &&
					(buff2[0]==0 || (buff2[0]>='0' && buff2[0]<='2')))
				{
					if (buff2[0])
						success = user->tmTeamMember.RequestTeamSwitch(&(teams[buff1[0]-'0'].tmTeam), &teams[buff2[0]-'0'].tmTeam);
					else
						success = user->tmTeamMember.RequestTeamSwitch(&(teams[buff1[0]-'0'].tmTeam), 0);
					printf("Success=%i\n", success);
				}
				else
				{
					printf("Aborted\n");
				}
			}
			if (ch=='D' || ch=='d')
			{
				printf("Cancel request team\n");
				char buff1[256];
				printf("Enter team index to cancel (0-2) or leave empty for all: ");
				gets_s(buff1);
				if ((buff1[0]!=0 && buff1[0]>='0' && buff1[0]<='2') || buff1[0]==0)
				{
					if (buff1[0])
						success = user->tmTeamMember.CancelTeamRequest(&(teams[buff1[0]-'0'].tmTeam));
					else
						success = user->tmTeamMember.CancelTeamRequest(0);
					printf("Success=%i\n", success);
				}
				else
				{
					printf("Aborted\n");
				}
			}
			if (ch=='E' || ch=='e')
			{
				printf("Leave specific team\n");
				char buff1[256];
				printf("Enter team index to leave (0-2): ");
				gets_s(buff1);
				if (buff1[0]!=0 && buff1[0]>='0' && buff1[0]<='2')
				{
					success = user->tmTeamMember.LeaveTeam(&(teams[buff1[0]-'0'].tmTeam),0);
					printf("Success=%i\n", success);
				}
				else
				{
					printf("Aborted\n");
				}

			}
			if (ch=='F' || ch=='f')
			{
				printf("Leave all teams\n");
				success = user->tmTeamMember.LeaveAllTeams(0);
				printf("Success=%i\n", success);

			}
			if (ch=='G' || ch=='g')
			{
				printf("Set team member limit\n");
				char buff1[256];
				printf("Enter team index to operate on (0-2): ");
				gets_s(buff1);
				char buff2[256];
				printf("Enter limit (0-9): ");
				gets_s(buff2);
				if (buff1[0]!=0 && buff1[0]>='0' && buff1[0]<='2' &&
					buff2[0]!=0 && buff2[0]>='0' && buff2[0]<='9')
				{
					success = teams[buff1[0]-'0'].tmTeam.SetMemberLimit(buff2[0]-'0',0);
					printf("Success=%i\n", success);
				}
				else
				{
					printf("Aborted\n");
				}
			}
			if (ch=='H' || ch=='h')
			{
				printf("Turn on balance teams setting\n");
				success = teamManager->GetWorldAtIndex(0)->SetBalanceTeams(true,0);
				printf("Success=%i\n", success);
			}
			if (ch=='I' || ch=='i')
			{
				printf("Turn off balance teams setting\n");
				success = teamManager->GetWorldAtIndex(0)->SetBalanceTeams(false,0);
				printf("Success=%i\n", success);
			}

			if (ch==' ')
			{
				if (teamManager->GetWorldAtIndex(0)->GetBalanceTeams())
					printf("Team balancing is on\n");
				else
					printf("Team balancing is off\n");

				for (unsigned int i=0; i < TEAM_TYPES_COUNT; i++)
				{
					printf("Team %i. %s %i/%i members ", i+1, teams[i].teamName.C_String(), teams[i].tmTeam.GetTeamMembersCount(), teams[i].tmTeam.GetMemberLimit());
					for (unsigned int j=0; j < teams[i].tmTeam.GetTeamMembersCount(); j++)
					{
						User *u = (User *) teams[i].tmTeam.GetTeamMemberByIndex(j)->GetOwner();
						printf("%s ", u->userName.C_String());
					}
					printf("\n");
				}
		
				unsigned int numUsers = teamManager->GetWorldAtIndex(0)->GetTeamMemberCount();
				for (unsigned int i=0; i < numUsers; i++)
				{
					User *u = (User *) teamManager->GetWorldAtIndex(0)->GetTeamMemberByIndex(i)->GetOwner();
					printf("User %i/%i. %s ", i+1, numUsers, u->userName.C_String());
					u->PrintTeamStatus();
					printf("\n");
				}
				printf("\n");
			}
			else if (ch=='q' || ch=='Q')
			{
				printf("Quitting.\n");
				quit=true;
			}
		}

		RakSleep(30);
	}

	rakPeer->Shutdown(100);
	replicaManager3->Clear();
	SLNet::RakPeerInterface::DestroyInstance(rakPeer);
	delete replicaManager3;
	SLNet::FullyConnectedMesh2::DestroyInstance(fullyConnectedMesh2);
	SLNet::TeamManager::DestroyInstance(teamManager);
	SLNet::NetworkIDManager::DestroyInstance(networkIDManager);

	for (unsigned int i=0; i < TEAM_TYPES_COUNT; i++)
	{
		// Teams are globally deallocated after NetworkIDManager, so prevent crash on automatic dereference
		teams[i].SetNetworkIDManager(0);
	}

	return 1;
}

