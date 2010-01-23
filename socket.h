//****************************************************************************
// Module name : Socket
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
// First written on  by Steve deRosier.
//
// Module Description:
// (fill in a detailed description of the module's 
// function here).
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef SOCKET_H_
#define SOCKET_H_

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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
//  Class Interface
// Define the module's class (or classes) here.
//
//****************************************************************************
class CUDPMessage 
{
public:
   enum {MAX_MSG_SIZE = 512}; // UDP Messages are limited to 512 bytes
   char mMsgData[MAX_MSG_SIZE];
   size_t mSize;
   sockaddr mSenderAddr;
   socklen_t mSenderAddrLen;
   
   void DumpHex();
};


class CSocket
{
public:
   CSocket();
   virtual ~CSocket();
   
   virtual void Close(void);
   virtual bool OpenUDP( int Port );
   
   virtual int RecieveUDP( CUDPMessage & Msg );

private:

   struct sockaddr_in mAddr;

   int mDomain;
   int mType;
   int mPort;

   int mFDSocket;
   
   bool mSockOpen;
};


// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


