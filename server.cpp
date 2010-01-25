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
// Function name: CServer::CServer( int Port )
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Constructor.  Initalizes member variables.  Note that the
//   no socket is opened until the server runs, ie not here.
//****************************************************************************
CServer::CServer( int Port ) : mPort(Port), mSocket(0)
{
}

//****************************************************************************
// Function name: CServer::~CServer()
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Destructor.  It closes and deletes the socket.
// Notes: Ignores errors from CSocket::Close().  We're closing it to cleanup 
//        and quit anyway.
//****************************************************************************
CServer::~CServer()
{
   if( mSocket )
   {
      try
      {
         mSocket->Close();
      }
      catch(...)
      {
         // catching and ignoring errors, we're killing the socket one way or another anyway.
      }
      
      delete mSocket;
   }
}

//****************************************************************************
// Function name: void CServer::ServerTask( void )
//    returns: nothing
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Main server task.  Basic flow/logic follows:
//   1. Create a new UDP socket
//   2. Open the socket
//   3. Loop forever:
//      3a. Recieve a message (blocks)
//      3b. If message recieved, process it
//      3c. If the message recieved generates a response, send the response
//
// Notes: Do not call directly, call RunServer() instead.
//****************************************************************************
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
         CUDPMessage Response;
         if( ProcessMsg( Msg, Response ) )
         {
            // Send the message
            try
            {
               cout << "sending message response" << endl;
               mSocket->Send( Response );
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
      
      
   } // END while(1)
}


//****************************************************************************
// Function name: bool CServer::ProcessMsg( CUDPMessage & Msg, CUDPMessage & ResponseMsg )
//    returns: true if there is a response to send
//    Msg: Msg recieved 
//    ResponseMsg: Response message to send
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Determines if the message recieved is a query, and if so 
//   constructs our contrived response, using SPOOF_IP as the IP to respond with.
//****************************************************************************
bool CServer::ProcessMsg( CUDPMessage & Msg, CUDPMessage & ResponseMsg )
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
         Answer.RData(SPOOF_IP);
         uint8_t Name[DNS_NAME_MAX];
         (DNSMsg.Query()).GetQName( Name );
         Answer.SetName( Name );   

         // Set the new header and answer; query can stay the same
         DNSMsg.Answer( Answer );
         Header.ANCount(1);
         DNSMsg.Header( Header );
         
         // Convert into a format we can send 
         ResponseMsg.mSize = DNSMsg.Pack( ResponseMsg.mMsgData );
         
         // Setup the rest of our response Msg
         ResponseMsg.mSenderAddr = Msg.mSenderAddr;
         ResponseMsg.mSenderAddrLen = Msg.mSenderAddrLen;
         

         #ifdef DEBUG_BUILD
            cout << "--- Answer to send ---" << endl;
            cout << DNSMsg;
            cout.flush();
            ResponseMsg.DumpHex();
         #endif
         
         RetVal = true; // Msg processed and have a response to send.
      }
   }
   
   return RetVal;
}
