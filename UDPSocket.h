//****************************************************************************
// Module name : UDPSocket
// Project     : dns-spoof
// Interface (header) File
//
// Copyright (C) 2010 by Steve deRosier.
// All rights reserved.
//
// The information contained herein is confidential 
// property of Company. The use, copying, transfer or 
// disclosure of such information is prohibited except
// by express written agreement with Company.
//
// First written on 1/23/2010 by Steve deRosier.
//
// Module Description:
//   UDP Specific socket implmentation.
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

// Don't change the following 3 lines.
// This checks and throws an error in case you
// didn't customize the above define.              
#ifdef MODULENAME_H_
#error Module name define not set in header
#endif

// Everything that you put in this file goes below
// this line.  It is importaint to keep everything
// within the #ifndef MODULENAME_H_ .. #endif pair 

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "socket.h"

//****************************************************************************
//  Defines section
// Add all constants here
//
//****************************************************************************

//****************************************************************************
//  Function Prototype Section
// Add prototypes for all functions called by this
// module, with the exception of runtime routines 
// or prototypes already included via #include.
// 
//****************************************************************************

//****************************************************************************
//****************************************************************************
//****************************************************************************
//  Class Interface
//****************************************************************************

//****************************************************************************
// Class CUDPMessage
// Defines a UDP message structure for use with UDP Send/Receive functions.
//****************************************************************************
class CUDPMessage : public CSockMsg
{
public:
   enum {MAX_MSG_SIZE = 512}; // UDP Messages are limited to 512 bytes
   uint8_t mMsgData[MAX_MSG_SIZE];
   size_t mSize;
   sockaddr_in mSenderAddr;
   socklen_t mSenderAddrLen;
   
   virtual void DumpHex();
   CUDPMessage() : mSize(0), mSenderAddrLen(0) {};
   virtual ~CUDPMessage() {};
};

//****************************************************************************
// Class CSocket
// Defines a socket class interface that can be subclassed for more specific 
// socket types.
//****************************************************************************
class CUDPSocket : public CSocket
{
public:
   CUDPSocket() {};
   virtual ~CUDPSocket();
   
   virtual void Close(void);
   virtual bool Open( int Port );
   
   virtual void Send( const CUDPMessage & Msg );
   virtual int Receive( CUDPMessage & Msg );

};

// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


