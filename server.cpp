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
#include "dns_msg.h"

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
   mSocket = new CUDPSocket;
   
   if( !mSocket->Open( mPort ) )
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
         RecSize = mSocket->Recieve( Msg );
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
         if( ProcessMsg( Msg ) )
         {
            // Send the message
            try
            {
               mSocket->Send( Msg );
            }
            catch(...)
            {
               // Error
               #ifdef DEBUG_BUILD
                  cerr << "Error: socket send " << endl;
               #endif   
            }
         }
      }
   }
}


bool CServer::ProcessMsg( CUDPMessage & Msg )
{
   bool RetVal = false;

   #ifdef DEBUG_BUILD
      cout << "--- new DNS recieved ---" << endl;
      Msg.DumpHex();
      
      CDNS_Msg DNSMsg( Msg.mMsgData );
      cout << DNSMsg;
      cout.flush();
   #endif
   
   // Validate our message, must be the right type of request
   CDNS_Header Header = DNSMsg.Header();
   if( Header.QDCount() > 0 )
   {
      if( ((DNSMsg.Query()).QType() == DNS_TYPE_A) &&
          ((DNSMsg.Query()).QClass() == DNS_CLASS_IN) )
      {
         // Build our answer from the recieved question
         Header.Type( RESPONSE );
         Header.RecurseAvail( true );
         
         CDNS_Answer Answer;
      
         Answer.QType(DNS_TYPE_A);
         Answer.QClass(DNS_CLASS_IN);
         Answer.TTL(7200);  // 2 hours
         Answer.RDLength(4);
         Answer.RData(0x06060606);
         uint8_t Name[DNS_NAME_MAX];
         (DNSMsg.Query()).GetQName( Name );
         Answer.SetName( Name );   

         // Set the new header and answer; query can stay the same
         DNSMsg.Answer( Answer );
         Header.ANCount(1);
         DNSMsg.Header( Header );
         
         // Convert into a format we can send 
         Msg.mSize = DNSMsg.Pack( Msg.mMsgData );

         #ifdef DEBUG_BUILD
            cout << "--- Answer to send ---" << endl;
            cout << DNSMsg;
            cout.flush();
            Msg.DumpHex();
         #endif
         
         RetVal = true;
      }
   }
   
   return RetVal;
}
