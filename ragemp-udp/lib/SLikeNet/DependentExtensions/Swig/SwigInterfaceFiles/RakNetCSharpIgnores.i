//
// This file was taken from RakNet 4.082.
// Please see licenses/RakNet license.txt for the underlying license and related copyright.
//
//
//
// Modified work: Copyright (c) 2018, SLikeSoft UG (haftungsbeschränkt)
//
// This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
// license found in the license.txt file in the root directory of this source tree.

//----------Ignores----------------
//This file specifies things that should be ignored by Swig
//Typeically this is used for the following reasons: a C# replacement was made for the function, it is not supported by Swig
//,it should not be exposed, or the item is internal and it would take time to convert

//BitStream
%ignore SLNet::BitStream::Write(unsigned char * const inTemplateVar);
%ignore SLNet::BitStream::WriteCompressed(unsigned char * const inTemplateVar);
%ignore SLNet::BitStream::Write(const unsigned char * const inTemplateVar);
%ignore SLNet::BitStream::WriteCompressed(const unsigned char * const inTemplateVar);
%ignore SLNet::BitStream::Write( const char* inputByteArray, const unsigned int numberOfBytes );
%ignore SLNet::BitStream::Read(char *varString);
%ignore SLNet::BitStream::Read(unsigned char *varString);
%ignore SLNet::BitStream::Read( char* output, const unsigned int numberOfBytes );
%ignore SLNet::BitStream::ReadCompressedDelta(char &outTemplateVar);
%ignore SLNet::BitStream::ReadDelta(char &outTemplateVar);
%ignore SLNet::BitStream::ReadCompressed(char &outTemplateVar);
%ignore SLNet::BitStream::ReadCompressedDelta(char* &outTemplateVar);
%ignore SLNet::BitStream::ReadDelta(char* &outTemplateVar);
%ignore SLNet::BitStream::ReadCompressed(char* &outTemplateVar);
%ignore SLNet::BitStream::CopyData(unsigned char** _data ) const;
%ignore SLNet::BitStream::PrintBits(char *out) const;
%ignore SLNet::BitStream::PrintHex(char *out) const;
%ignore SLNet::BitStream::GetData;
%ignore SLNet::BitStream::Serialize(bool writeToBitstream,  char* inputByteArray, const unsigned int numberOfBytes );
%ignore SLNet::BitStream::SerializeDelta(bool writeToBitstream,  char* inputByteArray, const unsigned int numberOfBytes );
%ignore SLNet::BitStream::SerializeCompressed(bool writeToBitstream,  char* inputByteArray, const unsigned int numberOfBytes );
%ignore SLNet::BitStream::SerializeCompressedDelta(bool writeToBitstream, char* inputByteArray, const unsigned int numberOfBytes );
%ignore SLNet::BitStream::ReadAlignedBytesSafe( char *inOutByteArray, int &inputLength, const int maxBytesToRead );
%ignore SLNet::BitStream::ReadAlignedBytesSafe( char *inOutByteArray, unsigned int &inputLength, const unsigned int maxBytesToRead );
%ignore SLNet::BitStream::ReadAlignedBytesSafeAlloc( char **outByteArray, int &inputLength, const int maxBytesToRead );
%ignore SLNet::BitStream::ReadAlignedBytesSafeAlloc( char **outByteArray, unsigned int &inputLength, const unsigned int maxBytesToRead );
%ignore SLNet::BitStream::WriteAlignedVar8(const char *inByteArray);
%ignore SLNet::BitStream::WriteAlignedVar8(const char *inByteArray);
%ignore SLNet::BitStream::ReadAlignedVar8(char *inOutByteArray);		
%ignore SLNet::BitStream::WriteAlignedVar16(const char *inByteArray);		
%ignore SLNet::BitStream::ReadAlignedVar16(char *inOutByteArray);	
%ignore SLNet::BitStream::WriteAlignedVar32(const char *inByteArray);	
%ignore SLNet::BitStream::ReadAlignedVar32(char *inOutByteArray);
%ignore SLNet::BitStream::WriteAlignedBytesSafe( const char *inByteArray, const unsigned int inputLength, const unsigned int maxBytesToWrite );
%ignore SLNet::BitStream::Read( BitStream &bitStream, BitSize_t numberOfBits );
%ignore SLNet::BitStream::Read( BitStream &bitStream );
%ignore SLNet::BitStream::Write( BitStream &bitStream, BitSize_t numberOfBits );
%ignore SLNet::BitStream::Write( BitStream &bitStream );
%ignore SLNet::BitStream::ReadAlignedBytesSafeAlloc( char ** outByteArray, unsigned int &inputLength, const unsigned int maxBytesToRead );
%ignore SLNet::BitStream::ReadAlignedBytesSafeAlloc( char **outByteArray, int &inputLength, const unsigned int maxBytesToRead );

