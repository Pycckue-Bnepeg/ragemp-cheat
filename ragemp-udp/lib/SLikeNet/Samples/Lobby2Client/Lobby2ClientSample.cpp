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

#include "Lobby2Message.h"
#include "slikenet/peerinterface.h"

#include "slikenet/MessageIdentifiers.h"
#include "Lobby2Client.h"
#include "slikenet/Kbhit.h"
#include "slikenet/sleep.h"
#include "RoomsErrorCodes.h"
#include "slikenet/DS_Queue.h"
#include <ctype.h>
#include <stdlib.h>
#include <limits> // used for std::numeric_limits
#include "slikenet/LinuxStrings.h"
#include "slikenet/Gets.h"
#include "slikenet/linux_adapter.h"
#include "slikenet/osx_adapter.h"

static const int NUM_CONNECTIONS=2;
SLNet::Lobby2Client lobby2Client[NUM_CONNECTIONS];
SLNet::Lobby2MessageFactory messageFactory;
SLNet::RakString testUserName[NUM_CONNECTIONS];
SLNet::RakPeerInterface *rakPeer[NUM_CONNECTIONS];
struct AutoExecutionPlanNode
{
	AutoExecutionPlanNode() {}
	AutoExecutionPlanNode(int i, SLNet::Lobby2MessageID o) {instanceNumber=i; operation=o;}
	int instanceNumber;
	SLNet::Lobby2MessageID operation;
};
DataStructures::Queue<AutoExecutionPlanNode> executionPlan;

void PrintCommands()
{
	unsigned int i;
	for (i=0; i < SLNet::L2MID_COUNT; i++)
	{
		SLNet::Lobby2Message *m = messageFactory.Alloc((SLNet::Lobby2MessageID)i);
		if (m)
		{
			printf("%i. %s", i+1, m->GetName());
			if (m->RequiresAdmin())
				printf(" (Admin command)");
			if (m->RequiresRankingPermission())
				printf(" (Ranking server command)");
			printf("\n");
			messageFactory.Dealloc(m);
		}		
		
	}
}

void ExecuteCommand(SLNet::Lobby2MessageID command, SLNet::RakString userName, int instanceNumber);
struct Lobby2ClientSampleCB : public SLNet::Lobby2Printf
{
	virtual void ExecuteDefaultResult(SLNet::Lobby2Message *message) {
		message->DebugPrintf();
		if (message->resultCode== SLNet::REC_SUCCESS && executionPlan.Size())
		{
			AutoExecutionPlanNode aepn = executionPlan.Pop();
			ExecuteCommand(aepn.operation, SLNet::RakString("user%i", aepn.instanceNumber), aepn.instanceNumber);
		}
	}
} callback[NUM_CONNECTIONS];

int main()
{
	printf("This sample creates two Lobby2Clients.\n");
	printf("They both connect to the server and performs queued operations on startup.");
	printf("(RANKING AND CLANS NOT YET DONE).\n");
	printf("Difficulty: Advanced\n\n");

	SLNet::Lobby2ResultCodeDescription::Validate();

	/// Do all these operations in this order once we are logged in.
	/// This is for easier testing.
	/// This plan will create the database, register two users, and log them both in
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_System_CreateDatabase), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_System_CreateTitle), _FILE_AND_LINE_ );

	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_CDKey_Add), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_System_RegisterProfanity), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Client_RegisterAccount), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Client_RegisterAccount), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_System_SetEmailAddressValidated), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_System_SetEmailAddressValidated), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Client_Login), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Client_Login), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Emails_Send), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Emails_Get), _FILE_AND_LINE_ );
// 	/// Create 2 clans
// 	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_Create), _FILE_AND_LINE_ );
// 	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_Create), _FILE_AND_LINE_ );
// 	// Invite to both
// 	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SendJoinInvitation), _FILE_AND_LINE_ );
// 	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SendJoinInvitation), _FILE_AND_LINE_ );
// 	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_RejectJoinInvitation), _FILE_AND_LINE_ );
// 	// Download invitations this clan has sent
// 	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_DownloadInvitationList), _FILE_AND_LINE_ );

	/*

	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Client_SetPresence), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Client_GetAccountDetails), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Client_PerTitleIntegerStorage), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Client_PerTitleIntegerStorage), _FILE_AND_LINE_ );

	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Client_StartIgnore), _FILE_AND_LINE_ );
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Client_GetIgnoreList), _FILE_AND_LINE_ );

	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Friends_SendInvite), _FILE_AND_LINE_);
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Friends_AcceptInvite), _FILE_AND_LINE_);

	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Ranking_SubmitMatch));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Ranking_SubmitMatch));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Ranking_UpdateRating));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Ranking_GetRating));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Ranking_WipeRatings));
	*/
