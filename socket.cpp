//****************************************************************************
// Module name : Socket
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
// First written on 1/22/2010 by Steve deRosier.
//
// Module Description:
//   Main socket object.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "socket.h"
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
CSocket::CSocket() : mSockOpen(false)
{
}

CSocket::~CSocket()
{
}
   
void CSocket::Close( void )
{
mSockOpen = false;
}

bool CSocket::OpenUDP( int Port )
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

int CSocket::RecieveUDP( CUDPMessage & Msg )
{
  
   if( mSockOpen && ((AF_INET == mDomain) && (SOCK_DGRAM == mType)) )
   {
      ssize_t RecVal;
      Msg.mSize = CUDPMessage::MAX_MSG_SIZE;
      Msg.mSenderAddrLen = sizeof(Msg.mSenderAddr);
      RecVal = recvfrom( mFDSocket, &(Msg.mMsgData), Msg.mSize, 0, 
                         &(Msg.mSenderAddr), &(Msg.mSenderAddrLen) );
      
      if( 0 > RecVal )
      {
         // error
      }
      
      Msg.mSize = RecVal;
      return RecVal;
   }
   else
   {
      // error
   }
   
   return -1;
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