//RakPeer
%define IGNORERAKPEERANDINTERFACE(theMacroInputFunction)
%ignore SLNet::RakPeer::theMacroInputFunction;
%ignore SLNet::RakPeerInterface::theMacroInputFunction;
%enddef

IGNORERAKPEERANDINTERFACE(GetIncomingPassword( char* passwordData, int *passwordDataLength  ))
IGNORERAKPEERANDINTERFACE(GetOfflinePingResponse( char **data, unsigned int *length ))
IGNORERAKPEERANDINTERFACE(RegisterAsRemoteProcedureCall( const char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) ))
IGNORERAKPEERANDINTERFACE(RegisterClassMemberRPC( const char* uniqueID, void *functionPointer ))
IGNORERAKPEERANDINTERFACE(UnregisterAsRemoteProcedureCall( const char* uniqueID ))
IGNORERAKPEERANDINTERFACE(RPC( const char* uniqueID, const char *data, BitSize_t bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, RakNetTime *includedTimestamp, NetworkID networkID, SLNet::BitStream *replyFromTarget ))
IGNORERAKPEERANDINTERFACE(RPC( const char* uniqueID, const SLNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, RakNetTime *includedTimestamp, NetworkID networkID, SLNet::BitStream *replyFromTarget ))
IGNORERAKPEERANDINTERFACE(GetRPCString( const char *data, const BitSize_t bitSize, const SystemAddress systemAddress))
IGNORERAKPEERANDINTERFACE(SendOutOfBand(const char *host, unsigned short remotePort, MessageID header, const char *data, BitSize_t dataLength, unsigned connectionSocketIndex=0 ))
IGNORERAKPEERANDINTERFACE(SetUserUpdateThread(void (*_userUpdateThreadPtr)(RakPeerInterface *, void *), void *_userUpdateThreadData))
IGNORERAKPEERANDINTERFACE(SendList)
IGNORERAKPEERANDINTERFACE(ReleaseSockets)
IGNORERAKPEERANDINTERFACE(GetSocket( const SystemAddress target ))
IGNORERAKPEERANDINTERFACE(GetSockets( DataStructures::List<RakNetSmartPtr<RakNetSocket> > &sockets ))
IGNORERAKPEERANDINTERFACE(ConnectWithSocket)
IGNORERAKPEERANDINTERFACE(SetRouterInterface)
IGNORERAKPEERANDINTERFACE(RemoveRouterInterface)
IGNORERAKPEERANDINTERFACE(GetConnectionList( SystemAddress *remoteSystems, unsigned short *numberOfSystems ) const)
IGNORERAKPEERANDINTERFACE(SetIncomingDatagramEventHandler( bool (*_incomingDatagramEventHandler)(RNS2RecvStruct *) ))