// 	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_Create), _FILE_AND_LINE_ );
// 	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_Get), _FILE_AND_LINE_ );
	/*
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SetProperties));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SetMyMemberProperties));
	*/
	/*
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SendJoinInvitation));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_WithdrawJoinInvitation));
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_DownloadInvitationList));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SendJoinInvitation));
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_RejectJoinInvitation));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SendJoinInvitation));
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_AcceptJoinInvitation));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SetSubleaderStatus));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_SetMemberRank));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_GrantLeader));
	*/

	/*
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_SendJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_WithdrawJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_AcceptJoinRequest));
	*/

//	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_SendJoinRequest));
//	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_DownloadRequestList));
	// TODO - test from here
	/*
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_RejectJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_AcceptJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_SendJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_AcceptJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_KickAndBlacklistUser));
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_SendJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_GetBlacklist));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_UnblacklistUser));
	executionPlan.Push(AutoExecutionPlanNode(1, SLNet::L2MID_Clans_SendJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_AcceptJoinRequest));
	executionPlan.Push(AutoExecutionPlanNode(0, SLNet::L2MID_Clans_GetMembers));
	*/

	/*
	// TODO
	L2MID_Clans_CreateBoard,
		L2MID_Clans_DestroyBoard,
		L2MID_Clans_CreateNewTopic,
		L2MID_Clans_ReplyToTopic,
		L2MID_Clans_RemovePost,
		L2MID_Clans_GetBoards,
		L2MID_Clans_GetTopics,
		L2MID_Clans_GetPosts,
		*/
	

	char ip[64], serverPort[30], clientPort[30];
	int i;
	for (i=0; i < NUM_CONNECTIONS; i++)
		rakPeer[i]= SLNet::RakPeerInterface::GetInstance();
	puts("Enter the rakPeer1 port to listen on");
	clientPort[0]=0;
	const int intClientPort = atoi(clientPort);
	if ((intClientPort < 0) || (intClientPort > std::numeric_limits<unsigned short>::max())) {
		printf("Specified client port %d is outside valid bounds [0, %u]", intClientPort, std::numeric_limits<unsigned short>::max());
		return 1;
	}
	SLNet::SocketDescriptor socketDescriptor(static_cast<unsigned short>(intClientPort),0);
	Gets(clientPort,sizeof(clientPort));
	if (clientPort[0]==0)
		strcpy_s(clientPort, "0");

	puts("Enter IP to connect to");;
	ip[0]=0;
	Gets(ip,sizeof(ip));
	if (ip[0]==0)
		strcpy_s(ip, "127.0.0.1");

	puts("Enter the port to connect to");
	serverPort[0]=0;
	Gets(serverPort,sizeof(serverPort));
	if (serverPort[0]==0)
		strcpy_s(serverPort, "61111");
	const int intServerPort = atoi(serverPort);
	if ((intServerPort < 0) || (intServerPort > std::numeric_limits<unsigned short>::max())) {
		printf("Specified server port %d is outside valid bounds [0, %u]", intServerPort, std::numeric_limits<unsigned short>::max());
		return 2;
	}

	for (i=0; i < NUM_CONNECTIONS; i++)
	{
		rakPeer[i]->Startup(1,&socketDescriptor, 1);
		rakPeer[i]->Connect(ip, static_cast<unsigned short>(intServerPort), 0,0);

		rakPeer[i]->AttachPlugin(&lobby2Client[i]);
		lobby2Client[i].SetMessageFactory(&messageFactory);
		lobby2Client[i].SetCallbackInterface(&callback[i]);
		testUserName[i]= SLNet::RakString("user%i", i);
	}

	SLNet::Packet *packet;
	// Loop for input
	for(;;)
	{
		for (i=0; i < NUM_CONNECTIONS; i++)
		{
			SLNet::RakPeerInterface *peer = rakPeer[i];
			for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
			{
				switch (packet->data[0])
				{
				case ID_DISCONNECTION_NOTIFICATION:
					// Connection lost normally
					printf("ID_DISCONNECTION_NOTIFICATION\n");
					break;
				case ID_ALREADY_CONNECTED:
					// Connection lost normally
					printf("ID_ALREADY_CONNECTED\n");
					break;
				case ID_CONNECTION_BANNED: // Banned from this server
					printf("We are banned from this server.\n");
					break;			
				case ID_CONNECTION_ATTEMPT_FAILED:
					printf("Connection attempt failed\n");
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					// Sorry, the server is full.  I don't do anything here but
					// A real app should tell the user
					printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
					break;
				case ID_INVALID_PASSWORD:
					printf("ID_INVALID_PASSWORD\n");
					break;
				case ID_CONNECTION_LOST:
					// Couldn't deliver a reliable packet - i.e. the other system was abnormally
					// terminated
					printf("ID_CONNECTION_LOST\n");
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					// This tells the rakPeer1 they have connected
					printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
					int j;
					for (j=0; j < NUM_CONNECTIONS; j++)
						lobby2Client[j].SetServerAddress(packet->systemAddress);
					if (i==NUM_CONNECTIONS-1)
					{
						PrintCommands();
						printf("Enter instance number 1 to %i followed by command number.\n", NUM_CONNECTIONS);

						if (executionPlan.Size())
						{
							/// Execute the first command now that both clients have connected.
							AutoExecutionPlanNode aepn = executionPlan.Pop();
							ExecuteCommand(aepn.operation, SLNet::RakString("user%i", aepn.instanceNumber), aepn.instanceNumber);
						}
					}
					break;
				case ID_LOBBY2_SERVER_ERROR:
					{
					SLNet::BitStream bs(packet->data,packet->length,false);
						bs.IgnoreBytes(2); // ID_LOBBY2_SERVER_ERROR and error code
						printf("ID_LOBBY2_SERVER_ERROR: ");
						if (packet->data[1]== SLNet::L2SE_UNKNOWN_MESSAGE_ID)
						{
							unsigned int messageId;
							bs.Read(messageId);
							printf("L2SE_UNKNOWN_MESSAGE_ID %i", messageId);
						}
						else
							printf("Unknown");
						printf("\n");				
					}

					break;
				}
			}
		}
		
		
		// This sleep keeps RakNet responsive
		RakSleep(30);

		if (_kbhit())
		{
			int ch = _getch();
			if (ch <= '0' || ch > '9')
			{
				printf("Bad instance number\n");
				continue;
			}
			int instanceNumber = ch - 1 - '0';
			if (instanceNumber >= NUM_CONNECTIONS)
			{
				printf("Enter between 1 and %i to pick the instance of RakPeer to run\n", 1+NUM_CONNECTIONS);
				continue;
			}

			printf("Enter message number or 'quit' to quit.\n");
			char str[128];
			Gets(str, sizeof(str));
			if (_stricmp(str, "quit")==0)
			{
				printf("Quitting.\n");
				break;
			}
			else
			{
				int command = atoi(str);
				if (command <=0 || command > SLNet::L2MID_COUNT)
				{
					printf("Invalid message index %i. Commands:\n", command);
					PrintCommands();
				}
				else
				{
					ExecuteCommand((SLNet::Lobby2MessageID)(command-1), SLNet::RakString("user%i", instanceNumber), instanceNumber);
				}
			}
		}
	}

	for (i=0; i < NUM_CONNECTIONS; i++)
		SLNet::RakPeerInterface::DestroyInstance(rakPeer[i]);
	return 0;
}
/// In a real application these parameters would be filled out from application data
/// Here I've just hardcoded everything for fast testing
void ExecuteCommand(SLNet::Lobby2MessageID command, SLNet::RakString userName, int instanceNumber)
{
	SLNet::Lobby2Message *m = messageFactory.Alloc(command);
	RakAssert(m);
	printf("Executing %s (message %i)\n", m->GetName(), command+1);
	// If additional requires are needed to test the command, stick them here
	switch (m->GetID())
	{
	case SLNet::L2MID_System_CreateTitle:
		{
			SLNet::System_CreateTitle *arg = (SLNet::System_CreateTitle *) m;
			arg->requiredAge=22;
			arg->titleName="Test Title Name";
			arg->titleSecretKey="Test secret key";
		}
		break;
	case SLNet::L2MID_System_DestroyTitle:
		{
			SLNet::System_DestroyTitle *arg = (SLNet::System_DestroyTitle *) m;
			arg->titleName="Test Title Name";
		}
		break;
	case SLNet::L2MID_System_GetTitleRequiredAge:
		{
			SLNet::System_GetTitleRequiredAge *arg = (SLNet::System_GetTitleRequiredAge *) m;
			arg->titleName="Test Title Name";
		}
		break;
	case SLNet::L2MID_System_GetTitleBinaryData:
		{
			SLNet::System_GetTitleBinaryData *arg = (SLNet::System_GetTitleBinaryData *) m;
			arg->titleName="Test Title Name";
		}
		break;
	case SLNet::L2MID_System_RegisterProfanity:
		{
			SLNet::System_RegisterProfanity *arg = (SLNet::System_RegisterProfanity *) m;
			arg->profanityWords.Insert("Bodily Functions", _FILE_AND_LINE_ );
			arg->profanityWords.Insert("Racial Epithet", _FILE_AND_LINE_ );
			arg->profanityWords.Insert("Euphemism treadmill", _FILE_AND_LINE_ );
		}
		break;
	case SLNet::L2MID_System_BanUser:
		{
			SLNet::System_BanUser *arg = (SLNet::System_BanUser *) m;
			arg->durationHours=12;
			arg->banReason="Ban Reason";
			arg->userName=userName;
		}
		break;
	case SLNet::L2MID_System_UnbanUser:
		{
			SLNet::System_UnbanUser *arg = (SLNet::System_UnbanUser *) m;
			arg->reason="Unban Reason";
			arg->userName=userName;
		}
		break;
	case SLNet::L2MID_CDKey_Add:
		{
			SLNet::CDKey_Add *arg = (SLNet::CDKey_Add *) m;
			arg->cdKeys.Insert("Test CD Key", _FILE_AND_LINE_ );
			arg->cdKeys.Insert("Test CD Key 2", _FILE_AND_LINE_ );
			arg->titleName="Test Title Name";
		}
		break;
	case SLNet::L2MID_CDKey_GetStatus:
		{
			SLNet::CDKey_GetStatus *arg = (SLNet::CDKey_GetStatus *) m;
			arg->cdKey="Test CD Key";
			arg->titleName="Test Title Name";
		}
		break;
	case SLNet::L2MID_CDKey_Use:
		{
			SLNet::CDKey_Use *arg = (SLNet::CDKey_Use *) m;
			arg->cdKey="Test CD Key";
			arg->titleName="Test Title Name";
			arg->userName=userName;
		}
		break;
	case SLNet::L2MID_CDKey_FlagStolen:
		{
			SLNet::CDKey_FlagStolen *arg = (SLNet::CDKey_FlagStolen *) m;
			arg->cdKey="Test CD Key";
			arg->titleName="Test Title Name";
			arg->wasStolen=true;
		}
		break;
	case SLNet::L2MID_Client_Login:
		{
			SLNet::Client_Login *arg = (SLNet::Client_Login *) m;
			arg->titleName="Test Title Name";
			arg->titleSecretKey="Test secret key";
			arg->userPassword="asdf";
			arg->userName=userName;
		}
		break;
	case SLNet::L2MID_Client_SetPresence:
		{
			SLNet::Client_SetPresence *arg = (SLNet::Client_SetPresence *) m;
			arg->presence.isVisible=true;
			arg->presence.status= SLNet::Lobby2Presence::IN_LOBBY;
//			arg->presence.titleName="Test Title Name";
		}
		break;
	case SLNet::L2MID_Client_RegisterAccount:
		{
			SLNet::Client_RegisterAccount *arg = (SLNet::Client_RegisterAccount *) m;
			arg->createAccountParameters.ageInDays=9999;
			arg->createAccountParameters.firstName="Firstname";
			arg->createAccountParameters.lastName="Lastname";
			arg->createAccountParameters.password="asdf";
			arg->createAccountParameters.passwordRecoveryQuestion="1+2=?";
			arg->createAccountParameters.passwordRecoveryAnswer="3";
			arg->createAccountParameters.emailAddress="username@provider.com";
			arg->createAccountParameters.homeCountry="United States";
			arg->createAccountParameters.homeState="california";
			arg->createAccountParameters.sex_male=true;
			arg->userName=userName;
			arg->cdKey="Test CD Key";
			arg->titleName="Test Title Name";
		}
		break;
	case SLNet::L2MID_System_SetEmailAddressValidated:
		{
			SLNet::System_SetEmailAddressValidated *arg = (SLNet::System_SetEmailAddressValidated *) m;
			arg->validated=true;
			arg->userName=userName;
		}
		break;
	case SLNet::L2MID_Client_ValidateHandle:
		{
			SLNet::Client_ValidateHandle *arg = (SLNet::Client_ValidateHandle *) m;
			arg->userName=userName;
		}
		break;

	case SLNet::L2MID_System_DeleteAccount:
		{
			SLNet::System_DeleteAccount *arg = (SLNet::System_DeleteAccount *) m;
			arg->userName=userName;
			arg->password="asdf";
		}
		break;

	case SLNet::L2MID_System_PruneAccounts:
		{
			SLNet::System_PruneAccounts *arg = (SLNet::System_PruneAccounts *) m;
			arg->deleteAccountsNotLoggedInDays=1;
		}
		break;

	case SLNet::L2MID_Client_GetEmailAddress:
		{
			SLNet::Client_GetEmailAddress *arg = (SLNet::Client_GetEmailAddress *) m;
			arg->userName=userName;
		}
		break;

	case SLNet::L2MID_Client_GetPasswordRecoveryQuestionByHandle:
		{
			SLNet::Client_GetPasswordRecoveryQuestionByHandle *arg = (SLNet::Client_GetPasswordRecoveryQuestionByHandle *) m;
			arg->userName=userName;
		}
		break;

	case SLNet::L2MID_Client_GetPasswordByPasswordRecoveryAnswer:
		{
			SLNet::Client_GetPasswordByPasswordRecoveryAnswer *arg = (SLNet::Client_GetPasswordByPasswordRecoveryAnswer *) m;
			arg->userName=userName;
			arg->passwordRecoveryAnswer="3";
		}
		break;

	case SLNet::L2MID_Client_ChangeHandle:
		{
			SLNet::Client_ChangeHandle *arg = (SLNet::Client_ChangeHandle *) m;
			arg->userName=userName;
			arg->newHandle="New user handle";
		}
		break;

	case SLNet::L2MID_Client_UpdateAccount:
		{
			// provided for documentation purposes only
			// SLNet::Client_UpdateAccount *arg = (SLNet::Client_UpdateAccount *) m;
		}
		break;

	case SLNet::L2MID_Client_GetAccountDetails:
		{
			// provided for documentation purposes only
			// SLNet::Client_GetAccountDetails *arg = (SLNet::Client_GetAccountDetails *) m;
		}
		break;

	case SLNet::L2MID_Client_StartIgnore:
		{
			SLNet::Client_StartIgnore *arg = (SLNet::Client_StartIgnore *) m;
			arg->targetHandle=SLNet::RakString("user%i", instanceNumber+1);
		}
		break;

	case SLNet::L2MID_Client_StopIgnore:
		{
			SLNet::Client_StopIgnore *arg = (SLNet::Client_StopIgnore *) m;
			arg->targetHandle=SLNet::RakString("user%i", instanceNumber+1);
		}
		break;

	case SLNet::L2MID_Client_GetIgnoreList:
		{
			// provided for documentation purposes only
			// SLNet::Client_GetIgnoreList *arg = (SLNet::Client_GetIgnoreList *) m;
		}
		break;

	case SLNet::L2MID_Client_PerTitleIntegerStorage:
		{
		SLNet::Client_PerTitleIntegerStorage *arg = (SLNet::Client_PerTitleIntegerStorage *) m;
			arg->titleName="Test Title Name";
			arg->slotIndex=0;
			arg->conditionValue=1.0;
			arg->addConditionForOperation= SLNet::Client_PerTitleIntegerStorage::PTISC_GREATER_THAN;
			arg->inputValue=0.0;
			static int runCount=0;
			if (runCount++%2==0)
				arg->operationToPerform= SLNet::Client_PerTitleIntegerStorage::PTISO_WRITE;
			else
				arg->operationToPerform= SLNet::Client_PerTitleIntegerStorage::PTISO_READ;
		}
		break;

	case SLNet::L2MID_Friends_SendInvite:
		{
		SLNet::Friends_SendInvite *arg = (SLNet::Friends_SendInvite *) m;
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->subject="Friends_SendInvite subject";
			arg->body="Friends_SendInvite body";
		}
		break;

	case SLNet::L2MID_Friends_AcceptInvite:
		{
		SLNet::Friends_AcceptInvite *arg = (SLNet::Friends_AcceptInvite *) m;
			arg->targetHandle= SLNet::RakString("user%i", 0);
			arg->subject="Friends_AcceptInvite subject";
			arg->body="Friends_AcceptInvite body";
			arg->emailStatus=0;
		}
		break;

	case SLNet::L2MID_Friends_RejectInvite:
		{
		SLNet::Friends_RejectInvite *arg = (SLNet::Friends_RejectInvite *) m;
			arg->targetHandle= SLNet::RakString("user%i", 0);
			arg->subject="L2MID_Friends_RejectInvite subject";
			arg->body="L2MID_Friends_RejectInvite body";
			arg->emailStatus=0;
		}
		break;

	case SLNet::L2MID_Friends_GetInvites:
		{
			// provided for documentation purposes only
			// SLNet::Friends_GetInvites *arg = (SLNet::Friends_GetInvites *) m;
		}
		break;

	case SLNet::L2MID_Friends_GetFriends:
		{
			// provided for documentation purposes only
			// SLNet::Friends_GetFriends *arg = (SLNet::Friends_GetFriends *) m;
		}
		break;

	case SLNet::L2MID_Friends_Remove:
		{
		SLNet::Friends_Remove *arg = (SLNet::Friends_Remove *) m;
			arg->targetHandle= SLNet::RakString("user%i", 0);
			arg->subject="L2MID_Friends_Remove subject";
			arg->body="L2MID_Friends_Remove body";
			arg->emailStatus=0;
		}
		break;

	case SLNet::L2MID_BookmarkedUsers_Add:
		{
		SLNet::BookmarkedUsers_Add *arg = (SLNet::BookmarkedUsers_Add *) m;
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->type=0;
			arg->description="L2MID_BookmarkedUsers_Add description";
		}
		break;
	case SLNet::L2MID_BookmarkedUsers_Remove:
		{
		SLNet::BookmarkedUsers_Remove *arg = (SLNet::BookmarkedUsers_Remove *) m;
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->type=0;
		}
		break;
	case SLNet::L2MID_BookmarkedUsers_Get:
		{
			// provided for documentation purposes only
			// SLNet::BookmarkedUsers_Get *arg = (SLNet::BookmarkedUsers_Get *) m;
		}
		break;

	case SLNet::L2MID_Emails_Send:
		{
		SLNet::Emails_Send *arg = (SLNet::Emails_Send *) m;
			arg->recipients.Insert(SLNet::RakString("user%i", instanceNumber+1), _FILE_AND_LINE_ );
			arg->recipients.Insert(SLNet::RakString("user%i", instanceNumber+2), _FILE_AND_LINE_ );
			arg->subject="L2MID_Emails_Send subject";
			arg->body="L2MID_Emails_Send body";
			arg->status=0;
		}
		break;

	case SLNet::L2MID_Emails_Get:
		{
		SLNet::Emails_Get *arg = (SLNet::Emails_Get *) m;
			arg->unreadEmailsOnly=true;
			arg->emailIdsOnly=true;
		}
		break;

	case SLNet::L2MID_Emails_Delete:
		{
		SLNet::Emails_Delete *arg = (SLNet::Emails_Delete *) m;
			arg->emailId=1;
		}
		break;

	case SLNet::L2MID_Emails_SetStatus:
		{
		SLNet::Emails_SetStatus *arg = (SLNet::Emails_SetStatus *) m;
			arg->emailId=2;
			arg->updateStatusFlag=true;
			arg->updateMarkedRead=true;
			arg->newStatusFlag=1234;
			arg->isNowMarkedRead=true;
		}
		break;

	case SLNet::L2MID_Ranking_SubmitMatch:
		{
		SLNet::Ranking_SubmitMatch *arg = (SLNet::Ranking_SubmitMatch *) m;
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
			arg->submittedMatch.matchNote="Ranking match note";
			arg->submittedMatch.matchParticipants.Insert(SLNet::MatchParticipant("user0", 5.0f), _FILE_AND_LINE_ );
			arg->submittedMatch.matchParticipants.Insert(SLNet::MatchParticipant("user1", 10.0f), _FILE_AND_LINE_ );
		}
		break;

	case SLNet::L2MID_Ranking_GetMatches:
		{
		SLNet::Ranking_GetMatches *arg = (SLNet::Ranking_GetMatches *) m;
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
		}
		break;

	case SLNet::L2MID_Ranking_GetMatchBinaryData:
		{
		SLNet::Ranking_GetMatchBinaryData *arg = (SLNet::Ranking_GetMatchBinaryData *) m;
			arg->matchID=1;
		}
		break;

	case SLNet::L2MID_Ranking_GetTotalScore:
		{
		SLNet::Ranking_GetTotalScore *arg = (SLNet::Ranking_GetTotalScore *) m;
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber);
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
		}
		break;

	case SLNet::L2MID_Ranking_WipeScoresForPlayer:
		{
		SLNet::Ranking_WipeScoresForPlayer *arg = (SLNet::Ranking_WipeScoresForPlayer *) m;
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber);
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
		}
		break;

	case SLNet::L2MID_Ranking_WipeMatches:
		{
		SLNet::Ranking_WipeMatches *arg = (SLNet::Ranking_WipeMatches *) m;
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
		}
		break;

	case SLNet::L2MID_Ranking_PruneMatches:
		{
		SLNet::Ranking_PruneMatches *arg = (SLNet::Ranking_PruneMatches *) m;
			arg->pruneTimeDays=1;
		}
		break;

	case SLNet::L2MID_Ranking_UpdateRating:
		{
		SLNet::Ranking_UpdateRating *arg = (SLNet::Ranking_UpdateRating *) m;
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber);
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
			arg->targetRating=1234.0f;
		}
		break;

	case SLNet::L2MID_Ranking_WipeRatings:
		{
		SLNet::Ranking_WipeRatings *arg = (SLNet::Ranking_WipeRatings *) m;
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
		}
		break;

	case SLNet::L2MID_Ranking_GetRating:
		{
		SLNet::Ranking_GetRating *arg = (SLNet::Ranking_GetRating *) m;
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber);
			arg->gameType="Match game type";
			arg->titleName="Test Title Name";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber);
		}
		break;

	case SLNet::L2MID_Clans_Create:
		{
		SLNet::Clans_Create *arg = (SLNet::Clans_Create *) m;
			static int idx=0;
			arg->clanHandle= SLNet::RakString("Clan handle %i", idx++);
			arg->failIfAlreadyInClan=false;
			arg->requiresInvitationsToJoin=true;
			arg->description="Clan Description";
			arg->binaryData->binaryData=new char[10];
			strcpy_s(arg->binaryData->binaryData,arg->binaryData->binaryDataLength,"Hello");
			arg->binaryData->binaryDataLength=10;
		}
		break;

	case SLNet::L2MID_Clans_SetProperties:
		{
		SLNet::Clans_SetProperties *arg = (SLNet::Clans_SetProperties *) m;
			arg->clanHandle="Clan handle";
			arg->description="Updated description";
		}
		break;

	case SLNet::L2MID_Clans_GetProperties:
		{
		SLNet::Clans_GetProperties *arg = (SLNet::Clans_GetProperties *) m;
			arg->clanHandle="Clan handle";
		}
		break;

	case SLNet::L2MID_Clans_SetMyMemberProperties:
		{
		SLNet::Clans_SetMyMemberProperties *arg = (SLNet::Clans_SetMyMemberProperties *) m;
			arg->clanHandle="Clan handle";
			arg->description="Updated description";
		}
		break;

	case SLNet::L2MID_Clans_GrantLeader:
		{
		SLNet::Clans_GrantLeader *arg = (SLNet::Clans_GrantLeader *) m;
			arg->clanHandle="Clan handle";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
		}
		break;

	case SLNet::L2MID_Clans_SetSubleaderStatus:
		{
		SLNet::Clans_SetSubleaderStatus *arg = (SLNet::Clans_SetSubleaderStatus *) m;
			arg->clanHandle="Clan handle";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->setToSubleader=true;
		}
		break;

	case SLNet::L2MID_Clans_SetMemberRank:
		{
		SLNet::Clans_SetMemberRank *arg = (SLNet::Clans_SetMemberRank *) m;
			arg->clanHandle="Clan handle";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->newRank=666;
		}
		break;

	case SLNet::L2MID_Clans_GetMemberProperties:
		{
		SLNet::Clans_GetMemberProperties *arg = (SLNet::Clans_GetMemberProperties *) m;
			arg->clanHandle="Clan handle";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber);
		}
		break;

	case SLNet::L2MID_Clans_ChangeHandle:
		{
		SLNet::Clans_ChangeHandle *arg = (SLNet::Clans_ChangeHandle *) m;
			arg->oldClanHandle="Clan handle";
			arg->newClanHandle="New Clan handle";
		}
		break;

	case SLNet::L2MID_Clans_Leave:
		{
		SLNet::Clans_Leave *arg = (SLNet::Clans_Leave *) m;
			arg->clanHandle="Clan handle";
			arg->dissolveIfClanLeader=false;
			arg->subject="L2MID_Clans_Leave";
			arg->emailStatus=0;
		}
		break;

	case SLNet::L2MID_Clans_Get:
		{
			// provided for documentation purposes only
			// SLNet::Clans_Get *arg = (SLNet::Clans_Get *) m;
		}
		break;

	case SLNet::L2MID_Clans_SendJoinInvitation:
		{
		SLNet::Clans_SendJoinInvitation *arg = (SLNet::Clans_SendJoinInvitation *) m;
			static int idx=0;
			arg->clanHandle= SLNet::RakString("Clan handle %i", idx++);
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->subject="L2MID_Clans_SendJoinInvitation";
		}
		break;

	case SLNet::L2MID_Clans_WithdrawJoinInvitation:
		{
		SLNet::Clans_WithdrawJoinInvitation *arg = (SLNet::Clans_WithdrawJoinInvitation *) m;
			arg->clanHandle="Clan handle";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->subject="L2MID_Clans_WithdrawJoinInvitation";
		}
		break;

	case SLNet::L2MID_Clans_AcceptJoinInvitation:
		{
		SLNet::Clans_AcceptJoinInvitation *arg = (SLNet::Clans_AcceptJoinInvitation *) m;
			static int idx=0;
			arg->clanHandle= SLNet::RakString("Clan handle %i", idx++);
			arg->subject="L2MID_Clans_AcceptJoinInvitation";
			arg->failIfAlreadyInClan=false;
		}
		break;

	case SLNet::L2MID_Clans_RejectJoinInvitation:
		{
		SLNet::Clans_RejectJoinInvitation *arg = (SLNet::Clans_RejectJoinInvitation *) m;
			static int idx=0;
			arg->clanHandle= SLNet::RakString("Clan handle %i", idx++);
			arg->subject="L2MID_Clans_WithdrawJoinInvitation";
		}
		break;

	case SLNet::L2MID_Clans_DownloadInvitationList:
		{
			// provided for documentation purposes only
			// SLNet::Clans_DownloadInvitationList *arg = (SLNet::Clans_DownloadInvitationList *) m;
		}
		break;

	case SLNet::L2MID_Clans_SendJoinRequest:
		{
		SLNet::Clans_SendJoinRequest *arg = (SLNet::Clans_SendJoinRequest *) m;
			arg->clanHandle="Clan handle";
			arg->subject="L2MID_Clans_SendJoinRequest";
		}
		break;

	case SLNet::L2MID_Clans_WithdrawJoinRequest:
		{
		SLNet::Clans_WithdrawJoinRequest *arg = (SLNet::Clans_WithdrawJoinRequest *) m;
			arg->clanHandle="Clan handle";
			arg->subject="L2MID_Clans_WithdrawJoinRequest";
		}
		break;

	case SLNet::L2MID_Clans_AcceptJoinRequest:
		{
		SLNet::Clans_AcceptJoinRequest *arg = (SLNet::Clans_AcceptJoinRequest *) m;
			arg->clanHandle="Clan handle";
			arg->requestingUserHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->subject="L2MID_Clans_AcceptJoinRequest";
		}
		break;

	case SLNet::L2MID_Clans_RejectJoinRequest:
		{
		SLNet::Clans_RejectJoinRequest *arg = (SLNet::Clans_RejectJoinRequest *) m;
			arg->clanHandle="Clan handle";
			arg->requestingUserHandle= SLNet::RakString("user%i", instanceNumber+1);
		}
		break;

	case SLNet::L2MID_Clans_DownloadRequestList:
		{
			// provided for documentation purposes only
			// SLNet::Clans_DownloadRequestList *arg = (SLNet::Clans_DownloadRequestList *) m;
		}
		break;

	case SLNet::L2MID_Clans_KickAndBlacklistUser:
		{
		SLNet::Clans_KickAndBlacklistUser *arg = (SLNet::Clans_KickAndBlacklistUser *) m;
			arg->clanHandle="Clan handle";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
			arg->kick=true;
			arg->blacklist=true;
		}
		break;

	case SLNet::L2MID_Clans_UnblacklistUser:
		{
		SLNet::Clans_UnblacklistUser *arg = (SLNet::Clans_UnblacklistUser *) m;
			arg->clanHandle="Clan handle";
			arg->targetHandle= SLNet::RakString("user%i", instanceNumber+1);
		}
		break;

	case SLNet::L2MID_Clans_GetBlacklist:
		{
		SLNet::Clans_GetBlacklist *arg = (SLNet::Clans_GetBlacklist *) m;
			arg->clanHandle="Clan handle";
		}
		break;

	case SLNet::L2MID_Clans_GetMembers:
		{
		SLNet::Clans_GetMembers *arg = (SLNet::Clans_GetMembers *) m;
			arg->clanHandle="Clan handle";
		}
		break;

	case SLNet::L2MID_Clans_CreateBoard:
		{
			// provided for documentation purposes only
			// SLNet::Clans_CreateBoard *arg = (SLNet::Clans_CreateBoard *) m;
		}
		break;

	case SLNet::L2MID_Clans_DestroyBoard:
		{
			// provided for documentation purposes only
			// SLNet::Clans_DestroyBoard *arg = (SLNet::Clans_DestroyBoard *) m;
		}
		break;

	case SLNet::L2MID_Clans_CreateNewTopic:
		{
			// provided for documentation purposes only
			// SLNet::Clans_CreateNewTopic *arg = (SLNet::Clans_CreateNewTopic *) m;
		}
		break;

	case SLNet::L2MID_Clans_ReplyToTopic:
		{
			// provided for documentation purposes only
			// SLNet::Clans_ReplyToTopic *arg = (SLNet::Clans_ReplyToTopic *) m;
		}
		break;

	case SLNet::L2MID_Clans_RemovePost:
		{
			// provided for documentation purposes only
			// SLNet::Clans_RemovePost *arg = (SLNet::Clans_RemovePost *) m;
		}
		break;

	case SLNet::L2MID_Clans_GetBoards:
		{
			// provided for documentation purposes only
			// SLNet::Clans_GetBoards *arg = (SLNet::Clans_GetBoards *) m;
		}
		break;

	case SLNet::L2MID_Clans_GetTopics:
		{
			// provided for documentation purposes only
			// SLNet::Clans_GetTopics *arg = (SLNet::Clans_GetTopics *) m;
		}
		break;

	case SLNet::L2MID_Clans_GetPosts:
		{
			// provided for documentation purposes only
			// SLNet::Clans_GetPosts *arg = (SLNet::Clans_GetPosts *) m;
		}
		break;
	}
	lobby2Client[instanceNumber].SendMsg(m);
	messageFactory.Dealloc(m);
}
