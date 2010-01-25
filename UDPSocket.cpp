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
CUDPSocket::~CUDPSocket()
{
   if( mSockOpen )
      Close();
   mSockOpen = false;
}
   
void CUDPSocket::Close( void )
{
   if( close( mFDSocket ) < 0 )
   {
      // Error
   }
   
   mSockOpen = false;
}

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
      cerr << "Error creating socket" << endl;
      return false;
   }

   mAddr.sin_family = mType;
   mAddr.sin_port = htons( mPort );
   mAddr.sin_addr.s_addr = htonl( INADDR_ANY );

   if( 0 > (bind( mFDSocket, (struct sockaddr *) &mAddr, sizeof(mAddr) )) )
   {
      // error
      cerr << "Error binding socket" << endl;
      return false;
   }
   
   mSockOpen = true;
   
   return true;
}

void CUDPSocket::Send( const CUDPMessage & Msg )
{
   ssize_t RecVal = -1;
   if( Msg.mSize < 0 )
   {
      return;
   }
   
   if( mSockOpen && ((AF_INET == mDomain) && (SOCK_DGRAM == mType)) )
   {
      RecVal = sendto( mFDSocket, &(Msg.mMsgData), Msg.mSize, 0, 
                         &(Msg.mSenderAddr), Msg.mSenderAddrLen );
      
      if( RecVal < 0 )
      {
         // error
      }
      
   }
   else
   {
      // error
   }
   
   return;
}

int CUDPSocket::Recieve( CUDPMessage & Msg )
{
   ssize_t RecVal = -1;
  
   if( mSockOpen && ((AF_INET == mDomain) && (SOCK_DGRAM == mType)) )
   {
      Msg.mSize = CUDPMessage::MAX_MSG_SIZE;
      Msg.mSenderAddrLen = sizeof(Msg.mSenderAddr);
      RecVal = recvfrom( mFDSocket, &(Msg.mMsgData), Msg.mSize, 0, 
                         &(Msg.mSenderAddr), &(Msg.mSenderAddrLen) );
      
      if( RecVal >= 0 )
      {
         // recieved OK
         Msg.mSize = RecVal;
      }
      else
      {
         // error
         Msg.mSize = 0;  // invalid data, so make sure that no one looks at it.
      }
      
   }
   else
   {
      // error
   }
   
   return RecVal;
}


void CUDPMessage::DumpHex()
{
#ifdef DEBUG_BUILD
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
#endif
}


