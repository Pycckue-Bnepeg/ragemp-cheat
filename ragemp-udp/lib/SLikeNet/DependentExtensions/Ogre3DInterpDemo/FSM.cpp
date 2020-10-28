/*
 *  Copyright (c) 2014, Oculus VR, Inc.
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

#include "FSM.h"
#include "State.h"
#include "slikenet/assert.h"



FSM::FSM()
{

}
FSM::~FSM()
{
	Clear();
}
void FSM::Clear(void)
{
	unsigned i;
	if (stateHistory.Size())
		stateHistory[stateHistory.Size()-1]->OnLeave(this, true);
	for (i=0; i < stateHistory.Size(); i++)
		stateHistory[i]->FSMRemoveRef(this);
	stateHistory.Clear(false, _FILE_AND_LINE_);
}
State *FSM::CurrentState(void) const
{
	if (stateHistory.Size()==0)
		return 0;
	return stateHistory[stateHistory.Size()-1];
}
State *FSM::GetState(int index) const
{
	RakAssert(index>=0 && index < (int) stateHistory.Size());
	return stateHistory[(unsigned) index];
}
int FSM::GetStateIndex(State *state) const
{
	return (int) stateHistory.GetIndexOf(state);
}
int FSM::GetStateHistorySize(void) const
{
	return stateHistory.Size();
}
// #med - change index parameter to unsigned int
void FSM::RemoveState(const int index)
{
	RakAssert(index>=0 && index < (int) stateHistory.Size());
	if (static_cast<unsigned int>(index)==stateHistory.Size()-1)
		stateHistory[index]->OnLeave(this, true);
	stateHistory[index]->FSMRemoveRef(this);
	stateHistory.RemoveAtIndex((const unsigned int)index);
	if (static_cast<unsigned int>(index)==stateHistory.Size())
		stateHistory[stateHistory.Size()-1]->OnEnter(this, false);
}
void FSM::AddState(State *state)
{
	if (stateHistory.Size())
		stateHistory[stateHistory.Size()-1]->OnLeave(this, false);
	state->FSMAddRef(this);
	state->OnEnter(this, true);
	stateHistory.Insert(state, _FILE_AND_LINE_ );
}
// #med - change index parameter to unsigned int
void FSM::ReplaceState(const int index, State *state)
{
	RakAssert(index>=0 && index < (int) stateHistory.Size());
	if (state!=stateHistory[index])
	{
		if (static_cast<unsigned int>(index)==stateHistory.Size()-1)
			stateHistory[index]->OnLeave(this, true);
		stateHistory[index]->FSMRemoveRef(this);
		state->FSMAddRef(this);
		if (static_cast<unsigned int>(index)==stateHistory.Size()-1)
			state->OnEnter(this, true);
		stateHistory[index]=state;
	}
}
// #med - change index parameter to unsigned int
void FSM::SetPriorState(const int index)
{
	RakAssert(index>=0 && index < (int) stateHistory.Size());
	stateHistory[stateHistory.Size()-1]->OnLeave(this, true);
	for (unsigned i=stateHistory.Size()-1; i > (unsigned) index; --i)
	{
		stateHistory[i]->FSMRemoveRef(this);
		stateHistory.RemoveFromEnd();
	}
	stateHistory[index]->OnEnter(this, false);
}