//RakPeer only
//Swig doesn't know how to handle friend functions, so even if it is in the protected section 
//They must be explicitly ignored
//This Specific case is somehow placed in the SLNet namespace rather than SLNet::RakPeer
//Ignore both to be safe
%ignore SLNet::RakPeerInterface::GetStatisticsList;
%ignore SLNet::RakPeer::GetStatisticsList;
%ignore SLNet::RakPeer::ProcessOfflineNetworkPacket;
%ignore SLNet::RakPeer::ProcessNetworkPacket;
%ignore SLNet::ProcessOfflineNetworkPacket;
%ignore SLNet::ProcessNetworkPacket;

//RakString
%ignore AppendBytes(const char *bytes, unsigned int count);//Interface remade
%ignore SLNet::RakString::FPrintf(FILE *fp); //Expects C file pointer
//RakString Internal
%ignore SLNet::RakString::RakString( SharedString *_sharedString );
%ignore SLNet::RakString::sharedString;
%ignore SLNet::RakString::RakString(const unsigned char *format, ...);
%ignore SLNet::RakString::freeList;
%ignore SLNet::RakString::emptyString;

//List
%ignore DataStructures::List::operator[];

//SystemAddress
%ignore SLNet::SystemAddress::ToString(bool writePort, char *dest) const;
%ignore SLNet::SystemAddress::ToString() const;

//RakNetGUID
%ignore SLNet::RakNetGUID::ToString(char *dest) const;

//AddressOrGUID
%ignore SLNet::AddressOrGUID::ToString(bool writePort, char *dest) const;
%ignore SLNet::AddressOrGUID::ToString() const;

//PacketizedTCP
%ignore SLNet::PacketizedTCP::SendList;
%ignore SLNet::TCPInterface::SendList;

//InternalPacket
%ignore SLNet::InternalPacket::refCountedData;
%ignore SLNet::InternalPacketRefCountedData;

//RemoteClient
%ignore SLNet::RemoteClient::SendOrBuffer;

//NetworkIDManager
%ignore SLNet::NetworkIDManager::GET_OBJECT_FROM_ID;
%ignore SLNet::NetworkIDManager::TrackNetworkIDObject;
%ignore SLNet::NetworkIDManager::StopTrackingNetworkIDObject;

//NetworkIDObject
%ignore SLNet::NetworkIDObject::SetParent;
%ignore SLNet::NetworkIDObject::GetParent;

//RakNetSocket
%ignore RakNetSocket::recvEvent;
%ignore RakNetSocket::Fcntl;

//To allow easier future support if needed, rather than not parsing the PluginInterface2 ignore the functions
//Later if decided that is needed remove the ignores and the commented typemaps and includes
%ignore SLNet::PluginInterface2::OnAttach;
%ignore SLNet::PluginInterface2::OnDetach;
%ignore SLNet::PluginInterface2::Update;
%ignore SLNet::PluginInterface2::OnReceive;
%ignore SLNet::PluginInterface2::OnStartup;
%ignore SLNet::PluginInterface2::OnShutdown;
%ignore SLNet::PluginInterface2::OnClosedConnection;
%ignore SLNet::PluginInterface2::OnNewConnection;
%ignore SLNet::PluginInterface2::OnFailedConnectionAttempt;
%ignore SLNet::PluginInterface2::OnDirectSocketSend;
%ignore SLNet::PluginInterface2::OnInternalPacket;
%ignore SLNet::PluginInterface2::OnAck;
%ignore SLNet::PluginInterface2::OnPushBackPacket;
%ignore SLNet::PluginInterface2::OnDirectSocketReceive;
%ignore SLNet::PluginInterface2::OnRakPeerShutdown;
%ignore SLNet::PluginInterface2::OnRakPeerStartup;
%ignore SLNet::PluginInterface2::OnReliabilityLayerPacketError;

//NatPunchthroughClient
	/// \internal For plugin handling
%ignore  SLNet::NatPunchthroughClient::Update(void);
	/// \internal For plugin handling
