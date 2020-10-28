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

#include "CloudServerHelper.h"
#include "slikenet/sleep.h"

#include "slikenet/Gets.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/BitStream.h"
#include "slikenet/FullyConnectedMesh2.h"
#include "slikenet/TwoWayAuthentication.h"
#include "slikenet/CloudClient.h"
#include "slikenet/DynDNS.h"
#include "slikenet/SocketLayer.h"
#include "slikenet/peerinterface.h"
#include "slikenet/ConnectionGraph2.h"
#include <stdlib.h>
#include <limits> // used for std::numeric_limits
#include "slikenet/linux_adapter.h"
#include "slikenet/osx_adapter.h"

using namespace SLNet;
#define CLOUD_SERVER_CONNECTION_COUNT_PRIMARY_KEY "CloudConnCount"

bool CloudServerHelperFilter::OnPostRequest(RakNetGUID clientGuid, SystemAddress clientAddress, CloudKey key, uint32_t dataLength, const char *data)
{
	// unused parameters
	(void)clientAddress;
	(void)dataLength;
	(void)data;

	if (clientGuid!=serverGuid)
	{
		if (key.primaryKey==CLOUD_SERVER_CONNECTION_COUNT_PRIMARY_KEY)
			return false;
	}
	return true;
}
bool CloudServerHelperFilter::OnReleaseRequest(RakNetGUID clientGuid, SystemAddress clientAddress, DataStructures::List<CloudKey> &cloudKeys)
{
	// unused parameters
	(void)clientGuid;
	(void)clientAddress;
	(void)cloudKeys;

	return true;
}
bool CloudServerHelperFilter::OnGetRequest(RakNetGUID clientGuid, SystemAddress clientAddress, CloudQuery &query, DataStructures::List<RakNetGUID> &specificSystems)
{
	// unused parameters
	(void)clientGuid;
	(void)clientAddress;
	(void)query;
	(void)specificSystems;

	return true;
}
bool CloudServerHelperFilter::OnUnsubscribeRequest(RakNetGUID clientGuid, SystemAddress clientAddress, DataStructures::List<CloudKey> &cloudKeys, DataStructures::List<RakNetGUID> &specificSystems)
{
	// unused parameters
	(void)clientGuid;
	(void)clientAddress;
	(void)cloudKeys;
	(void)specificSystems;

	return true;
}

bool CloudServerHelper::ParseCommandLineParameters(int argc, char **argv)
{
	char *DEFAULT_SERVER_TO_SERVER_PASSWORD="qwerty1234";
	const unsigned short DEFAULT_SERVER_PORT=60000;
	const unsigned short DEFAULT_ALLOWED_INCOMING_CONNECTIONS=1024;
	const unsigned short DEFAULT_ALLOWED_OUTGOING_CONNECTIONS=64;

	if (argc<2) serverToServerPassword=DEFAULT_SERVER_TO_SERVER_PASSWORD;
	else serverToServerPassword=argv[1];

	if (argc<3) rakPeerPort=DEFAULT_SERVER_PORT;
	else {
		const int intPeerPort = atoi(argv[2]);
		if ((intPeerPort < 0) || (intPeerPort > std::numeric_limits<unsigned short>::max())) {
			printf("Specified peer port %d is outside valid bounds [0, %u]", intPeerPort, std::numeric_limits<unsigned short>::max());
			return false;
		}
		rakPeerPort = static_cast<unsigned short>(rakPeerPort);
	}

	if (argc<4) allowedIncomingConnections=DEFAULT_ALLOWED_INCOMING_CONNECTIONS;
	else {
		const int intConnections = atoi(argv[3]);
		if ((intConnections < 0) || (intConnections > std::numeric_limits<unsigned short>::max())) {
			printf("Specified allowed incoming connections %d is outside valid bounds [0, %u]", intConnections, std::numeric_limits<unsigned short>::max());
			return false;
		}
		allowedIncomingConnections = static_cast<unsigned short>(intConnections);
	}

	if (argc<5) allowedOutgoingConnections=DEFAULT_ALLOWED_OUTGOING_CONNECTIONS;
	else {
		const int intConnections = atoi(argv[4]);
		if ((intConnections < 0) || (intConnections > std::numeric_limits<unsigned short>::max())) {
			printf("Specified allowed outgoing connections %d is outside valid bounds [0, %u]", intConnections, std::numeric_limits<unsigned short>::max());
			return false;
		}
		allowedOutgoingConnections = static_cast<unsigned short>(intConnections);
	}

	return true;
}

