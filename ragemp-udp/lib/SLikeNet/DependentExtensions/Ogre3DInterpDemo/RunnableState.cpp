/*
 * This file was taken from RakNet 4.082.
 * Please see licenses/RakNet license.txt for the underlying license and related copyright.
 *
 * Modified work: Copyright (c) 2017-2018, SLikeSoft UG (haftungsbeschränkt)
 *
 * This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 * license found in the license.txt file in the root directory of this source tree.
 */

#include "RunnableState.h"


// #med - better rename member variable (parent) --- then revert local parameter to parent
void RunnableState::SetParentApp(AppInterface *newparent)
{
	parent=newparent;
}
void RunnableState::SetFocus(bool hasFocus)
{
	// unused parameters
	(void)hasFocus;
}
void RunnableState::OnStateReady(void)
{

}
