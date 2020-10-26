// 0000   b0 be 76 29 a3 be 30 9c 23 9e e1 70 08 00 45 00   ..v)..0.#..p..E.
// 0010   00 94 5d d9 00 00 80 11 00 00 c0 a8 00 65 33 53   ..]..........e3S
// 0020   ec 6a dd 99 55 f5 00 80 e1 5c 07 00 ff ff 00 fe   .j..U....\......
// 0030   fe fe fe fd fd fd fd 12 34 56 78 04 cc ac 13 95   ........4Vx.....
// 0040   55 f5 b0 04 5f c4 8d 06 00 00 70 05 00 00 00 00   U..._.....p.....
// 0050   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
// 0060   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
// 0070   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
// 0080   00 00 00 00 00 00 00 00 00 00 00 00 00 00 8e df   ................
// 0090   05 ee 27 f7 1a 7d 1f af 0a 00 20 87 1f 6a 36 00   ..'..}.... ..j6.
// 00a0   00 00                                             ..

// (16 * 5 + 2) + 4 = 86 = 0x56

#[doc = " These i32s are never returned to the user."]
#[doc = " Ping from a connected system.  Update timestamps (internal use only)"]
pub const ID_CONNECTED_PING: i32 = 0;
#[doc = " Ping from an unconnected system.  Reply but do not update timestamps. (internal use only)"]
pub const ID_UNCONNECTED_PING: i32 = 1;
#[doc = " Ping from an unconnected system.  Only reply if we have open connections. Do not update timestamps. (internal use only)"]
pub const ID_UNCONNECTED_PING_OPEN_CONNECTIONS: i32 = 2;
#[doc = " Pong from a connected system.  Update timestamps (internal use only)"]
pub const ID_CONNECTED_PONG: i32 = 3;
#[doc = " A reliable packet to detect lost connections (internal use only)"]
pub const ID_DETECT_LOST_CONNECTIONS: i32 = 4;
#[doc = " C2S: Initial query: Header(1), OfflineMesageID(16), Protocol number(1), Pad(toMTU), sent with no fragment set."]
#[doc = " If protocol fails on server, returns ID_INCOMPATIBLE_PROTOCOL_VERSION to client"]
pub const ID_OPEN_CONNECTION_REQUEST_1: i32 = 5;
#[doc = " S2C: Header(1), OfflineMesageID(16), server GUID(8), HasSecurity(1), Cookie(4, if HasSecurity)"]
#[doc = " , public key (if do security is true), MTU(2). If public key fails on client, returns ID_PUBLIC_KEY_MISMATCH"]
pub const ID_OPEN_CONNECTION_REPLY_1: i32 = 6;
#[doc = " C2S: Header(1), OfflineMesageID(16), Cookie(4, if HasSecurity is true on the server), clientSupportsSecurity(1 bit),"]
#[doc = " handshakeChallenge (if has security on both server and client), remoteBindingAddress(6), MTU(2), client GUID(8)"]
#[doc = " Connection slot allocated if cookie is valid, server is not full, GUID and IP not already in use."]
pub const ID_OPEN_CONNECTION_REQUEST_2: i32 = 7;
#[doc = " S2C: Header(1), OfflineMesageID(16), server GUID(8), mtu(2), doSecurity(1 bit), handshakeAnswer (if do security is true)"]
pub const ID_OPEN_CONNECTION_REPLY_2: i32 = 8;
#[doc = " C2S: Header(1), GUID(8), Timestamp, HasSecurity(1), Proof(32)"]
pub const ID_CONNECTION_REQUEST: i32 = 9;
#[doc = " RakPeer - Remote system requires secure connections, pass a public key to RakPeerInterface::Connect()"]
pub const ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY: i32 = 10;
#[doc = " RakPeer - We passed a public key to RakPeerInterface::Connect(), but the other system did not have security turned on"]
pub const ID_OUR_SYSTEM_REQUIRES_SECURITY: i32 = 11;
#[doc = " RakPeer - Wrong public key passed to RakPeerInterface::Connect()"]
pub const ID_PUBLIC_KEY_MISMATCH: i32 = 12;
#[doc = " RakPeer - Same as ID_ADVERTISE_SYSTEM, but intended for internal use rather than being passed to the user."]
#[doc = " Second byte indicates i32. Used currently for NAT punchthrough for receiver port advertisement. See ID_NAT_ADVERTISE_RECIPIENT_PORT"]
pub const ID_OUT_OF_BAND_INTERNAL: i32 = 13;
#[doc = " If RakPeerInterface::Send() is called where PacketReliability contains _WITH_ACK_RECEIPT, then on a later call to"]
#[doc = " RakPeerInterface::Receive() you will get ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS. The message will be 5 bytes long,"]
#[doc = " and bytes 1-4 inclusive will contain a number in native order containing a number that identifies this message."]
#[doc = " This number will be returned by RakPeerInterface::Send() or RakPeerInterface::SendList(). ID_SND_RECEIPT_ACKED means that"]
#[doc = " the message arrived"]
pub const ID_SND_RECEIPT_ACKED: i32 = 14;
#[doc = " If RakPeerInterface::Send() is called where PacketReliability contains UNRELIABLE_WITH_ACK_RECEIPT, then on a later call to"]
#[doc = " RakPeerInterface::Receive() you will get ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS. The message will be 5 bytes long,"]
#[doc = " and bytes 1-4 inclusive will contain a number in native order containing a number that identifies this message. This number"]
#[doc = " will be returned by RakPeerInterface::Send() or RakPeerInterface::SendList(). ID_SND_RECEIPT_LOSS means that an ack for the"]
#[doc = " message did not arrive (it may or may not have been delivered, probably not). On disconnect or shutdown, you will not get"]
#[doc = " ID_SND_RECEIPT_LOSS for unsent messages, you should consider those messages as all lost."]
pub const ID_SND_RECEIPT_LOSS: i32 = 15;
#[doc = " RakPeer - In a client/server environment, our connection request to the server has been accepted."]
pub const ID_CONNECTION_REQUEST_ACCEPTED: i32 = 16;
#[doc = " RakPeer - Sent to the player when a connection request cannot be completed due to inability to connect."]
pub const ID_CONNECTION_ATTEMPT_FAILED: i32 = 17;
#[doc = " RakPeer - Sent a connect request to a system we are currently connected to."]
pub const ID_ALREADY_CONNECTED: i32 = 18;
#[doc = " RakPeer - A remote system has successfully connected."]
pub const ID_NEW_INCOMING_CONNECTION: i32 = 19;
#[doc = " RakPeer - The system we attempted to connect to is not accepting new connections."]
pub const ID_NO_FREE_INCOMING_CONNECTIONS: i32 = 20;
#[doc = " RakPeer - The system specified in Packet::systemAddress has disconnected from us.  For the client, this would mean the"]
#[doc = " server has shutdown."]
pub const ID_DISCONNECTION_NOTIFICATION: i32 = 21;
#[doc = " RakPeer - Reliable packets cannot be delivered to the system specified in Packet::systemAddress.  The connection to that"]
#[doc = " system has been closed."]
pub const ID_CONNECTION_LOST: i32 = 22;
#[doc = " RakPeer - We are banned from the system we attempted to connect to."]
pub const ID_CONNECTION_BANNED: i32 = 23;
#[doc = " RakPeer - The remote system is using a password and has refused our connection because we did not set the correct password."]
pub const ID_INVALID_PASSWORD: i32 = 24;
#[doc = " RakPeer - The remote system is using a password and has refused our connection because we did not set the correct password."]
pub const ID_INCOMPATIBLE_PROTOCOL_VERSION: i32 = 25;
#[doc = " RakPeer::SetLimitIPConnectionFrequency()"]
pub const ID_IP_RECENTLY_CONNECTED: i32 = 26;
#[doc = " RakPeer - The sizeof(RakNetTime) bytes following this byte represent a value which is automatically modified by the difference"]
#[doc = " in system times between the sender and the recipient. Requires that you call SetOccasionalPing."]
pub const ID_TIMESTAMP: i32 = 27;
#[doc = " RakPeer - Pong from an unconnected system.  First byte is ID_UNCONNECTED_PONG, second sizeof(RakNet::TimeMS) bytes is the ping,"]
#[doc = " following bytes is system specific enumeration data."]
#[doc = " Read using bitstreams"]
pub const ID_UNCONNECTED_PONG: i32 = 28;
#[doc = " RakPeer - Inform a remote system of our IP/Port. On the recipient, all data past ID_ADVERTISE_SYSTEM is whatever was passed to"]
#[doc = " the data parameter"]
pub const ID_ADVERTISE_SYSTEM: i32 = 29;
#[doc = "  partTotal (unsigned int),"]
#[doc = " partLength (unsigned int), first part data (length <= MAX_MTU_SIZE). See the three parameters partCount, partTotal"]
#[doc = "  and partLength in OnFileProgress in FileListTransferCBInterface.h"]
pub const ID_DOWNLOAD_PROGRESS: i32 = 30;
#[doc = " ConnectionGraph2 plugin - In a client/server environment, a client other than ourselves has disconnected gracefully."]
#[doc = "   Packet::systemAddress is modified to reflect the systemAddress of this client."]
pub const ID_REMOTE_DISCONNECTION_NOTIFICATION: i32 = 31;
#[doc = " ConnectionGraph2 plugin - In a client/server environment, a client other than ourselves has been forcefully dropped."]
#[doc = "  Packet::systemAddress is modified to reflect the systemAddress of this client."]
pub const ID_REMOTE_CONNECTION_LOST: i32 = 32;
#[doc = " ConnectionGraph2 plugin: Bytes 1-4 = count. for (count items) contains {SystemAddress, RakNetGUID, 2 byte ping}"]
pub const ID_REMOTE_NEW_INCOMING_CONNECTION: i32 = 33;
#[doc = " FileListTransfer plugin - Setup data"]
pub const ID_FILE_LIST_TRANSFER_HEADER: i32 = 34;
#[doc = " FileListTransfer plugin - A file"]
pub const ID_FILE_LIST_TRANSFER_FILE: i32 = 35;
#[doc = " FileListTransfer plugin - A file"]
pub const ID_FILE_LIST_REFERENCE_PUSH_ACK: i32 = 36;
#[doc = " DirectoryDeltaTransfer plugin - Request from a remote system for a download of a directory"]
pub const ID_DDT_DOWNLOAD_REQUEST: i32 = 37;
#[doc = " RakNetTransport plugin - Transport provider message, used for remote console"]
pub const ID_TRANSPORT_STRING: i32 = 38;
#[doc = " ReplicaManager plugin - Create an object"]
pub const ID_REPLICA_MANAGER_CONSTRUCTION: i32 = 39;
#[doc = " ReplicaManager plugin - Changed scope of an object"]
pub const ID_REPLICA_MANAGER_SCOPE_CHANGE: i32 = 40;
#[doc = " ReplicaManager plugin - Serialized data of an object"]
pub const ID_REPLICA_MANAGER_SERIALIZE: i32 = 41;
#[doc = " ReplicaManager plugin - New connection, about to send all world objects"]
pub const ID_REPLICA_MANAGER_DOWNLOAD_STARTED: i32 = 42;
#[doc = " ReplicaManager plugin - Finished downloading all serialized objects"]
pub const ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE: i32 = 43;
#[doc = " RakVoice plugin - Open a communication channel"]
pub const ID_RAKVOICE_OPEN_CHANNEL_REQUEST: i32 = 44;
#[doc = " RakVoice plugin - Communication channel accepted"]
pub const ID_RAKVOICE_OPEN_CHANNEL_REPLY: i32 = 45;
#[doc = " RakVoice plugin - Close a communication channel"]
pub const ID_RAKVOICE_CLOSE_CHANNEL: i32 = 46;
#[doc = " RakVoice plugin - Voice data"]
pub const ID_RAKVOICE_DATA: i32 = 47;
#[doc = " Autopatcher plugin - Get a list of files that have changed since a certain date"]
pub const ID_AUTOPATCHER_GET_CHANGELIST_SINCE_DATE: i32 = 48;
#[doc = " Autopatcher plugin - A list of files to create"]
pub const ID_AUTOPATCHER_CREATION_LIST: i32 = 49;
#[doc = " Autopatcher plugin - A list of files to delete"]
pub const ID_AUTOPATCHER_DELETION_LIST: i32 = 50;
#[doc = " Autopatcher plugin - A list of files to get patches for"]
pub const ID_AUTOPATCHER_GET_PATCH: i32 = 51;
#[doc = " Autopatcher plugin - A list of patches for a list of files"]
pub const ID_AUTOPATCHER_PATCH_LIST: i32 = 52;
#[doc = " Autopatcher plugin - Returned to the user: An error from the database repository for the autopatcher."]
pub const ID_AUTOPATCHER_REPOSITORY_FATAL_ERROR: i32 = 53;
#[doc = " Autopatcher plugin - Returned to the user: The server does not allow downloading unmodified game files."]
pub const ID_AUTOPATCHER_CANNOT_DOWNLOAD_ORIGINAL_UNMODIFIED_FILES: i32 = 54;
#[doc = " Autopatcher plugin - Finished getting all files from the autopatcher"]
pub const ID_AUTOPATCHER_FINISHED_INTERNAL: i32 = 55;
#[doc = " Autopatcher plugin - Finished getting all files from the autopatcher"]
pub const ID_AUTOPATCHER_FINISHED: i32 = 56;
#[doc = " Autopatcher plugin - Returned to the user: You must restart the application to finish patching."]
pub const ID_AUTOPATCHER_RESTART_APPLICATION: i32 = 57;
#[doc = " NATPunchthrough plugin: internal"]
pub const ID_NAT_PUNCHTHROUGH_REQUEST: i32 = 58;
#[doc = " NATPunchthrough plugin: internal"]
pub const ID_NAT_CONNECT_AT_TIME: i32 = 59;
#[doc = " NATPunchthrough plugin: internal"]
pub const ID_NAT_GET_MOST_RECENT_PORT: i32 = 60;
#[doc = " NATPunchthrough plugin: internal"]
pub const ID_NAT_CLIENT_READY: i32 = 61;
#[doc = " NATPunchthrough plugin: Destination system is not connected to the server. Bytes starting at offset 1 contains the"]
#[doc = "  RakNetGUID destination field of NatPunchthroughClient::OpenNAT()."]
pub const ID_NAT_TARGET_NOT_CONNECTED: i32 = 62;
#[doc = " NATPunchthrough plugin: Destination system is not responding to ID_NAT_GET_MOST_RECENT_PORT. Possibly the plugin is not installed."]
#[doc = "  Bytes starting at offset 1 contains the RakNetGUID  destination field of NatPunchthroughClient::OpenNAT()."]
pub const ID_NAT_TARGET_UNRESPONSIVE: i32 = 63;
#[doc = " NATPunchthrough plugin: The server lost the connection to the destination system while setting up punchthrough."]
#[doc = "  Possibly the plugin is not installed. Bytes starting at offset 1 contains the RakNetGUID  destination"]
#[doc = "  field of NatPunchthroughClient::OpenNAT()."]
pub const ID_NAT_CONNECTION_TO_TARGET_LOST: i32 = 64;
#[doc = " NATPunchthrough plugin: This punchthrough is already in progress. Possibly the plugin is not installed."]
#[doc = "  Bytes starting at offset 1 contains the RakNetGUID destination field of NatPunchthroughClient::OpenNAT()."]
pub const ID_NAT_ALREADY_IN_PROGRESS: i32 = 65;
#[doc = " NATPunchthrough plugin: This message is generated on the local system, and does not come from the network."]
#[doc = "  packet::guid contains the destination field of NatPunchthroughClient::OpenNAT(). Byte 1 contains 1 if you are the sender, 0 if not"]
pub const ID_NAT_PUNCHTHROUGH_FAILED: i32 = 66;
#[doc = " NATPunchthrough plugin: Punchthrough succeeded. See packet::systemAddress and packet::guid. Byte 1 contains 1 if you are the sender,"]
#[doc = "  0 if not. You can now use RakPeer::Connect() or other calls to communicate with this system."]
pub const ID_NAT_PUNCHTHROUGH_SUCCEEDED: i32 = 67;
#[doc = " ReadyEvent plugin - Set the ready state for a particular system"]
#[doc = " First 4 bytes after the message contains the id"]
pub const ID_READY_EVENT_SET: i32 = 68;
#[doc = " ReadyEvent plugin - Unset the ready state for a particular system"]
#[doc = " First 4 bytes after the message contains the id"]
pub const ID_READY_EVENT_UNSET: i32 = 69;
#[doc = " All systems are in state ID_READY_EVENT_SET"]
#[doc = " First 4 bytes after the message contains the id"]
pub const ID_READY_EVENT_ALL_SET: i32 = 70;
#[doc = " \\internal, do not process in your game"]
#[doc = " ReadyEvent plugin - Request of ready event state - used for pulling data when newly connecting"]
pub const ID_READY_EVENT_QUERY: i32 = 71;
#[doc = " Lobby packets. Second byte indicates i32."]
pub const ID_LOBBY_GENERAL: i32 = 72;
#[doc = " Lobby packets. Second byte indicates i32."]
pub const ID_RPC_REMOTE_ERROR: i32 = 73;
#[doc = " Plugin based replacement for RPC system"]
pub const ID_RPC_PLUGIN: i32 = 74;
#[doc = " FileListTransfer transferring large files in chunks that are read only when needed, to save memory"]
pub const ID_FILE_LIST_REFERENCE_PUSH: i32 = 75;
#[doc = " Force the ready event to all set"]
pub const ID_READY_EVENT_FORCE_ALL_SET: i32 = 76;
#[doc = " Rooms function"]
pub const ID_ROOMS_EXECUTE_FUNC: i32 = 77;
#[doc = " Rooms function"]
pub const ID_ROOMS_LOGON_STATUS: i32 = 78;
#[doc = " Rooms function"]
pub const ID_ROOMS_HANDLE_CHANGE: i32 = 79;
#[doc = " Lobby2 message"]
pub const ID_LOBBY2_SEND_MESSAGE: i32 = 80;
#[doc = " Lobby2 message"]
pub const ID_LOBBY2_SERVER_ERROR: i32 = 81;
#[doc = " Informs user of a new host GUID. Packet::Guid contains this new host RakNetGuid. The old host can be read out using BitStream->Read(RakNetGuid) starting on byte 1"]
#[doc = " This is not returned until connected to a remote system"]
#[doc = " If the oldHost is UNASSIGNED_RAKNET_GUID, then this is the first time the host has been determined"]
pub const ID_FCM2_NEW_HOST: i32 = 82;
#[doc = " \\internal For FullyConnectedMesh2 plugin"]
pub const ID_FCM2_REQUEST_FCMGUID: i32 = 83;
#[doc = " \\internal For FullyConnectedMesh2 plugin"]
pub const ID_FCM2_RESPOND_CONNECTION_COUNT: i32 = 84;
#[doc = " \\internal For FullyConnectedMesh2 plugin"]
pub const ID_FCM2_INFORM_FCMGUID: i32 = 85;
#[doc = " \\internal For FullyConnectedMesh2 plugin"]
pub const ID_FCM2_UPDATE_MIN_TOTAL_CONNECTION_COUNT: i32 = 86;
#[doc = " A remote system (not necessarily the host) called FullyConnectedMesh2::StartVerifiedJoin() with our system as the client"]
#[doc = " Use FullyConnectedMesh2::GetVerifiedJoinRequiredProcessingList() to read systems"]
#[doc = " For each system, attempt NatPunchthroughClient::OpenNAT() and/or RakPeerInterface::Connect()"]
#[doc = " When this has been done for all systems, the remote system will automatically be informed of the results"]
#[doc = " \\note Only the designated client gets this message"]
#[doc = " \\note You won't get this message if you are already connected to all target systems"]
#[doc = " \\note If you fail to connect to a system, this does not automatically mean you will get ID_FCM2_VERIFIED_JOIN_FAILED as that system may have been shutting down from the host too"]
#[doc = " \\sa FullyConnectedMesh2::StartVerifiedJoin()"]
pub const ID_FCM2_VERIFIED_JOIN_START: i32 = 87;
#[doc = " \\internal The client has completed processing for all systems designated in ID_FCM2_VERIFIED_JOIN_START"]
pub const ID_FCM2_VERIFIED_JOIN_CAPABLE: i32 = 88;
#[doc = " Client failed to connect to a required systems notified via FullyConnectedMesh2::StartVerifiedJoin()"]
#[doc = " RakPeerInterface::CloseConnection() was automatically called for all systems connected due to ID_FCM2_VERIFIED_JOIN_START"]
#[doc = " Programmer should inform the player via the UI that they cannot join this session, and to choose a different session"]
#[doc = " \\note Server normally sends us this message, however if connection to the server was lost, message will be returned locally"]
#[doc = " \\note Only the designated client gets this message"]
pub const ID_FCM2_VERIFIED_JOIN_FAILED: i32 = 89;
#[doc = " The system that called StartVerifiedJoin() got ID_FCM2_VERIFIED_JOIN_CAPABLE from the client and then called RespondOnVerifiedJoinCapable() with true"]
#[doc = " AddParticipant() has automatically been called for this system"]
#[doc = " Use GetVerifiedJoinAcceptedAdditionalData() to read any additional data passed to RespondOnVerifiedJoinCapable()"]
#[doc = " \\note All systems in the mesh get this message"]
#[doc = " \\sa RespondOnVerifiedJoinCapable()"]
pub const ID_FCM2_VERIFIED_JOIN_ACCEPTED: i32 = 90;
#[doc = " The system that called StartVerifiedJoin() got ID_FCM2_VERIFIED_JOIN_CAPABLE from the client and then called RespondOnVerifiedJoinCapable() with false"]
#[doc = " CloseConnection() has been automatically called for each system connected to since ID_FCM2_VERIFIED_JOIN_START."]
#[doc = " The connection is NOT automatically closed to the original host that sent StartVerifiedJoin()"]
#[doc = " Use GetVerifiedJoinRejectedAdditionalData() to read any additional data passed to RespondOnVerifiedJoinCapable()"]
#[doc = " \\note Only the designated client gets this message"]
#[doc = " \\sa RespondOnVerifiedJoinCapable()"]
pub const ID_FCM2_VERIFIED_JOIN_REJECTED: i32 = 91;
#[doc = " UDP proxy messages. Second byte indicates i32."]
pub const ID_UDP_PROXY_GENERAL: i32 = 92;
#[doc = " SQLite3Plugin - execute"]
pub const ID_SQLite3_EXEC: i32 = 93;
#[doc = " SQLite3Plugin - Remote database is unknown"]
pub const ID_SQLite3_UNKNOWN_DB: i32 = 94;
#[doc = " Events happening with SQLiteClientLoggerPlugin"]
pub const ID_SQLLITE_LOGGER: i32 = 95;
#[doc = " Sent to Nati32DetectionServer"]
pub const ID_NAT_i32_DETECTION_REQUEST: i32 = 96;
#[doc = " Sent to Nati32DetectionClient. Byte 1 contains the i32 of NAT detected."]
pub const ID_NAT_i32_DETECTION_RESULT: i32 = 97;
#[doc = " Used by the router2 plugin"]
pub const ID_ROUTER_2_INTERNAL: i32 = 98;
#[doc = " No path is available or can be established to the remote system"]
#[doc = " Packet::guid contains the endpoint guid that we were trying to reach"]
pub const ID_ROUTER_2_FORWARDING_NO_PATH: i32 = 99;
#[doc = " \\brief You can now call connect, ping, or other operations to the destination system."]
#[doc = ""]
#[doc = " Connect as follows:"]
#[doc = ""]
#[doc = " RakNet::BitStream bs(packet->data, packet->length, false);"]
#[doc = " bs.IgnoreBytes(sizeof(MessageID));"]
#[doc = " RakNetGUID endpointGuid;"]
#[doc = " bs.Read(endpointGuid);"]
#[doc = " unsigned short sourceToDestPort;"]
#[doc = " bs.Read(sourceToDestPort);"]
#[doc = " char ipAddressString[32];"]
#[doc = " packet->systemAddress.ToString(false, ipAddressString);"]
#[doc = " rakPeerInterface->Connect(ipAddressString, sourceToDestPort, 0,0);"]
pub const ID_ROUTER_2_FORWARDING_ESTABLISHED: i32 = 100;
#[doc = " The IP address for a forwarded connection has changed"]
#[doc = " Read endpointGuid and port as per ID_ROUTER_2_FORWARDING_ESTABLISHED"]
pub const ID_ROUTER_2_REROUTED: i32 = 101;
#[doc = " \\internal Used by the team balancer plugin"]
pub const ID_TEAM_BALANCER_INTERNAL: i32 = 102;
#[doc = " Cannot switch to the desired team because it is full. However, if someone on that team leaves, you will"]
#[doc = "  get ID_TEAM_BALANCER_TEAM_ASSIGNED later."]
#[doc = " For TeamBalancer: Byte 1 contains the team you requested to join. Following bytes contain NetworkID of which member"]
pub const ID_TEAM_BALANCER_REQUESTED_TEAM_FULL: i32 = 103;
#[doc = " Cannot switch to the desired team because all teams are locked. However, if someone on that team leaves,"]
#[doc = "  you will get ID_TEAM_BALANCER_SET_TEAM later."]
#[doc = " For TeamBalancer: Byte 1 contains the team you requested to join."]
pub const ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED: i32 = 104;
#[doc = " Cannot switch to the desired team because all teams are locked. However, if someone on that team leaves,"]
#[doc = "  you will get ID_TEAM_BALANCER_SET_TEAM later."]
#[doc = " For TeamBalancer: Byte 1 contains the team you requested to join."]
pub const ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED: i32 = 105;
#[doc = " Team balancer plugin informing you of your team. Byte 1 contains the team you requested to join. Following bytes contain NetworkID of which member."]
pub const ID_TEAM_BALANCER_TEAM_ASSIGNED: i32 = 106;
#[doc = " Gamebryo Lightspeed integration"]
pub const ID_LIGHTSPEED_INTEGRATION: i32 = 107;
#[doc = " XBOX integration"]
pub const ID_XBOX_LOBBY: i32 = 108;
#[doc = " The password we used to challenge the other system passed, meaning the other system has called TwoWayAuthentication::AddPassword() with the same password we passed to TwoWayAuthentication::Challenge()"]
#[doc = " You can read the identifier used to challenge as follows:"]
#[doc = " RakNet::BitStream bs(packet->data, packet->length, false); bs.IgnoreBytes(sizeof(RakNet::MessageID)); RakNet::RakString password; bs.Read(password);"]
pub const ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS: i32 = 109;
#[doc = " The password we used to challenge the other system passed, meaning the other system has called TwoWayAuthentication::AddPassword() with the same password we passed to TwoWayAuthentication::Challenge()"]
#[doc = " You can read the identifier used to challenge as follows:"]
#[doc = " RakNet::BitStream bs(packet->data, packet->length, false); bs.IgnoreBytes(sizeof(RakNet::MessageID)); RakNet::RakString password; bs.Read(password);"]
pub const ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS: i32 = 110;
#[doc = " A remote system sent us a challenge using TwoWayAuthentication::Challenge(), and the challenge failed."]
#[doc = " If the other system must pass the challenge to stay connected, you should call RakPeer::CloseConnection() to terminate the connection to the other system."]
pub const ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE: i32 = 111;
#[doc = " The other system did not add the password we used to TwoWayAuthentication::AddPassword()"]
#[doc = " You can read the identifier used to challenge as follows:"]
#[doc = " RakNet::BitStream bs(packet->data, packet->length, false); bs.IgnoreBytes(sizeof(MessageID)); RakNet::RakString password; bs.Read(password);"]
pub const ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE: i32 = 112;
#[doc = " The other system did not respond within a timeout threshhold. Either the other system is not running the plugin or the other system was blocking on some operation for a long time."]
#[doc = " You can read the identifier used to challenge as follows:"]
#[doc = " RakNet::BitStream bs(packet->data, packet->length, false); bs.IgnoreBytes(sizeof(MessageID)); RakNet::RakString password; bs.Read(password);"]
pub const ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT: i32 = 113;
#[doc = " \\internal"]
pub const ID_TWO_WAY_AUTHENTICATION_NEGOTIATION: i32 = 114;
#[doc = " CloudClient / CloudServer"]
pub const ID_CLOUD_POST_REQUEST: i32 = 115;
#[doc = " CloudClient / CloudServer"]
pub const ID_CLOUD_RELEASE_REQUEST: i32 = 116;
#[doc = " CloudClient / CloudServer"]
pub const ID_CLOUD_GET_REQUEST: i32 = 117;
#[doc = " CloudClient / CloudServer"]
pub const ID_CLOUD_GET_RESPONSE: i32 = 118;
#[doc = " CloudClient / CloudServer"]
pub const ID_CLOUD_UNSUBSCRIBE_REQUEST: i32 = 119;
#[doc = " CloudClient / CloudServer"]
pub const ID_CLOUD_SERVER_TO_SERVER_COMMAND: i32 = 120;
#[doc = " CloudClient / CloudServer"]
pub const ID_CLOUD_SUBSCRIPTION_NOTIFICATION: i32 = 121;
#[doc = " CloudClient / CloudServer"]
pub const ID_LIB_VOICE: i32 = 122;
#[doc = " CloudClient / CloudServer"]
pub const ID_RELAY_PLUGIN: i32 = 123;
#[doc = " CloudClient / CloudServer"]
pub const ID_NAT_REQUEST_BOUND_ADDRESSES: i32 = 124;
#[doc = " CloudClient / CloudServer"]
pub const ID_NAT_RESPOND_BOUND_ADDRESSES: i32 = 125;
#[doc = " CloudClient / CloudServer"]
pub const ID_FCM2_UPDATE_USER_CONTEXT: i32 = 126;
#[doc = " CloudClient / CloudServer"]
pub const ID_RESERVED_3: i32 = 127;
#[doc = " CloudClient / CloudServer"]
pub const ID_RESERVED_4: i32 = 128;
#[doc = " CloudClient / CloudServer"]
pub const ID_RESERVED_5: i32 = 129;
#[doc = " CloudClient / CloudServer"]
pub const ID_RESERVED_6: i32 = 130;
#[doc = " CloudClient / CloudServer"]
pub const ID_RESERVED_7: i32 = 131;
#[doc = " CloudClient / CloudServer"]
pub const ID_RESERVED_8: i32 = 132;
#[doc = " CloudClient / CloudServer"]
pub const ID_RESERVED_9: i32 = 133;
#[doc = " CloudClient / CloudServer"]
pub const ID_USER_PACKET_ENUM: i32 = 134;