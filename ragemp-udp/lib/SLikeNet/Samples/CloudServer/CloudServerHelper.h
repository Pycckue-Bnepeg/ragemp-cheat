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

#ifndef __CLOUD_SERVER_HELPER_H
#define __CLOUD_SERVER_HELPER_H

#include "slikenet/CloudServer.h"

namespace SLNet
{

class DynDNS;
class CloudClient;
class TwoWayAuthentication;
class FullyConnectedMesh2;
class ConnectionGraph2;

class CloudServerHelperFilter : public CloudServerQueryFilter
{
public:
	virtual bool OnPostRequest(RakNetGUID clientGuid, SystemAddress clientAddress, CloudKey key, uint32_t dataLength, const char *data);
	virtual bool OnReleaseRequest(RakNetGUID clientGuid, SystemAddress clientAddress, DataStructures::List<CloudKey> &cloudKeys);
	virtual bool OnGetRequest(RakNetGUID clientGuid, SystemAddress clientAddress, CloudQuery &query, DataStructures::List<RakNetGUID> &specificSystems);
	virtual bool OnUnsubscribeRequest(RakNetGUID clientGuid, SystemAddress clientAddress, DataStructures::List<CloudKey> &cloudKeys, DataStructures::List<RakNetGUID> &specificSystems);

	RakNetGUID serverGuid;
};

// To use this class without DynDNS, you only need the CloudServer class. and CloudServerHelperFilter
// The only function you need is CloudServerHelper::OnPacket() for CloudServerHelper::OnConnectionCountChange
// For setup, call cloudServer->AddQueryFilter(sampleFilter);
struct CloudServerHelper
{
	char *serverToServerPassword;
	unsigned short rakPeerPort;
	// #med - consider changing to (unsigned?) int
	unsigned short allowedIncomingConnections;
	unsigned short allowedOutgoingConnections;

	virtual void OnPacket(Packet *packet, RakPeerInterface *rakPeer, CloudClient *cloudClient, SLNet::CloudServer *cloudServer, SLNet::FullyConnectedMesh2 *fullyConnectedMesh2, TwoWayAuthentication *twoWayAuthentication, ConnectionGraph2 *connectionGraph2);
	virtual bool Update(void);
	virtual bool ParseCommandLineParameters(int argc, char **argv);
	virtual void PrintHelp(void);
	bool StartRakPeer(SLNet::RakPeerInterface *rakPeer);
	Packet *ConnectToRakPeer(const char *host, unsigned short port, RakPeerInterface *rakPeer);
	MessageID AuthenticateRemoteServerBlocking(RakPeerInterface *rakPeer, TwoWayAuthentication *twoWayAuthentication, RakNetGUID remoteSystem);
	void SetupPlugins(
		SLNet::CloudServer *cloudServer,
		SLNet::CloudServerHelperFilter *sampleFilter,
		SLNet::CloudClient *cloudClient,
		SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
		SLNet::TwoWayAuthentication *twoWayAuthentication,
		SLNet::ConnectionGraph2 *connectionGraph2,
		const char *newServerToServerPassword
		);

	int JoinCloud(
		SLNet::RakPeerInterface *rakPeer,
		SLNet::CloudServer *cloudServer,
		SLNet::CloudClient *cloudClient,
		SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
		SLNet::TwoWayAuthentication *twoWayAuthentication,
		SLNet::ConnectionGraph2 *connectionGraph2,
		const char *rakPeerIpOrDomain
		);


	// Call when the number of client connections change
	// Usually internal
	virtual void OnConnectionCountChange(RakPeerInterface *rakPeer, CloudClient *cloudClient);
protected:
	// Call when you get ID_FCM2_NEW_HOST
	virtual void OnFCMNewHost(Packet *packet, RakPeerInterface *rakPeer);


	virtual int OnJoinCloudResult(
		Packet *packet,
		SLNet::RakPeerInterface *rakPeer,
		SLNet::CloudServer *cloudServer,
		SLNet::CloudClient *cloudClient,
		SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
		SLNet::TwoWayAuthentication *twoWayAuthentication,
		SLNet::ConnectionGraph2 *connectionGraph2,
		const char *rakPeerIpOrDomain,
		char myPublicIP[32]
		);
};

struct CloudServerHelper_DynDns : public CloudServerHelper
{
public:
	CloudServerHelper_DynDns(DynDNS *_dynDns);

	// Returns false on DNS update failure
	virtual bool Update(void);
	virtual bool SetHostDNSToThisSystemBlocking(void);
	virtual bool ParseCommandLineParameters(int argc, char **argv);
protected:
	DynDNS *dynDNS;
	char *dynDNSUsernameAndPassword;
	char *dnsHost;

	// Call when you get ID_FCM2_NEW_HOST
	virtual void OnFCMNewHost(Packet *packet, RakPeerInterface *rakPeer);

	virtual int OnJoinCloudResult(
		Packet *packet,
		SLNet::RakPeerInterface *rakPeer,
		SLNet::CloudServer *cloudServer,
		SLNet::CloudClient *cloudClient,
		SLNet::FullyConnectedMesh2 *fullyConnectedMesh2,
		SLNet::TwoWayAuthentication *twoWayAuthentication,
		SLNet::ConnectionGraph2 *connectionGraph2,
		const char *rakPeerIpOrDomain,
		char myPublicIP[32]
		);
};

} // namespace SLNet

#endif // __CLOUD_SERVER_HELPER_H