%ignore SLNet::NatPunchthroughClient::OnReceive(Packet *packet);
	/// \internal For plugin handling
%ignore SLNet::NatPunchthroughClient::OnNewConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, bool isIncoming);
	/// \internal For plugin handling
%ignore SLNet::NatPunchthroughClient::OnClosedConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason );
%ignore SLNet::NatPunchthroughClient::OnAttach();
%ignore SLNet::NatPunchthroughClient::OnDetach();
%ignore SLNet::NatPunchthroughClient::OnRakPeerShutdown();

//Uin24_t
%ignore SLNet::uint24_t::operator++(int);
%ignore SLNet::uint24_t::operator--(int);

//ReadyEvent
%ignore SLNet::ReadyEvent::RemoteSystemCompBySystemAddress;
%ignore SLNet::ReadyEvent::RemoteSystem;
%ignore SLNet::ReadyEvent::ReadyEventNodeComp;
%ignore SLNet::ReadyEvent::ReadyEventNode;

//PacketLoggger
%ignore SLNet::PacketLogger::FormatLine;

//FullyConnectedMesh2
%ignore SLNet::FullyConnectedMesh2::GetElapsedRuntime;
%ignore SLNet::FullyConnectedMesh2::OnReceive;

//Structs
%ignore SLNet::PI2_FailedConnectionAttemptReason;
%ignore SLNet::PI2_LostConnectionReason;
//These are internal
%ignore SLNet::PluginInterface2::SetRakPeerInterface;
%ignore SLNet::PluginInterface2::SetPacketizedTCP;

//ByteQueue
%ignore DataStructures::ByteQueue::WriteBytes(const char *in, unsigned length, const char *file, unsigned int line);
%ignore DataStructures::ByteQueue::ReadBytes(char *out, unsigned maxLengthToRead, bool peek);
%ignore DataStructures::ByteQueue::PeekContiguousBytes(unsigned int *outLength) const;

//UDPProxyClient
%ignore SLNet::UDPProxyClient::pingServerGroups;
%ignore SLNet::UDPProxyClient::OnReceive;

//NatPunchThroughServer
%ignore SLNet::NatPunchthroughServer::NatPunchthroughUserComp;

//UDPForwarder
%ignore SLNet::UDPForwarder::threadOperationIncomingQueue;
%ignore SLNet::UDPForwarder::threadOperationOutgoingQueue;
%ignore SLNet::UDPForwarder::forwardList;
%ignore AddForwardingEntry(SrcAndDest srcAndDest, SLNet::TimeMS timeoutOnNoDataMS, unsigned short *port, const char *forceHostAddress);

//MessageFilter
%ignore SLNet::FilterSet;//internal
%ignore SLNet::MessageFilterStrComp;//internal
%ignore SLNet::FilterSetComp;//internal
%ignore SLNet::FilteredSystem;//internal
%ignore SLNet::FilteredSystemComp;//internal
%ignore SLNet::MessageFilter::SetDisallowedMessageCallback(int filterSetID, void *userData, void (*invalidMessageCallback)(RakPeerInterface *peer, AddressOrGUID addressOrGUID, int filterSetID, void *userData, unsigned char messageID)); //Void pointer
%ignore SLNet::MessageFilter::SetTimeoutCallback(int filterSetID, void *userData, void (*invalidMessageCallback)(RakPeerInterface *peer, AddressOrGUID addressOrGUID, int filterSetID, void *userData)); //Void pointer

