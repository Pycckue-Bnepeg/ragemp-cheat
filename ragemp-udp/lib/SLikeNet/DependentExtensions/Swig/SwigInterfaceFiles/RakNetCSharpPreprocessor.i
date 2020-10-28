//
// This file was taken from RakNet 4.082.
// Please see licenses/RakNet license.txt for the underlying license and related copyright.
//
//
//
// Modified work: Copyright (c) 2018, SLikeSoft UG (haftungsbeschr�nkt)
//
// This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
// license found in the license.txt file in the root directory of this source tree.

//----------------Extra Swig PreProcessor------------
//To keep everything clean general swig preprocessor commands are stuck here
//Alot of items not in here may be preprocessor, but are in thier own section for some reason or another
//Defines
#ifdef SWIGWIN
#define _MSC_VER 10000
#define WIN32
#define _WIN32
#endif

//typedefs
typedef int int32_t;
typedef unsigned int uint32_t;
typedef uint32_t DefaultIndexType;
#ifdef SWIGWIN
typedef unsigned int SOCKET;
typedef unsigned int __UDPSOCKET__ ;
typedef unsigned int __TCPSOCKET__;
#endif
typedef SLNet::RakString::SharedString SharedString;
//Global Inserts
#define SWIG_CSHARP_NO_IMCLASS_STATIC_CONSTRUCTOR 1;

//----------------------------Method modifiers---------------------
%include "RakNetSwigMacros.i"
