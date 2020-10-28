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

//--------------------------Renames--------------------------------
//These are renames, to make the interface a different name than the C++ interface
//There are many reasons to do this sometimes the original function is useful but not perfect and a pure
//C# helper is created. The original is renamed and hidden.
//operators need renaming
//Some items may conflict with C#, they would need renaming

%rename(GET_BASE_OBJECT_FROM_IDORIG) SLNet::NetworkIDManager::GET_BASE_OBJECT_FROM_ID;
%rename(SetNetworkIDManagerOrig) SLNet::NetworkIDObject::SetNetworkIDManager;
%rename(GetBandwidthOrig) SLNet::RakPeer::GetBandwidth;
%rename(GetBandwidthOrig) SLNet::RakPeerInterface::GetBandwidth;
%rename(Equals) operator ==;
%rename(CopyData) operator =;
%rename(OpLess) operator <;
%rename(OpGreater) operator >;
%rename(OpArray) operator [];
%rename(OpLessEquals) operator <=;
%rename(OpGreaterEquals) operator >=;
%rename(OpNotEqual) operator !=;
%rename(OpPlus) operator +;
%rename(OpPlusPlus) operator ++;
%rename(OpMinusMinus) operator --;
%rename(OpMinus) operator -;
%rename(OpDivide) operator /;
%rename(OpMultiply) operator *;