//Table
//%ignore DataStructures::Table::GetRows;
%ignore DataStructures::Table::GetListHead;
%ignore DataStructures::Table::SortTable(Table::SortQuery *sortQueries, unsigned numSortQueries, Table::Row** out);//Completely C# replacement using a helper function
%ignore DataStructures::Table::GetCellValueByIndex(unsigned rowIndex, unsigned columnIndex, char *output);
%ignore DataStructures::Table::GetCellValueByIndex(unsigned rowIndex, unsigned columnIndex, char *output, int *outputLength);
%ignore DataStructures::Table::PrintColumnHeaders(char *out, int outLength, char columnDelineator) const;
%ignore DataStructures::Table::PrintRow(char *out, int outLength, char columnDelineator, bool printDelineatorForBinary, Table::Row* inputRow) const;
%ignore DataStructures::Table::QueryTable(unsigned *columnIndicesSubset, unsigned numColumnSubset, FilterQuery *inclusionFilters, unsigned numInclusionFilters, unsigned *rowIds, unsigned numRowIDs, Table *result);
%ignore UpdateCell(unsigned rowId, unsigned columnIndex, int byteLength, char *data);
%ignore UpdateCellByIndex(unsigned rowIndex, unsigned columnIndex, int byteLength, char *data);
%ignore Cell::ptr;
%ignore Cell::Cell(double numericValue, char *charValue, void *ptr, DataStructures::Table::ColumnType type);
%ignore Cell::SetByType(double numericValue, char *charValue, void *ptr, DataStructures::Table::ColumnType type);
%ignore Cell::SetPtr;
%ignore Row::UpdateCell(unsigned columnIndex, int byteLength, const char *data);
%ignore Cell::Get(char *output, int *outputLength);
%ignore Cell::Set(const char *input, int inputLength);
%ignore Cell::Get(char *output);
%ignore Cell::c; //Not really useful externally
%ignore ColumnIndex;// Swig will not ignore the definition I wish to, so I ignore both and make helper functions
 		
//Table Lists
%ignore DataStructures::List <ColumnDescriptor>::GetIndexOf;
%ignore DataStructures::List <Row>::GetIndexOf;
%ignore DataStructures::List <Cell>::GetIndexOf;
%ignore DataStructures::List <FilterQuery>::GetIndexOf;
%ignore DataStructures::List <SortQuery>::GetIndexOf;
 
//BPlusTree
//The next two use C function pointers
%ignore DataStructures::BPlusTree::ForEachData;
%ignore DataStructures::BPlusTree::ForEachLeaf;
%ignore DataStructures::Page::keys;
%ignore DataStructures::Page::children;
%ignore DataStructures::Page::data;
 
//FileList Lists
%ignore DataStructures::List<SLNet::FileListNode>::GetIndexOf;
%ignore SLNet::FileList::GetCallbacks(DataStructures::List<FileListProgress*> &callbacks);

// FileListTransfer
%ignore SLNet::FileListTransfer::GetCallbacks(DataStructures::List<FileListProgress*> &callbacks);
%ignore SendIRIToAddressCB(FileListTransfer::ThreadData threadData, bool *returnOutput, void* perThreadData);

//Bplus
%ignore DataStructures::BPlusTree::ValidateTree;

//File
%ignore SLNet::IncrementalReadInterface::GetFilePart( const char *filename, unsigned int startReadBytes, unsigned int numBytesToRead, void *preallocatedDestination, FileListNodeContext context);

//FileList
%ignore AddFile(const char *filename, const char *fullPathToFile, const char *data, const unsigned dataLength, const unsigned fileLength, FileListNodeContext context, bool isAReference=false, bool takeDataPointer=false);

//CommandParserInterface
//Internal
%ignore RegisteredCommand;
%ignore RegisteredCommandComp( const char* const & key, const RegisteredCommand &data );
%ignore GetRegisteredCommand(const char *command, RegisteredCommand *rc);
%ignore ParseConsoleString(char *str, const char delineator, unsigned char delineatorToggle, unsigned *numParameters, char **parameterList, unsigned parameterListLength);
%ignore SendCommandList(TransportInterface *transport, SystemAddress systemAddress);

//TransportInterface
%ignore SLNet::TransportInterface::Send( SystemAddress systemAddress, const char *data, ... );

//Router2
%ignore OnReceive(Packet *packet);

