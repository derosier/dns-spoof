//****************************************************************************
// Module name : test_socket
// Project     : dns-spoof
//
// Copyright (C) 2010 by Steve deRosier.
// All rights reserved.
//
// The information contained herein is confidential 
// property of Company. The use, copying, transfer or 
// disclosure of such information is prohibited except
// by express written agreement with Company.
//
// First written on 1/25/2010 by Steve deRosier.
//
// Module Description:
//   Test driver for socket class unit tests.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "../UDPSocket.h"
#include "../cerror.h"

//****************************************************************************
//  Defines section
// Add all #defines or consts here
//
//****************************************************************************
using namespace std;
const int PortServer = 9998;
const int PortClient = 9997;

//****************************************************************************
//  Function Prototype Section
// Add prototypes for all functions called by this
// module, with the exception of runtime routines,
// or prototypes already included via header files.
//****************************************************************************

//****************************************************************************
// main
//
// This test driver spawns a client process that opens a port and sends
// data to the server process.  The server process then sends a string
// back.  
//
//****************************************************************************
int main( int argc, char * argv[] )
{
   cout << "UDPSocket Unit Tests" << endl;
   cout << "****************************************************************************" << endl;
   
   struct sockaddr_in sa_server;
   struct sockaddr_in sa_client;
   
   sa_server.sin_family = AF_INET;
   sa_server.sin_port = htons(PortServer);
   sa_server.sin_addr.s_addr = inet_addr( "127.0.0.1" );
   
   sa_client.sin_family = AF_INET;
   sa_client.sin_port = htons(PortClient);
   sa_client.sin_addr.s_addr = inet_addr( "127.0.0.1" );
   
   
   if( fork() == 0 )
   {
      // Child - client
      sleep( 2 );
      cout << "Started client" << endl;

      CUDPSocket * mSocket;
      
      // Create our UDP socket
      mSocket = new CUDPSocket;

      // Open our UDP port
      try
      {
         if( !mSocket->Open( PortClient ) )
         {
            // Error
            cerr << "Error client: failed to open UDP socket on port " << PortClient << endl;
            return EXIT_FAILURE;      
         }
      }
      catch( CE_UDPSocket & e )
      {
         cerr << "Error client: failed to open UDP socket on port " << PortClient;
         cerr << ": " << e.ErrString() << "." << endl;
         return EXIT_FAILURE;
      }

      // create a message
      CUDPMessage MsgSend;
      MsgSend.mSenderAddr = sa_server;  // mSenderAddr in this case is where we want to send it
      MsgSend.mSenderAddrLen = sizeof(sockaddr_in);
      memcpy( &(MsgSend.mMsgData), "Sending 123\0", 12);
      MsgSend.mSize = 12;
      
      // Send the message
      try
      {
         cout << "Client sending message" << endl;
         mSocket->Send( MsgSend );
      }
      catch( CE_UDPSocket & e )
      {
         cerr << "Error on send: ";
         cerr << e.ErrString() << "." << endl;
         return EXIT_FAILURE;
      }
      
      // Wait for a Response
      cout << "Client waiting for response" << endl;
      
      // print our response
      int RecSize = 0;
      CUDPMessage Msg;
      
      // Attempt to get a message.  This will block untill an error or msg Received.
      try
      {
         RecSize = mSocket->Receive( Msg );
      }
      catch( CE_UDPSocket & e )
      {
         cerr << "Error on receive: ";
         cerr << e.ErrString() << "." << endl;
         return EXIT_FAILURE;
      }
         
      // Got a message, process it
      if( RecSize > 0 )
      {
         char buff[512];
         memcpy( buff, &(Msg.mMsgData), Msg.mSize ); 
         cout << "Client got message: \"" << buff << "\"" << endl;
         cout << "Test 4: passed"  << endl;
      }

      // We're done, close our socket
      try
      {
         mSocket->Close();
      }
      catch( CE_UDPSocket & e )
      {
         cerr << "Error on close: ";
         cerr << e.ErrString() << "." << endl;
         return EXIT_FAILURE;
      }

   }
   else
   {
      CUDPSocket * mSocket;
      
      // Parent - server
      cout << "Started server" << endl;
      
      // Create our UDP socket
      cout << "Test 1: constructing new UDP socket" << endl;
      mSocket = new CUDPSocket;
      cout << "Test 1: passed" << endl;
      
      cout << "Test 2: Opening UDP Port on: " << PortServer << endl;
      try
      {
         if( !mSocket->Open( PortServer ) )
         {
            // Error
            cerr << "Error: failed to open UDP socket on port " << PortServer << endl;
            return EXIT_FAILURE;      
         }
      }
      catch( CE_UDPSocket & e )
      {
         cerr << "Error: failed to open UDP socket on port " << PortServer;
         cerr << ": " << e.ErrString() << "." << endl;
         return EXIT_FAILURE;
      }

      cout << "Test 2: passed" << endl;
      
      cout << "Test 3: Receive message from client" << endl;
      int RecSize = 0;
      CUDPMessage Msg;
      
      // Attempt to get a message.  This will block untill an error or msg Received.
      try
      {
         RecSize = mSocket->Receive( Msg );
      }
      catch( CE_UDPSocket & e )
      {
         cerr << "Error on receive: ";
         cerr << e.ErrString() << "." << endl;
         return EXIT_FAILURE;
      }
         
      // Got a message, process it
      if( RecSize > 0 )
      {
         char buff[512];
         memcpy( buff, &(Msg.mMsgData), Msg.mSize ); 
         cout << "Server Received from client: \"" << buff << "\"" << endl;
         cout << "Test 3: passed"  << endl;
         
         cout << "Test 4: sending message to client" << endl;
         CUDPMessage Response = Msg;

         memcpy( &(Response.mMsgData), "Server got data.\0", 17);
         Response.mSize = 17;
         
         // Send the message
         try
         {
            cout << "sending message response" << endl;
            mSocket->Send( Response );
         }
         catch( CE_UDPSocket & e )
         {
            cerr << "Error on send: ";
            cerr << e.ErrString() << "." << endl;
            return EXIT_FAILURE;
         }
         
      }
      
      cout << "Test 5: closing socket " << endl;
      try
      {
         mSocket->Close();
      }
      catch( CE_UDPSocket & e )
      {
         cerr << "Error on close: ";
         cerr << e.ErrString() << "." << endl;
         return EXIT_FAILURE;
      }
      cout << "Test 5: passed " << endl;
         
         
   }
   
   
   return EXIT_SUCCESS;
}

