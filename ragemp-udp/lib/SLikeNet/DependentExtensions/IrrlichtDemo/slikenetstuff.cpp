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

#include "slikenetstuff.h"

#include "slikenet/NetworkIDManager.h"
#include "CDemo.h"
#include "slikenet/time.h"
#include "slikenet/GetTime.h"
#include "slikenet/SocketLayer.h"
#include "slikenet/linux_adapter.h"
#include "slikenet/osx_adapter.h"

using namespace SLNet;

RakPeerInterface *rakPeer;
NetworkIDManager *networkIDManager;
ReplicaManager3Irrlicht *irrlichtReplicaManager3;
NatPunchthroughClient *natPunchthroughClient;
CloudClient *cloudClient;
SLNet::FullyConnectedMesh2 *fullyConnectedMesh2;
PlayerReplica *playerReplica;

/*
class DebugBoxSceneNode : public scene::ISceneNode 
{
public:
	DebugBoxSceneNode(scene::ISceneNode* parent,
		scene::ISceneManager* mgr,
		s32 id = -1);
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual void OnRegisterSceneNode();
	virtual void render();

	CDemo *demo;
};
DebugBoxSceneNode::DebugBoxSceneNode(
									 scene::ISceneNode* parent,
									 scene::ISceneManager* mgr,
									 s32 id)
									 : scene::ISceneNode(parent, mgr, id)
{
#ifdef _DEBUG
	setDebugName("DebugBoxSceneNode");
#endif
	setAutomaticCulling(scene::EAC_OFF);
} 
const core::aabbox3d<f32>& DebugBoxSceneNode::getBoundingBox() const
{
	return demo->GetSyndeyBoundingBox();
}
void DebugBoxSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		demo->GetSceneManager()->registerNodeForRendering(this, scene::ESNRP_SOLID);
}
void DebugBoxSceneNode::render()
{
	if (DebugDataVisible)
	{ 
		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation); 

		video::SMaterial m;
		m.Lighting = false;
		demo->GetDevice()->getVideoDriver()->setMaterial(m);
		demo->GetDevice()->getVideoDriver()->draw3DBox(demo->GetSyndeyBoundingBox());
	}
}
*/

DataStructures::List<PlayerReplica*> PlayerReplica::playerList;

// Take this many milliseconds to move the visible position to the real position
static const float INTERP_TIME_MS=100.0f;

