//****************************************************************************
// Module name : Server
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
// First written on 1/22/2010 by Steve deRosier.
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
#ifndef SERVER_H_
#define SERVER_H_

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
#include "UDPSocket.h"

//****************************************************************************
//  Defines section
// Add all constants here
//
//****************************************************************************

// SPOOF_IP is the hard-coded IP address to send in repsonse to all DNS requests.
// Replace the number with any other number to change.  I recomend hex as it
// is easier to visualize each individual octect of the IP address. 
const uint32_t SPOOF_IP = 0x06060606; // 6.6.6.6

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
class CServer 
{
public:
   CServer( int Port = 53 );
   virtual ~CServer();

   virtual void RunServer( void ) { ServerTask(); }
   
public:   
   void ServerTask( void );
   bool ProcessMsg( CUDPMessage & Msg, CUDPMessage & ResponseMsg );
   
   int mPort;
   CUDPSocket * mSocket;
};


// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