//MultiList
%ignore DataStructures::Multilist::ForEach;

//ConnectionGraph2
%ignore SLNet::ConnectionGraph2::SystemAddressAndGuid;
%ignore SLNet::ConnectionGraph2::SystemAddressAndGuidComp;
%ignore SLNet::ConnectionGraph2::RemoteSystem;
%ignore SLNet::ConnectionGraph2::RemoteSystemComp;
%ignore SLNet::ConnectionGraph2::GetConnectionList;
%ignore SLNet::ConnectionGraph2::GetParticipantList;

#ifdef SWIG_ADDITIONAL_SQL_LITE
//LogParameter
%ignore SLNet::LogParameter::LogParameter(void *t);
%ignore SLNet::LogParameter::LogParameter(void *t);
%ignore SLNet::LogParameter::LogParameter(const unsigned char t[]);
%ignore SLNet::LogParameter::LogParameter(const char t[]);
%ignore SLNet::LogParameter::LogParameter(BlobDescriptor t);
%ignore SLNet::LogParameter::LogParameter(RGBImageBlob t);

//SQLite3Row List and SQLLite3Table, not pointers so these functions not needed and will error
%ignore DataStructures::Multilist<ML_STACK, SLNet::RakString,SLNet::RakString,DefaultIndexType>::GetPtr;
%ignore DataStructures::Multilist<ML_STACK, SLNet::RakString,SLNet::RakString,DefaultIndexType>::ClearPointers;
%ignore DataStructures::Multilist<ML_STACK, SLNet::RakString,SLNet::RakString,DefaultIndexType>::ClearPointer;

//common
%ignore LogParameter;
%ignore BlobDescriptor;
%ignore RGBImageBlob;

//SQLiteClientLogger
%ignore SLNet::SQLiteClientLoggerPlugin::CheckQuery;
%ignore SLNet::SQLiteClientLoggerPlugin::ParameterListHelper;
%ignore SLNet::SQLiteClientLoggerPlugin::SqlLog;
%ignore SLNet::SQLiteClientLoggerPlugin::__sqlLogInternal;
%ignore SLNet::SQLiteClientLoggerPlugin::logger;
#endif

//Global
%ignore REGISTER_STATIC_RPC;
%ignore CLASS_MEMBER_ID;
%ignore REGISTER_CLASS_MEMBER_RPC;
%ignore UNREGISTER_STATIC_RPC;
%ignore UNREGISTER_CLASS_MEMBER_RPC;

//Operators
//These need te be handled manually or not at all
%ignore operator const char*;
%ignore operator wchar_t*;
%ignore operator uint32_t;
%ignore operator &; //Not overloadable in C#
%ignore operator <<;//Doesn't work the same in C#, only usable with int
%ignore operator >>;//Doesn't work the same in C#, only usable with int

//X= is automatically handled in C# if you overload = and X, you can't specify an overload
%ignore operator +=;
%ignore operator -=;
%ignore operator /=;

//RakString
%ignore SLNet::RakString::operator = (char *);
%ignore SLNet::RakString::operator == (char *) const;
%ignore SLNet::RakString::operator != (char *) const;

//Structs
%ignore RPCParameters;

//Global
%ignore StatisticsToString; //Custom C# wrapper written for it

#ifdef SWIG_ADDITIONAL_AUTOPATCHER
	%ignore CreatePatch; //Custom C# wrapper written for it
	%ignore SLNet::AutopatcherServer::StartThreads;
	%ignore SLNet::AutopatcherClient::OnThreadCompletion;
	%ignore SLNet::MemoryCompressor::Compress(char *input, const unsigned inputLength, bool finish);
	%ignore SLNet::MemoryDecompressor::MemoryDecompress(unsigned char *inputByteArray, const unsigned inputLength, bool ignoreStreamEnd);
	%ignore CompressorBase::GetOutput;
#endif
