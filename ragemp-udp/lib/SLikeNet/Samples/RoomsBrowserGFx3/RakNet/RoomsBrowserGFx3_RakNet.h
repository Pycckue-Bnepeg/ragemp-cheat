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

#ifndef __ROOMS_BROWSER_GFX3_RAKNET
#define __ROOMS_BROWSER_GFX3_RAKNET

#include "slikenet/WindowsIncludes.h"
#include "slikenet/types.h"
#include "Lobby2Message.h"
#include "slikenet/PluginInterface2.h"
#include "RoomsBrowserGFx3.h"
#include "RoomsPlugin.h"

namespace SLNet {

/// This is data that should be set with RakPeerInterface::SetOfflinePingResponse()
/// It should also be able to write this data to a LobbyServer instance
struct ServerAndRoomBrowserData
{
	unsigned short numPlayers;
	unsigned short maxPlayers;
	SLNet::RakString mapName;
	SLNet::RakString roomName;
	SLNet::RakNetGUID roomId;
	
	/// For the LAN browser, it expects the offline ping response to be packed using the format from this structure
	/// Therefore, to advertise that your server is available or updated, fill out the parameters in this structure, then call SetAsOfflinePingResponse()
	/// When your server is no longer available, set the offline ping response to NULL
	void SetAsOfflinePingResponse(SLNet::RakPeerInterface *rakPeer);

	/// Write to a RakNet table data structure, useful when creating the room or setting the room properties
	/// Writes everything EXCEPT the list of players
	void WriteToTable(DataStructures::Table *table);

	/// \internal
	void Serialize(SLNet::BitStream *bitStream, bool writeToBitstream);
};

// GFxPlayerTinyD3D9.cpp has an instance of this class, and callls the corresponding 3 function
// This keeps the patching code out of the GFx sample as much as possible
class RoomsBrowserGFx3_RakNet : public RoomsBrowserGFx3, public SLNet::Lobby2Callbacks, public PluginInterface2, SLNet::RoomsCallback
{
public:
	RoomsBrowserGFx3_RakNet();
	virtual ~RoomsBrowserGFx3_RakNet();
	virtual void Init(SLNet::Lobby2Client *_lobby2Client,
		SLNet::Lobby2MessageFactory *_messageFactory,
		RakPeerInterface *_rakPeer, 
		SLNet::RoomsPlugin *_roomsPlugin,
		SLNet::RakString _titleName,
		SLNet::RakString _titleSecretKey,
		SLNet::RakString _pathToXMLPropertyFile,
		unsigned short _lanServerPort,
		GPtr<FxDelegate> pDelegate,
		GPtr<GFxMovieView> pMovie);

	void Update(void);
	void Shutdown(void);

	// Update all callbacks from flash
	void                Accept(CallbackProcessor* cbreg);

	virtual const char *QueryPlatform(void) const {return "RakNet";}
	virtual void SaveProperty(const char *propertyId, const char *propertyValue);
	virtual void LoadProperty(const char *propertyId, SLNet::RakString &propertyOut);