bool CloudServerHelper_DynDns::ParseCommandLineParameters(int argc, char **argv)
{
	const unsigned short DEFAULT_SERVER_PORT=60000;
	const unsigned short DEFAULT_ALLOWED_INCOMING_CONNECTIONS=1024;
	const unsigned short DEFAULT_ALLOWED_OUTGOING_CONNECTIONS=64;
#ifdef _DEBUG
	char *DEFAULT_DNS_HOST = "test.dnsalias.net";
	char *DEFAULT_USERNAME_AND_PASSWORD = "test:test";
	char *DEFAULT_SERVER_TO_SERVER_PASSWORD = "qwerty1234";
#endif

#ifndef _DEBUG
	// Only allow insecure defaults for debugging
	if (argc<4)
	{
		PrintHelp();
		return false;
	}
	dnsHost=argv[1];
	dynDNSUsernameAndPassword=argv[2];
	serverToServerPassword=argv[3];
#else
	if (argc<2) dnsHost=DEFAULT_DNS_HOST;
	else dnsHost=argv[1];

	if (argc<3) dynDNSUsernameAndPassword=DEFAULT_USERNAME_AND_PASSWORD;
	else dynDNSUsernameAndPassword=argv[2];

	if (argc<4) serverToServerPassword=DEFAULT_SERVER_TO_SERVER_PASSWORD;
	else serverToServerPassword=argv[3];
#endif

	if (argc<5) rakPeerPort=DEFAULT_SERVER_PORT;
	else {
		const int intPeerPort = atoi(argv[4]);
		if ((intPeerPort < 0) || (intPeerPort > std::numeric_limits<unsigned short>::max())) {
			printf("Specified peer port %d is outside valid bounds [0, %u]", intPeerPort, std::numeric_limits<unsigned short>::max());
			return false;
		}
		rakPeerPort = static_cast<unsigned short>(rakPeerPort);
	}

	if (argc<6) allowedIncomingConnections=DEFAULT_ALLOWED_INCOMING_CONNECTIONS;
	else {
		const int intConnections = atoi(argv[5]);
		if ((intConnections < 0) || (intConnections > std::numeric_limits<unsigned short>::max())) {
			printf("Specified allowed incoming connections %d is outside valid bounds [0, %u]", intConnections, std::numeric_limits<unsigned short>::max());
			return false;
		}
		allowedIncomingConnections = static_cast<unsigned short>(intConnections);
	}

	if (argc<7) allowedOutgoingConnections=DEFAULT_ALLOWED_OUTGOING_CONNECTIONS;
	else {
		const int intConnections = atoi(argv[6]);
		if ((intConnections < 0) || (intConnections > std::numeric_limits<unsigned short>::max())) {
			printf("Specified allowed outgoing connections %d is outside valid bounds [0, %u]", intConnections, std::numeric_limits<unsigned short>::max());
			return false;
		}
		allowedOutgoingConnections = static_cast<unsigned short>(intConnections);
	}

	return true;
}

void CloudServerHelper::PrintHelp(void)
{
	printf("Distributed authenticated CloudServer using DNS based host migration.\n");
	printf("Query running servers with CloudClient::Get() on key CloudServerList,0\n\n");
	printf("Query load with key CloudConnCount,0. Read row data as unsigned short.\n\n");
	printf("Usage:\n");
	printf("CloudServer.exe DNSHost Username:Password S2SPWD [Port] [ConnIn] [ConnOut]\n\n");
	printf("Parameters:\n");
	printf("DNSHost - Free DNS hostname from http://www.dyndns.com/\n");
	printf("Username:Password - Account settings from http://www.dyndns.com/\n");
	printf("S2SPWD - Server to server cloud password. Anything random.\n");
	printf("Port - RakNet listen port. Default is 60000\n");
	printf("ConnIn - Max incoming connections for clients. Default is 1024\n");
	printf("ConnIn - Max outgoing connections, used for server to server. Default 64\n\n");
	printf("Example:\n");
	printf("CloudServer.exe test.dnsalias.net test:test qwerty1234 60000 1024 64\n\n");
}

