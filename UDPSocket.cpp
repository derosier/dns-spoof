//****************************************************************************
// Module name : UDPSocket
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
// First written on 1/23/2010 by Steve deRosier.
//
// Module Description:
//    UDP Socket implmentation.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "UDPSocket.h"
#include <arpa/inet.h>
#include "cerror.h"

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
// Function name: CUDPSocket::~CUDPSocket()
// Created by: author's name
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Destructor.  Closes the socket before finishing.
// Notes: If Close() throws, we're going to ignore the error.  Odds are you're 
//        only closing this from the destructor if the program is closing down
//        or in other situations where we don't want to see an error anyway.
//****************************************************************************
CUDPSocket::~CUDPSocket()
{
   if( mSockOpen )
   {
      try
      {
         Close();
      }
      catch(...)
      {
         // catching and ignoring errors, we're killing the socket one way or another anyway.
      }
   }
   mSockOpen = false;
}
   
//****************************************************************************
// Function name: void CUDPSocket::Close( void )
//    returns: nothing
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Closes the socket.  
// Notes: 
//****************************************************************************
void CUDPSocket::Close( void )
{
   if( close( mFDSocket ) < 0 )
   {
      // Error
      throw CE_UDPSocket( CE_UDPSocket::SOCK_CLOSE );
   }
   
   mSockOpen = false;
}

//****************************************************************************
// Function name: bool CUDPSocket::Open( int Port )
//    returns: true if the socket was opened.
//    Port: Port number to bind the socket to.
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Opens a UDP datagram socket on the specifed port.
// Notes: restrictions, odd modes
//****************************************************************************
bool CUDPSocket::Open( int Port )
{
   if( mSockOpen )
   {
      Close();
   }
   
   mPort = Port;
   mDomain = AF_INET;
   mType = SOCK_DGRAM;
   
   if( 0 > (mFDSocket = socket( mDomain, mType, 0)) )
   {
      // error
      throw CE_UDPSocket( CE_UDPSocket::SOCK_CREATE );
   }

   mAddr.sin_family = mType;
   mAddr.sin_port = htons( mPort );
   mAddr.sin_addr.s_addr = htonl( INADDR_ANY );

   if( 0 > (bind( mFDSocket, (struct sockaddr *) &mAddr, sizeof(mAddr) )) )
   {
      // error
      throw CE_UDPSocket( CE_UDPSocket::SOCK_BIND );
   }
   
   mSockOpen = true;
   
   return true;
}

//****************************************************************************
// Function name: void CUDPSocket::Send( const CUDPMessage & Msg )
//    returns: nothing
//    Msg: The message to send
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Sends Msg on the already open UDP socket.
// Notes: 1. Send() will block until the mesage is sent.
//****************************************************************************
void CUDPSocket::Send( const CUDPMessage & Msg )
{
   ssize_t RecVal = -1;
   if( Msg.mSize <= 0 )
   {
      throw CE_UDPSocket( CE_UDPSocket::MSG_SIZE_ZERO );
   }
   
   if( Msg.mSenderAddrLen <= 0 )
   {
      throw CE_UDPSocket( CE_UDPSocket::MSG_ROUTE );
   }
   
   if( !mSockOpen )
   {
      throw CE_UDPSocket( CE_UDPSocket::SOCK_NOT_OPEN );
   }
   
   if( mSockOpen && ((AF_INET == mDomain) && (SOCK_DGRAM == mType)) )
   {
      RecVal = sendto( mFDSocket, &(Msg.mMsgData), Msg.mSize, 0, 
                         (struct sockaddr *)&(Msg.mSenderAddr), Msg.mSenderAddrLen );
      
      if( RecVal < 0 )
      {
         // error
         throw CE_UDPSocket( CE_UDPSocket::SOCK_TX );
      }
      
   }
   else
   {
      // error
      throw CE_UDPSocket( CE_UDPSocket::SOCK_TYPE );
   }
   
   return;
}

//****************************************************************************
// Function name: int CUDPSocket::Recieve( CUDPMessage & Msg )
//    returns: the recieved size of the message
//    Msg: Destination message class structure.
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Recieves a UDP message and places it in the destination Msg.
// Notes: Recieve() will block until a message is received.
//****************************************************************************
int CUDPSocket::Recieve( CUDPMessage & Msg )
{
   ssize_t RecVal = -1;
  
   if( mSockOpen && ((AF_INET == mDomain) && (SOCK_DGRAM == mType)) )
   {
      Msg.mSize = CUDPMessage::MAX_MSG_SIZE;
      Msg.mSenderAddrLen = sizeof(Msg.mSenderAddr);
      RecVal = recvfrom( mFDSocket, &(Msg.mMsgData), Msg.mSize, 0, 
                         (struct sockaddr *)&(Msg.mSenderAddr), &(Msg.mSenderAddrLen) );
      
      if( RecVal >= 0 )
      {
         // recieved OK
         Msg.mSize = RecVal;
      }
      else
      {
         // error
         Msg.mSize = 0;  // invalid data, so make sure that no one looks at it.
         throw CE_UDPSocket( CE_UDPSocket::SOCK_RX );
      }
      
   }
   else
   {
      if( !mSockOpen )
      {
         throw CE_UDPSocket( CE_UDPSocket::SOCK_NOT_OPEN );
      }
      else
      {
         throw CE_UDPSocket( CE_UDPSocket::SOCK_TYPE );
      }
   }
   
   return RecVal;
}


//****************************************************************************
// Function name: void CUDPMessage::DumpHex()
//    returns: nothing
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description:  Debugging dump of the message in hex mode.
//****************************************************************************
void CUDPMessage::DumpHex()
{
   char oldfill = cout.fill();

   cout << endl << endl << "CUDPMessage dump:" << endl;
   cout << "size: " << mSize << endl;

   // Sender stuff doesn't translate to meaningful data for some reason.  I don't care for my
   // debug dump, maybe later it should be fixed.
   //   cout << "sender size: " << mSenderAddrLen << endl;
   //    char SenderAddr[INET_ADDRSTRLEN];
   //    inet_ntop(AF_INET, &mSenderAddr, SenderAddr, INET_ADDRSTRLEN); 
   //    cout << "sender: "<< SenderAddr << endl;

   cout << "data:" << endl;
   cout << "0x ";
   cout << std::hex;
   for( unsigned int i = 0; i < mSize; i++ )
   {
      cout.fill('0');
      cout.width(2);
      // The mask below defeats the sign-extension on the int cast
      cout << (0x000000FF & (unsigned int)(mMsgData[i]));
      cout << " ";
   }

   cout << endl;
   cout << dec;
   cout.fill(oldfill);  
   cout.flush();
}


