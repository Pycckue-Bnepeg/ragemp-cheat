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

#ifndef __RUNNABLE_STATE_H
#define __RUNNABLE_STATE_H

#include "AppTypes.h"
#include "State.h"

class AppInterface;

class RunnableState : public State
{
public:
	virtual void Update(AppTime curTimeMS, AppTime elapsedTimeMS)=0;
	virtual void UpdateEnd(AppTime curTimeMS, AppTime elapsedTimeMS)
	{
		// unused parameters
		(void)curTimeMS;
		(void)elapsedTimeMS;
	}
	virtual void Render(AppTime curTimeMS)=0;
	void SetParentApp(AppInterface *newparent);
	// Call when internal variables set and the state is ready to use
	virtual void OnStateReady(void);

	// Don't do this so I can override by return type in derived classes
	//virtual AppInterface *GetParentApp(void) const;

	virtual void SetFocus(bool hasFocus);

protected:
	AppInterface *parent;

};


#endif
