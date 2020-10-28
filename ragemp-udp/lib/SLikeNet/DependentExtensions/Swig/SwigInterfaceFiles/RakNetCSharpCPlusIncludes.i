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

//This is not parsed by swig but inserted into the generated C++ wrapper, these includes
//are needed so the wrapper includes the needed .h filese
//This also includes the typemaps used.
 %{
 /* Includes the header in the wrapper code */
//Defines
#ifdef SWIGWIN
#define _MSC_VER 10000
#define WIN32
#define _WIN32
#define _DEBUG
#define _RAKNET_DLL
#endif
//TypeDefs
typedef int int32_t;
typedef unsigned int uint32_t;
typedef uint32_t DefaultIndexType;
#ifdef SWIGWIN
typedef unsigned int SOCKET;
#endif
//Includes
#include "slikenet/smartptr.h"
#include "slikenet/defines.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/Export.h"
#include "slikenet/SimpleMutex.h"
#include "slikenet/string.h"
#include "slikenet/wstring.h"
#include "slikenet/BitStream.h"
#include "slikenet/DS_List.h"	
#include "slikenet/DS_ByteQueue.h"
#include "slikenet/assert.h"
#include "slikenet/NativeTypes.h"
#include "slikenet/SocketIncludes.h"
#include "slikenet/time.h"
#include "slikenet/Export.h"
#include "slikenet/memoryoverride.h"
#include "slikenet/types.h"
#include "slikenet/socket.h"
#include "slikenet/statistics.h"
#include "slikenet/NetworkIDObject.h"
#include "slikenet/NetworkIDManager.h"
//The below three classes have been removed from interface, if PluginInterface2 is fully exposed again
//or another class needs them uncomment them and the related typemaps
//#include "slikenet/TCPInterface.h"
//#include "slikenet/PacketizedTCP.h"
//#include "slikenet/InternalPacket.h"
#include "slikenet/PluginInterface2.h"
#include "slikenet/peerinterface.h"
#include "slikenet/peer.h"
#include "slikenet/PacketPriority.h"
#include "slikenet/PacketLogger.h"
#include "slikenet/PacketFileLogger.h"
#include "slikenet/NatTypeDetectionClient.h"
#include "slikenet/NatPunchthroughClient.h"
#include "slikenet/Router2.h"
#include "slikenet/UDPProxyClient.h"
#include "slikenet/FullyConnectedMesh2.h"
#include "slikenet/ReadyEvent.h"
//#include "slikenet/TeamBalancer.h"
#include "slikenet/TeamManager.h"
#include "slikenet/NatPunchthroughServer.h"
#include "slikenet/UDPForwarder.h"
#include "slikenet/UDPProxyServer.h"
#include "slikenet/UDPProxyCoordinator.h"
#include "slikenet/NatTypeDetectionServer.h"
#include "slikenet/DS_BPlusTree.h"
#include "slikenet/DS_Table.h"
#include "slikenet/FileListTransferCBInterface.h"//
#include "slikenet/IncrementalReadInterface.h"//
#include "slikenet/FileListNodeContext.h"//
#include "slikenet/FileList.h"//
#include "slikenet/TransportInterface.h"//
#include "slikenet/CommandParserInterface.h"//
#include "slikenet/LogCommandParser.h"//
#include "slikenet/MessageFilter.h"//
#include "slikenet/DirectoryDeltaTransfer.h"//
#include "slikenet/FileListTransfer.h"//
#include "slikenet/ThreadsafePacketLogger.h"//
#include "slikenet/PacketConsoleLogger.h"//
#include "slikenet/PacketFileLogger.h"//
#include "slikenet/DS_Multilist.h"
#include "slikenet/ConnectionGraph2.h"
#include "slikenet/GetTime.h"
//#include "slikenet/transport2.h"
//#include "slikenet/RoomsPlugin.h"
//Macros
//Swig C++ code only TypeDefs
//Most of these are nested structs/classes that swig needs to understand as global
//They will reference the nested struct/class while appearing global
typedef SLNet::RakString::SharedString SharedString;
typedef DataStructures::Table::Row Row;
typedef DataStructures::Table::Cell Cell; 
typedef DataStructures::Table::FilterQuery FilterQuery;
typedef DataStructures::Table::ColumnDescriptor ColumnDescriptor;
typedef DataStructures::Table::SortQuery SortQuery;
typedef SLNet::FileListTransferCBInterface::OnFileStruct OnFileStruct;
typedef SLNet::FileListTransferCBInterface::FileProgressStruct FileProgressStruct;
typedef SLNet::FileListTransferCBInterface::DownloadCompleteStruct DownloadCompleteStruct;

 %}

#ifdef SWIG_ADDITIONAL_SQL_LITE
	%{
	#include "SQLite3PluginCommon.h"
	#include "SQLite3ClientPlugin.h"
	#include "SQLiteLoggerCommon.h"
	#include "SQLiteClientLoggerPlugin.h"
	#ifdef SWIG_ADDITIONAL_SQL_LITE_SERVER
		#include "SQLite3ServerPlugin.h"
		#include "SQLiteServerLoggerPlugin.h"
	#endif
	typedef SLNet::LogParameter::DataUnion DataUnion;
	typedef SLNet::SQLiteClientLoggerPlugin::ParameterListHelper ParameterListHelper;
	%}
#endif

#ifdef SWIG_ADDITIONAL_AUTOPATCHER
	%{
	#include "slikenet/AutopatcherRepositoryInterface.h"
	#include "AutopatcherServer.h"
	#include "AutopatcherClient.h"
	#include "AutopatcherMySQLRepository.h"
	#include "CreatePatch.h"
	#include "MemoryCompressor.h"
	#include "ApplyPatch.h"
	#include "slikenet/AutopatcherPatchContext.h"
	%}
#endif

#ifdef RAKNET_COMPATIBILITY
%{
using namespace RakNet;
%}
#else
%{
using namespace SLNet;
%}
#endif