bool CloudServerHelper::StartRakPeer(SLNet::RakPeerInterface *rakPeer)
{
	SLNet::SocketDescriptor sd(SLNet::CloudServerHelper::rakPeerPort,0);
	SLNet::StartupResult sr = rakPeer->Startup(SLNet::CloudServerHelper::allowedIncomingConnections+ SLNet::CloudServerHelper::allowedOutgoingConnections,&sd,1);
	if (sr!= SLNet::RAKNET_STARTED)
	{
		printf("Startup failed. Reason=%i\n", (int) sr);
		return false;
	}
	rakPeer->SetMaximumIncomingConnections(SLNet::CloudServerHelper::allowedIncomingConnections);
	//rakPeer->SetTimeoutTime(60000,UNASSIGNED_SYSTEM_ADDRESS);
	return true;
}

Packet *CloudServerHelper::ConnectToRakPeer(const char *host, unsigned short port, RakPeerInterface *rakPeer)
{
	printf("RakPeer: Connecting to %s\n", host);
	ConnectionAttemptResult car;
	car = rakPeer->Connect(host, port, 0, 0);
	if (car!=CONNECTION_ATTEMPT_STARTED)
	{
		printf("Connect() call failed\n");
		if (car==CANNOT_RESOLVE_DOMAIN_NAME) printf("Cannot resolve domain name\n");
		return 0;
	}

	Packet *packet;
	for(;;)
	{
		for (packet=rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet=rakPeer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_CONNECTION_ATTEMPT_FAILED:
				return packet;
			case ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY:
			case ID_OUR_SYSTEM_REQUIRES_SECURITY:
			case ID_PUBLIC_KEY_MISMATCH:
			case ID_CONNECTION_BANNED:
			case ID_INVALID_PASSWORD:
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				return packet;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			case ID_IP_RECENTLY_CONNECTED:
				printf("Remote system full. Retrying...");
				car = rakPeer->Connect(host, port, 0, 0);
				if (car!=CONNECTION_ATTEMPT_STARTED)
				{
					printf("Connect() call failed\n");
					if (car==CANNOT_RESOLVE_DOMAIN_NAME) printf("Cannot resolve domain name\n");
					return 0;
				}
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				if (packet->guid==rakPeer->GetMyGUID())
				{
					// Just connected to myself! Host must be pointing to our own IP address.
					rakPeer->CloseConnection(packet->guid,false);
					RakSleep(30); // Let the thread clear out
					packet->data[0]=ID_ALREADY_CONNECTED;

					return packet;
				}
			case ID_ALREADY_CONNECTED:
				return packet; // Not initial host
			}
		}

		RakSleep(30);
	}
}
bool CloudServerHelper_DynDns::SetHostDNSToThisSystemBlocking(void)
{
	dynDNS->UpdateHostIPAsynch(
		dnsHost,
		0,
		dynDNSUsernameAndPassword);

	// Wait for the DNS update to complete
	do
	{
		dynDNS->Update();

		if (dynDNS->IsCompleted())
		{
			printf("%s\n", dynDNS->GetCompletedDescription());
			break;
		}

		RakSleep(30);
	} while (!dynDNS->IsCompleted());

	return dynDNS->WasResultSuccessful();
}

