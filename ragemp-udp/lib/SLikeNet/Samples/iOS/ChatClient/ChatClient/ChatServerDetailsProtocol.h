/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant 
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 */

//
// Protocol to receive the server details from the modal view
//

@protocol ChatServerDetailsProtocol
- (void) connectToChatServer:(NSString *)serverIP serverPort:(NSString *)serverPort;
@end