	ACTIONSCRIPT_CALLABLE_HEADER(f2c_ConnectToServer);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Login);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_RegisterAccount);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_UpdateRoomsList);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_UpdateFriendsList);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_JoinByFilter);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_CreateRoom);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Friends_SendInvite);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Friends_Remove);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Friends_AcceptInvite);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Friends_RejectInvite);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Directed_Chat_Func);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Room_Chat_Func);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_Logoff);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_LeaveRoom);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_SendInvite);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_StartSpectating);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_StopSpectating);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_GrantModerator);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_SetReadyStatus);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_GetReadyStatus);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_SetRoomLockState);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_GetRoomLockState);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_AreAllMembersReady);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_KickMember);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_GetRoomProperties);
	ACTIONSCRIPT_CALLABLE_HEADER(f2c_StartGame);

	// Lobby2
	virtual void MessageResult(SLNet::Client_Login *message);
	virtual void MessageResult(SLNet::Client_Logoff *message);
	virtual void MessageResult(SLNet::Client_RegisterAccount *message);
	virtual void MessageResult(SLNet::Friends_GetFriends *message);
	virtual void MessageResult(SLNet::Friends_GetInvites *message);
	virtual void MessageResult(SLNet::Friends_SendInvite *message);
	virtual void MessageResult(SLNet::Friends_AcceptInvite *message);
	virtual void MessageResult(SLNet::Friends_RejectInvite *message);
	virtual void MessageResult(SLNet::Friends_Remove *message);
	virtual void MessageResult(SLNet::Notification_Friends_PresenceUpdate *message);
	virtual void MessageResult(SLNet::Notification_Friends_StatusChange *message);

	// PluginInterface2
	virtual void OnClosedConnection(const SystemAddress &systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason );
	virtual void OnNewConnection(const SystemAddress &systemAddress, RakNetGUID rakNetGUID, bool isIncoming);
	virtual void OnFailedConnectionAttempt(Packet *packet, PI2_FailedConnectionAttemptReason failedConnectionAttemptReason);
	virtual PluginReceiveResult OnReceive(Packet *packet);

	/// Rooms callbacks
	virtual void CreateRoom_Callback( const SystemAddress &senderAddress, SLNet::CreateRoom_Func *callResult);
	virtual void SearchByFilter_Callback( const SystemAddress &senderAddress, SLNet::SearchByFilter_Func *callResult);
	virtual void JoinByFilter_Callback( const SystemAddress &senderAddress, SLNet::JoinByFilter_Func *callResult);
	virtual void Chat_Callback( const SystemAddress &senderAddress, SLNet::Chat_Func *callResult);
	virtual void Chat_Callback( const SystemAddress &senderAddress, SLNet::Chat_Notification *notification);
	virtual void LeaveRoom_Callback( const SystemAddress &senderAddress, SLNet::LeaveRoom_Func *callResult);
	virtual void SendInvite_Callback( const SystemAddress &senderAddress, SLNet::SendInvite_Func *callResult);
	virtual void StartSpectating_Callback( const SystemAddress &senderAddress, SLNet::StartSpectating_Func *callResult);
	virtual void StopSpectating_Callback( const SystemAddress &senderAddress, SLNet::StopSpectating_Func *callResult);
	virtual void GrantModerator_Callback( const SystemAddress &senderAddress, SLNet::GrantModerator_Func *callResult);
	virtual void SetReadyStatus_Callback( const SystemAddress &senderAddress, SLNet::SetReadyStatus_Func *callResult);
	virtual void GetReadyStatus_Callback( const SystemAddress &senderAddress, SLNet::GetReadyStatus_Func *callResult);
	virtual void SetRoomLockState_Callback( const SystemAddress &senderAddress, SLNet::SetRoomLockState_Func *callResult);
	virtual void GetRoomLockState_Callback( const SystemAddress &senderAddress, SLNet::GetRoomLockState_Func *callResult);
	virtual void AreAllMembersReady_Callback( const SystemAddress &senderAddress, SLNet::AreAllMembersReady_Func *callResult);
	virtual void KickMember_Callback( const SystemAddress &senderAddress, SLNet::KickMember_Func *callResult);
	virtual void GetRoomProperties_Callback( const SystemAddress &senderAddress, SLNet::GetRoomProperties_Func *callResult);

	// Notifications due to other room members
	virtual void RoomMemberStartedSpectating_Callback( const SystemAddress &senderAddress, SLNet::RoomMemberStartedSpectating_Notification *notification);
	virtual void RoomMemberStoppedSpectating_Callback( const SystemAddress &senderAddress, SLNet::RoomMemberStoppedSpectating_Notification *notification);
	virtual void ModeratorChanged_Callback( const SystemAddress &senderAddress, SLNet::ModeratorChanged_Notification *notification);
	virtual void RoomMemberReadyStatusSet_Callback( const SystemAddress &senderAddress, SLNet::RoomMemberReadyStatusSet_Notification *notification);
	virtual void RoomLockStateSet_Callback( const SystemAddress &senderAddress, SLNet::RoomLockStateSet_Notification *notification);
	virtual void RoomMemberKicked_Callback( const SystemAddress &senderAddress, SLNet::RoomMemberKicked_Notification *notification);
	virtual void RoomMemberLeftRoom_Callback( const SystemAddress &senderAddress, SLNet::RoomMemberLeftRoom_Notification *notification);
	virtual void RoomMemberJoinedRoom_Callback( const SystemAddress &senderAddress, SLNet::RoomMemberJoinedRoom_Notification *notification);
	virtual void RoomInvitationSent_Callback( const SystemAddress &senderAddress, SLNet::RoomInvitationSent_Notification *notification);
	virtual void RoomInvitationWithdrawn_Callback( const SystemAddress &senderAddress, SLNet::RoomInvitationWithdrawn_Notification *notification);
	virtual void RoomDestroyedOnModeratorLeft_Callback( const SystemAddress &senderAddress, SLNet::RoomDestroyedOnModeratorLeft_Notification *notification);

	SLNet::Lobby2Client *lobby2Client;
	SLNet::Lobby2MessageFactory *msgFactory;
	SLNet::RakPeerInterface *rakPeer;
	SLNet::RakString titleName;
	SLNet::RakString titleSecretKey;
	SLNet::RakString pathToXMLPropertyFile;
	SLNet::RakString loginUsername;
	SLNet::RoomsPlugin *roomsPlugin;
	unsigned short lanServerPort;
};

} // namespace SLNet

#endif // __ROOMS_BROWSER_GFX3_RAKNET
