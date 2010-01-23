//****************************************************************************
// Module name : Server 
// Project     : dns-spoof
//
// Copyright (C) 2010 Cal-Sierra Communications.
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
//    Main server module for dns-spoof
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "server.h"

#ifdef DEBUG_BUILD
#include <iostream>
using namespace std;
#endif
//****************************************************************************
//  Defines section
// Add all #defines or consts here
//
//****************************************************************************

//****************************************************************************
//  Function Prototype Section
// Add prototypes for all functions called by this
// module, with the exception of runtime routines,
// or prototypes already included via header files.
//****************************************************************************

//****************************************************************************
// Function name: TYPE foo(TYPE arg1, TYPE arg2)
//    returns: return value description
//    arg1: description
//    arg2: description
// Created by: author's name
// Date created: date
// Description: detailed description 
// Notes: restrictions, odd modes
//****************************************************************************
CServer::CServer( int Port ) : mPort(Port), mSocket(0)
{
}

CServer::~CServer()
{
   if( mSocket )
   {
      mSocket->Close();
      delete mSocket;
   }
}



void CServer::ServerTask( void )
{

   #ifdef DEBUG_BUILD
      cout << "Starting server on port " << mPort << endl;
   #endif   

   // Create our UDP socket   
   mSocket = new CSocket;
   
   if( !mSocket->OpenUDP( mPort ) )
   {
      // Error
      #ifdef DEBUG_BUILD
         cerr << "Error: failed to open UDP socket on port " << mPort << endl;
      #endif   
      return;      
   }
   
   // Get messages and process them
   while( 1 )
   {
      int RecSize = 0;
      CUDPMessage Msg;
      
      // Attempt to get a message.  This will block untill an error or msg recieved.
      try
      {
         RecSize = mSocket->RecieveUDP( Msg );
      }
      catch(...)
      {
         // Error
         #ifdef DEBUG_BUILD
            cerr << "Error: socket recieve " << endl;
         #endif   
      }
      
      // Got a message, process it
      if( RecSize > 0 )
      {
         ProcessMsg( Msg );
      }
   }
}


void CServer::ProcessMsg( CUDPMessage Msg )
{
   Msg.DumpHex();
}