void InstantiateRakNetClasses(void)
{
	static const int MAX_PLAYERS=32;
	static const unsigned short TCP_PORT=0;
	static const SLNet::TimeMS UDP_SLEEP_TIMER=30;

	// Basis of all UDP communications
	rakPeer= SLNet::RakPeerInterface::GetInstance();
	// Using fixed port so we can use AdvertiseSystem and connect on the LAN if the server is not available.
	SLNet::SocketDescriptor sd(1234,0);
	sd.socketFamily=AF_INET; // Only IPV4 supports broadcast on 255.255.255.255
	while (IRNS2_Berkley::IsPortInUse(sd.port, sd.hostAddress, sd.socketFamily, SOCK_DGRAM)==true)
		sd.port++;
	// +1 is for the connection to the NAT punchthrough server
	SLNET_VERIFY(rakPeer->Startup(MAX_PLAYERS+1,&sd,1) == SLNet::RAKNET_STARTED);
	rakPeer->SetMaximumIncomingConnections(MAX_PLAYERS);
	// Fast disconnect for easier testing of host migration
	rakPeer->SetTimeoutTime(5000,UNASSIGNED_SYSTEM_ADDRESS);
	// ReplicaManager3 replies on NetworkIDManager. It assigns numbers to objects so they can be looked up over the network
	// It's a class in case you wanted to have multiple worlds, then you could have multiple instances of NetworkIDManager
	networkIDManager=new NetworkIDManager;
	// Automatically sends around new / deleted / changed game objects
	irrlichtReplicaManager3=new ReplicaManager3Irrlicht;
	irrlichtReplicaManager3->SetNetworkIDManager(networkIDManager);
	rakPeer->AttachPlugin(irrlichtReplicaManager3);
	// Automatically destroy connections, but don't create them so we have more control over when a system is considered ready to play
	irrlichtReplicaManager3->SetAutoManageConnections(false,true);
	// Create and register the network object that represents the player
	playerReplica = new PlayerReplica;
	irrlichtReplicaManager3->Reference(playerReplica);
	// Lets you connect through routers
	natPunchthroughClient=new NatPunchthroughClient;
	rakPeer->AttachPlugin(natPunchthroughClient);
	// Uploads game instance, basically client half of a directory server
	// Server code is in NATCompleteServer sample
	cloudClient=new CloudClient;
	rakPeer->AttachPlugin(cloudClient);
	fullyConnectedMesh2=new FullyConnectedMesh2;
	fullyConnectedMesh2->SetAutoparticipateConnections(false);
	fullyConnectedMesh2->SetConnectOnNewRemoteConnection(false, "");
	rakPeer->AttachPlugin(fullyConnectedMesh2);
	// Connect to the NAT punchthrough server
	SLNET_VERIFY(rakPeer->Connect(DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_IP, DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_PORT,0,0) == CONNECTION_ATTEMPT_STARTED);

	// Advertise ourselves on the lAN if the NAT punchthrough server is not available
 	//for (int i=0; i < 8; i++)
 	//	rakPeer->AdvertiseSystem("255.255.255.255", 1234+i, 0,0,0);
}
void DeinitializeRakNetClasses(void)
{
	// Shutdown so the server knows we stopped
	rakPeer->Shutdown(100,0);
	SLNet::RakPeerInterface::DestroyInstance(rakPeer);
	delete networkIDManager;
	delete irrlichtReplicaManager3;
	delete natPunchthroughClient;
	delete cloudClient;
	delete fullyConnectedMesh2;
	// ReplicaManager3 deletes all referenced objects, including this one
	//playerReplica->PreDestruction(0);
	//delete playerReplica;
}
BaseIrrlichtReplica::BaseIrrlichtReplica()
{
}
BaseIrrlichtReplica::~BaseIrrlichtReplica()
{

}
void BaseIrrlichtReplica::SerializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
{
	// unused parameters
	(void)destinationConnection;

	constructionBitstream->Write(position);
}
bool BaseIrrlichtReplica::DeserializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *sourceConnection)
{
	// unused parameters
	(void)sourceConnection;

	constructionBitstream->Read(position);
	return true;
}
RM3SerializationResult BaseIrrlichtReplica::Serialize(SLNet::SerializeParameters *serializeParameters)
{
	// unused parameters
	(void)serializeParameters;

	return RM3SR_BROADCAST_IDENTICALLY;
}
void BaseIrrlichtReplica::Deserialize(SLNet::DeserializeParameters *deserializeParameters)
{
	// unused parameters
	(void)deserializeParameters;
}
void BaseIrrlichtReplica::Update(SLNet::TimeMS curTime)
{
	// unused parameters
	(void)curTime;
}
PlayerReplica::PlayerReplica()
{
	model=0;
	rotationDeltaPerMS=0.0f;
	isMoving=false;
	deathTimeout=0;
	lastUpdate= SLNet::GetTimeMS();
	playerList.Push(this,_FILE_AND_LINE_);
}
PlayerReplica::~PlayerReplica()
{
	unsigned int index = playerList.GetIndexOf(this);
	if (index != (unsigned int) -1)
		playerList.RemoveAtIndexFast(index);
}
void PlayerReplica::WriteAllocationID(SLNet::Connection_RM3 *destinationConnection, SLNet::BitStream *allocationIdBitstream) const
{
	// unused parameters
	(void)destinationConnection;

	allocationIdBitstream->Write(SLNet::RakString("PlayerReplica"));
}
void PlayerReplica::SerializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
{
	BaseIrrlichtReplica::SerializeConstruction(constructionBitstream, destinationConnection);
	constructionBitstream->Write(rotationAroundYAxis);
	constructionBitstream->Write(playerName);
	constructionBitstream->Write(IsDead());
}
bool PlayerReplica::DeserializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *sourceConnection)
{
	if (!BaseIrrlichtReplica::DeserializeConstruction(constructionBitstream, sourceConnection))
		return false;
	constructionBitstream->Read(rotationAroundYAxis);
	constructionBitstream->Read(playerName);
	constructionBitstream->Read(isDead);
	return true;
}
void PlayerReplica::PostDeserializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
{
	// unused parameters
	(void)constructionBitstream;
	(void)destinationConnection;

	// Object was remotely created and all data loaded. Now we can make the object visible
	scene::IAnimatedMesh* mesh = 0;
	scene::ISceneManager *sm = demo->GetSceneManager();
	mesh = sm->getMesh(IRRLICHT_MEDIA_PATH "sydney.md2");
	model = sm->addAnimatedMeshSceneNode(mesh, 0);

//	DebugBoxSceneNode * debugBox = new DebugBoxSceneNode(model,sm);
//	debugBox->demo=demo;
//	debugBox->setDebugDataVisible(true); 

	model->setPosition(position);
	model->setRotation(core::vector3df(0, rotationAroundYAxis, 0));
	model->setScale(core::vector3df(2,2,2));
	model->setMD2Animation(scene::EMAT_STAND);
	curAnim=scene::EMAT_STAND;
	model->setMaterialTexture(0, demo->GetDevice()->getVideoDriver()->getTexture(IRRLICHT_MEDIA_PATH "sydney.bmp"));
	model->setMaterialFlag(video::EMF_LIGHTING, true);
	model->addShadowVolumeSceneNode();
	model->setAutomaticCulling ( scene::EAC_BOX );
	model->setVisible(true);
	model->setAnimationEndCallback(this);
	wchar_t playerNameWChar[1024];
	mbstowcs_s(NULL, playerNameWChar, playerName.C_String(), 1023);
	scene::IBillboardSceneNode *bb = sm->addBillboardTextSceneNode(0, playerNameWChar, model);
	bb->setSize(core::dimension2df(40,20));
	bb->setPosition(core::vector3df(0,model->getBoundingBox().MaxEdge.Y+bb->getBoundingBox().MaxEdge.Y-bb->getBoundingBox().MinEdge.Y+5.0f,0));
	bb->setColor(video::SColor(255,255,128,128), video::SColor(255,255,128,128));
}
void PlayerReplica::PreDestruction(SLNet::Connection_RM3 *sourceConnection)
{
	// unused parameters
	(void)sourceConnection;

	if (model)
		model->remove();
}
RM3SerializationResult PlayerReplica::Serialize(SLNet::SerializeParameters *serializeParameters)
{
	BaseIrrlichtReplica::Serialize(serializeParameters);
	serializeParameters->outputBitstream[0].Write(position);
	serializeParameters->outputBitstream[0].Write(rotationAroundYAxis);
	serializeParameters->outputBitstream[0].Write(isMoving);
	serializeParameters->outputBitstream[0].Write(IsDead());
	return RM3SR_BROADCAST_IDENTICALLY;
}
void PlayerReplica::Deserialize(SLNet::DeserializeParameters *deserializeParameters)
{
	BaseIrrlichtReplica::Deserialize(deserializeParameters);
	deserializeParameters->serializationBitstream[0].Read(position);
	deserializeParameters->serializationBitstream[0].Read(rotationAroundYAxis);
	deserializeParameters->serializationBitstream[0].Read(isMoving);
	bool wasDead=isDead;
	deserializeParameters->serializationBitstream[0].Read(isDead);
	if (isDead==true && wasDead==false)
	{
		demo->PlayDeathSound(position);
	}

	core::vector3df positionOffset;
	positionOffset=position-model->getPosition();
	positionDeltaPerMS = positionOffset / INTERP_TIME_MS;
	float rotationOffset;
	rotationOffset=GetRotationDifference(rotationAroundYAxis,model->getRotation().Y);
	rotationDeltaPerMS = rotationOffset / INTERP_TIME_MS;
	interpEndTime = SLNet::GetTimeMS() + (SLNet::TimeMS) INTERP_TIME_MS;
}
void PlayerReplica::Update(SLNet::TimeMS curTime)
{
	// Is a locally created object?
	if (creatingSystemGUID==rakPeer->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS))
	{
		// Local player has no mesh to interpolate
		// Input our camera position as our player position
		playerReplica->position=demo->GetSceneManager()->getActiveCamera()->getPosition()-irr::core::vector3df(0,CAMERA_HEIGHT,0);
		playerReplica->rotationAroundYAxis=demo->GetSceneManager()->getActiveCamera()->getRotation().Y-90.0f;
		isMoving=demo->IsMovementKeyDown();

		// Ack, makes the screen messed up and the mouse move off the window
		// Find another way to keep the dead player from moving
	//	demo->EnableInput(IsDead()==false);

		return;
	}

	// Update interpolation
	SLNet::TimeMS elapsed = curTime-lastUpdate;
	if (elapsed<=1)
		return;
	if (elapsed>100)
		elapsed=100;

	lastUpdate=curTime;
	irr::core::vector3df curPositionDelta = position-model->getPosition();
	irr::core::vector3df interpThisTick = positionDeltaPerMS*(float) elapsed;
	if (curTime < interpEndTime && interpThisTick.getLengthSQ() < curPositionDelta.getLengthSQ())
	{
		model->setPosition(model->getPosition()+positionDeltaPerMS*(float) elapsed);
	}
	else
	{
		model->setPosition(position);
	}

	float curRotationDelta = GetRotationDifference(rotationAroundYAxis,model->getRotation().Y);
	float interpThisTickRotation = rotationDeltaPerMS*(float)elapsed;
	if (curTime < interpEndTime && fabs(interpThisTickRotation) < fabs(curRotationDelta))
	{
		model->setRotation(model->getRotation()+core::vector3df(0,interpThisTickRotation,0));
	}
	else
	{
		model->setRotation(core::vector3df(0,rotationAroundYAxis,0));
	}

	if (isDead)
	{
		UpdateAnimation(scene::EMAT_DEATH_FALLBACK);
		model->setLoopMode(false);		
	}
	else if (curAnim!=scene::EMAT_ATTACK)
	{
		if (isMoving)
		{
			UpdateAnimation(scene::EMAT_RUN);
			model->setLoopMode(true);
		}
		else
		{
			UpdateAnimation(scene::EMAT_STAND);
			model->setLoopMode(true);
		}
	}	
}
void PlayerReplica::UpdateAnimation(irr::scene::EMD2_ANIMATION_TYPE anim)
{
	if (anim!=curAnim)
		model->setMD2Animation(anim);
	curAnim=anim;
}
float PlayerReplica::GetRotationDifference(float r1, float r2)
{
	float diff = r1-r2;
	while (diff>180.0f)
		diff-=360.0f;
	while (diff<-180.0f)
		diff+=360.0f;
	return diff;
}
void PlayerReplica::OnAnimationEnd(scene::IAnimatedMeshSceneNode* node)
{
	// unused parameters
	(void)node;

	if (curAnim==scene::EMAT_ATTACK)
	{
		if (isMoving)
		{
			UpdateAnimation(scene::EMAT_RUN);
			model->setLoopMode(true);
		}
		else
		{
			UpdateAnimation(scene::EMAT_STAND);
			model->setLoopMode(true);
		}
	}
}
void PlayerReplica::PlayAttackAnimation(void)
{
	if (isDead==false)
	{
		UpdateAnimation(scene::EMAT_ATTACK);
		model->setLoopMode(false);		
	}
}
bool PlayerReplica::IsDead(void) const
{
	return deathTimeout > SLNet::GetTimeMS();
}
BallReplica::BallReplica()
{
	creationTime= SLNet::GetTimeMS();
}
BallReplica::~BallReplica()
{
}
void BallReplica::WriteAllocationID(SLNet::Connection_RM3 *destinationConnection, SLNet::BitStream *allocationIdBitstream) const
{
	// unused parameters
	(void)destinationConnection;

	allocationIdBitstream->Write(SLNet::RakString("BallReplica"));
}
void BallReplica::SerializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
{
	BaseIrrlichtReplica::SerializeConstruction(constructionBitstream, destinationConnection);
	constructionBitstream->Write(shotDirection);
}
bool BallReplica::DeserializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *sourceConnection)
{
	if (!BaseIrrlichtReplica::DeserializeConstruction(constructionBitstream, sourceConnection))
		return false;
	constructionBitstream->Read(shotDirection);
	return true;
}
void BallReplica::PostDeserializeConstruction(SLNet::BitStream *constructionBitstream, SLNet::Connection_RM3 *destinationConnection)
{
	// unused parameters
	(void)constructionBitstream;
	(void)destinationConnection;

	// Shot visible effect and BallReplica classes are not linked, but they update the same way, such that
	// they are in the same spot all the time
	demo->shootFromOrigin(position, shotDirection);

	// Find the owner of this ball, and make them play the attack animation
	unsigned int idx;
	for (idx=0; idx < PlayerReplica::playerList.Size(); idx++)
	{
		if (PlayerReplica::playerList[idx]->creatingSystemGUID==creatingSystemGUID)
		{
			PlayerReplica::playerList[idx]->PlayAttackAnimation();
			break;
		}
	}
}
void BallReplica::PreDestruction(SLNet::Connection_RM3 *sourceConnection)
{
	// unused parameters
	(void)sourceConnection;

	// The system that shot this ball destroyed it, or disconnected
	// Technically we should clear out the node visible effect too, but it's not important for now
}
RM3SerializationResult BallReplica::Serialize(SLNet::SerializeParameters *serializeParameters)
{
	BaseIrrlichtReplica::Serialize(serializeParameters);
	return RM3SR_BROADCAST_IDENTICALLY;
}
void BallReplica::Deserialize(SLNet::DeserializeParameters *deserializeParameters)
{
	BaseIrrlichtReplica::Deserialize(deserializeParameters);
}
void BallReplica::Update(SLNet::TimeMS curTime)
{
	// Is a locally created object?
	if (creatingSystemGUID==rakPeer->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS))
	{
		// Destroy if shot expired
		if (curTime > shotLifetime)
		{
			// Destroy on network
			BroadcastDestruction();
			delete this;
			return;
		}
	}

	// Keep at the same position as the visible effect
	// Deterministic, so no need to actually transmit position
	// The variable position is the origin that the ball was created at. For the player, it is their actual position
	SLNet::TimeMS elapsedTime;
	// Due to ping variances and timestamp miscalculations, it's possible with very low pings to get a slightly negative time, so we have to check
	if (curTime>=creationTime)
		elapsedTime = curTime - creationTime;
	else
		elapsedTime=0;
	irr::core::vector3df updatedPosition = position + shotDirection * (float) elapsedTime * SHOT_SPEED;

	// See if the bullet hit us
	if (creatingSystemGUID!=rakPeer->GetGuidFromSystemAddress(SLNet::UNASSIGNED_SYSTEM_ADDRESS))
	{
		if (playerReplica->IsDead()==false)
		{
			//float playerHalfHeight=demo->GetSyndeyBoundingBox().getExtent().Y/2;
			irr::core::vector3df positionRelativeToCharacter = updatedPosition-playerReplica->position;//+core::vector3df(0,playerHalfHeight,0);
			if (demo->GetSyndeyBoundingBox().isPointInside(positionRelativeToCharacter))
			//if ((playerReplica->position+core::vector3df(0,playerHalfHeight,0)-updatedPosition).getLengthSQ() < BALL_DIAMETER*BALL_DIAMETER/4.0f)
			{
				// We're dead for 3 seconds
				playerReplica->deathTimeout=curTime+3000;
			}
		}
	}
}
SLNet::Replica3 *Connection_RM3Irrlicht::AllocReplica(SLNet::BitStream *allocationId, ReplicaManager3 *replicaManager3)
{
	// unused parameters
	(void)replicaManager3;

	SLNet::RakString typeName; allocationId->Read(typeName);
	if (typeName=="PlayerReplica") {BaseIrrlichtReplica *r = new PlayerReplica; r->demo=demo; return r;}
	if (typeName=="BallReplica") {BaseIrrlichtReplica *r = new BallReplica; r->demo=demo; return r;}
	return 0;
}