MessageID CloudServerHelper::AuthenticateRemoteServerBlocking(RakPeerInterface *rakPeer, TwoWayAuthentication *twoWayAuthentication, RakNetGUID remoteSystem)
{
	twoWayAuthentication->Challenge("CloudServerHelperS2SPassword", remoteSystem);

	MessageID messageId;
	Packet *packet;
	for(;;)
	{
		for (packet=rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet=rakPeer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION:
				if (packet->guid==remoteSystem)
				{
					messageId=packet->data[0];
					rakPeer->DeallocatePacket(packet);
					return messageId;
				}
				break;
			case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS:
			case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT:
			case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE:
				{
					messageId=packet->data[0];
					rakPeer->DeallocatePacket(packet);
					return messageId;
				}
				break;
			}
		}			

		RakSleep(30);
	}
}
void CloudServerHelper::SetupPlugins(
	SLNet::CloudServer *cloudServer,
	SLNet::CloudServerHelperFilter *sampleFilter,
	SLNet::CloudClient *cloudClient,
	SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
	SLNet::TwoWayAuthentication *twoWayAuthentication,
	SLNet::ConnectionGraph2 *connectionGraph2,
	const char *newServerToServerPassword
	)
{
	// unused parameters
	(void)cloudClient;

	cloudServer->AddQueryFilter(sampleFilter);
	// Connect to all systems told about via ConnectionGraph2::AddParticpant(). We are only told about servers that have already been authenticated
	fullyConnectedMesh2->SetConnectOnNewRemoteConnection(true, "");
	// Do not add to the host trracking system all connections, only those designated as servers
	fullyConnectedMesh2->SetAutoparticipateConnections(false);
	// Shared password
	twoWayAuthentication->AddPassword("CloudServerHelperS2SPassword", newServerToServerPassword);
	// Do not add systems to the graph unless first validated as a server through the TwoWayAuthentication plugin
	connectionGraph2->SetAutoProcessNewConnections(false);
}
void CloudServerHelper::OnPacket(Packet *packet, RakPeerInterface *rakPeer, CloudClient *cloudClient, SLNet::CloudServer *cloudServer, SLNet::FullyConnectedMesh2 *fullyConnectedMesh2, TwoWayAuthentication *twoWayAuthentication, ConnectionGraph2 *connectionGraph2)
{
	switch (packet->data[0])
	{
	case ID_FCM2_NEW_HOST:
		SLNet::CloudServerHelper::OnFCMNewHost(packet, rakPeer);
		break;
	case ID_CONNECTION_REQUEST_ACCEPTED:
		twoWayAuthentication->Challenge("CloudServerHelperS2SPassword", packet->guid);
		// Fallthrough
	case ID_NEW_INCOMING_CONNECTION:
		printf("Got connection to %s\n", packet->systemAddress.ToString(true));
		SLNet::CloudServerHelper::OnConnectionCountChange(rakPeer, cloudClient);
		break;
	case ID_CONNECTION_LOST:
	//	printf("ID_CONNECTION_LOST (UDP) from %s\n", packet->systemAddress.ToString(true));
		SLNet::CloudServerHelper::OnConnectionCountChange(rakPeer, cloudClient);
		break;
	case ID_DISCONNECTION_NOTIFICATION:
	//	printf("ID_DISCONNECTION_NOTIFICATION (UDP) from %s\n", packet->systemAddress.ToString(true));
		SLNet::CloudServerHelper::OnConnectionCountChange(rakPeer, cloudClient);
		break;
	case ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS:
		printf("New server connected to us from %s\n", packet->systemAddress.ToString(true));
		// Fallthrough
	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS:
		if (packet->data[0]==ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS)
			printf("We connected to server %s\n", packet->systemAddress.ToString(true));
		cloudServer->AddServer(packet->guid);
		fullyConnectedMesh2->AddParticipant(packet->guid);
		connectionGraph2->AddParticipant(packet->systemAddress, packet->guid);
		break;
	case ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE:
	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE:
	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT:
		rakPeer->CloseConnection(packet->guid,false);
		break;
	}
}
bool CloudServerHelper::Update(void)
{
	return true;
}
CloudServerHelper_DynDns::CloudServerHelper_DynDns(DynDNS *_dynDns)
{
	dynDNS = _dynDns;
}
bool CloudServerHelper_DynDns::Update(void)
{
	// Keep DNS updated if needed
	if (dynDNS->IsRunning())
	{
		dynDNS->Update();
		if (dynDNS->IsCompleted())
		{
			printf("%s.\n", dynDNS->GetCompletedDescription());
			if (dynDNS->WasResultSuccessful()==false)
				return false;
			printf("Note: The DNS cache update takes about 60 seconds.\n");
		}
	}
	return true;
}
void CloudServerHelper::OnFCMNewHost(Packet *packet, RakPeerInterface *rakPeer)
{
	// unused parameters
	(void)packet;
	(void)rakPeer;
}
void CloudServerHelper_DynDns::OnFCMNewHost(Packet *packet, RakPeerInterface *rakPeer)
{
	RakAssert(packet->data[0]==ID_FCM2_NEW_HOST);
	SLNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(MessageID));
	RakNetGUID oldHost;
	bsIn.Read(oldHost);
	RakNetGUID newHost = packet->guid;
	if (newHost==rakPeer->GetMyGUID() && oldHost!=newHost)
	{
		printf("Assuming host. Updating DNS\n");

		// Change dynDNS to point to us
		dynDNS->UpdateHostIPAsynch(
			dnsHost,
			0,
			dynDNSUsernameAndPassword);
	}
}
void CloudServerHelper::OnConnectionCountChange(RakPeerInterface *rakPeer, CloudClient *cloudClient)
{
	SLNet::BitStream bs;
	CloudKey cloudKey(CLOUD_SERVER_CONNECTION_COUNT_PRIMARY_KEY,0);
	unsigned short numberOfSystems;
	rakPeer->GetConnectionList(0, &numberOfSystems);
	bs.Write(numberOfSystems);
	cloudClient->Post(&cloudKey, bs.GetData(), bs.GetNumberOfBytesUsed(), rakPeer->GetMyGUID());
}
int CloudServerHelper_DynDns::OnJoinCloudResult(
	Packet *packet,
	SLNet::RakPeerInterface *rakPeer,
	SLNet::CloudServer *cloudServer,
	SLNet::CloudClient *cloudClient,
	SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
	SLNet::TwoWayAuthentication *twoWayAuthentication,
	SLNet::ConnectionGraph2 *connectionGraph2,
	const char *rakPeerIpOrDomain,
	char myPublicIP[32]
	)
{
	if (packet->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
	{
		printf("Failed connection. Changing DNS to point to this system.\n");

		if (SetHostDNSToThisSystemBlocking()==false)
			return 1;

		// dynDNS gets our public IP when it succeeds
		strcpy_s( myPublicIP, 32, dynDNS->GetMyPublicIP());
	}

	return CloudServerHelper::OnJoinCloudResult(packet, rakPeer, cloudServer, cloudClient, fullyConnectedMesh2, twoWayAuthentication, connectionGraph2, rakPeerIpOrDomain, myPublicIP);
}
int CloudServerHelper::OnJoinCloudResult(
							  Packet *packet,
							  SLNet::RakPeerInterface *rakPeer,
							  SLNet::CloudServer *cloudServer,
							  SLNet::CloudClient *cloudClient,
							  SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
							  SLNet::TwoWayAuthentication *twoWayAuthentication,
							  SLNet::ConnectionGraph2 *connectionGraph2,
							  const char *rakPeerIpOrDomain,
							  char myPublicIP[32]
							  )
{

	SLNet::MessageID result;
	SystemAddress packetAddress;
	RakNetGUID packetGuid;
	result = packet->data[0];
	packetAddress = packet->systemAddress;
	packetGuid = packet->guid;

	if (result==ID_CONNECTION_REQUEST_ACCEPTED)
	{
		printf("Connected to host %s.\n", rakPeerIpOrDomain);

		// We connected through a public IP.
		// Our external IP should also be public
		// rakPeer->GetExternalID(packetAddress).ToString(false, myPublicIP);

		// Log in to the remote server using two way authentication
		result = SLNet::CloudServerHelper::AuthenticateRemoteServerBlocking(rakPeer, twoWayAuthentication, packetGuid);
		if (result==ID_CONNECTION_LOST || result==ID_DISCONNECTION_NOTIFICATION)
		{
			printf("Connection lost while authenticating.\n");
			printf("Waiting 60 seconds then restarting.\n");
			RakSleep(60000);
			return 2;
		}
		else if (result==ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT)
		{
			// Other system is not running plugin? Fail
			printf("Remote server did not respond to challenge.\n");
			return 1;
		}
		else if (result==ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE)
		{
			printf("Failed remote server challenge.\n");
			return 1;
		}

		RakAssert(result==ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS);

		// Add this system as a server, and to FullyConnectedMesh2 as a participant
		cloudServer->AddServer(packetGuid);
		fullyConnectedMesh2->AddParticipant(packetGuid);
		connectionGraph2->AddParticipant(packetAddress, packetGuid);
	}
	else if (result==ID_ALREADY_CONNECTED)
	{
		printf("Connected to self. DNS entry already points to this server.\n");

		/*
		if (SetHostDNSToThisSystemBlocking()==false)
		return 1;

		// dynDNS gets our public IP when it succeeds
		strcpy_s( myPublicIP, dynDNS->GetMyPublicIP());
		*/

		// dnsHost is always public, so if I can connect through it that's my public IP
		RakNetSocket2::DomainNameToIP( rakPeerIpOrDomain, myPublicIP );
	}
	else if (result==ID_CONNECTION_ATTEMPT_FAILED)
	{
		
	}
	else
	{
		// Another server is running but we cannot connect to them
		printf("Critical failure\n");
		printf("Reason: ");
		switch (result)
		{
		case ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY:
		case ID_OUR_SYSTEM_REQUIRES_SECURITY:
		case ID_PUBLIC_KEY_MISMATCH:
			printf("Other system is running security code.\n");
			break;
		case ID_CONNECTION_BANNED:
			printf("Banned from the other system.\n");
			break;
		case ID_INVALID_PASSWORD:
			printf("Other system has a password.\n");
			break;
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("Different major RakNet version.\n");
			break;
		default:
			printf("N/A\n");
			break;
		}
		return 1;
	}

	// Force the external server address for queries. Otherwise it would report 127.0.0.1 since the client is on localhost
	SystemAddress forceAddress;
	forceAddress.FromStringExplicitPort(myPublicIP, SLNet::CloudServerHelper::rakPeerPort);
	cloudServer->ForceExternalSystemAddress(forceAddress);

	if (result==ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS)
	{
		OnConnectionCountChange(rakPeer, cloudClient);
	}
	else
	{
		SLNet::BitStream bs;
		CloudKey cloudKey(CLOUD_SERVER_CONNECTION_COUNT_PRIMARY_KEY,0);
		bs.WriteCasted<unsigned short>(0);
		cloudClient->Post(&cloudKey, bs.GetData(), bs.GetNumberOfBytesUsed(), rakPeer->GetMyGUID());
	}
	return 0;
}
int CloudServerHelper::JoinCloud(
	SLNet::RakPeerInterface *rakPeer,
	SLNet::CloudServer *cloudServer,
	SLNet::CloudClient *cloudClient,
	SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
	SLNet::TwoWayAuthentication *twoWayAuthentication,
	SLNet::ConnectionGraph2 *connectionGraph2,
	const char *rakPeerIpOrDomain
	)
{

	Packet *packet;
	char myPublicIP[32];

	// Reset plugins
	cloudServer->Clear();
	fullyConnectedMesh2->Clear();

	// ---- CONNECT TO EXISTING SERVER ----
	packet = SLNet::CloudServerHelper::ConnectToRakPeer(rakPeerIpOrDomain, SLNet::CloudServerHelper::rakPeerPort, rakPeer);
	if (packet==0)
		return 1;
	

	int res = OnJoinCloudResult(packet, rakPeer, cloudServer, cloudClient, fullyConnectedMesh2, twoWayAuthentication, connectionGraph2, rakPeerIpOrDomain, myPublicIP);
	rakPeer->DeallocatePacket(packet);
	return res;
}
