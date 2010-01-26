//****************************************************************************
// Module name : dns_header
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
//   Header for DNS messages.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "dns_header.h"
#include <arpa/inet.h>
using namespace std;

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
// Function name: CDNS_Header::CDNS_Header( uint8_t * Data )
//    Data: Message data buffer to process
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Constructor to create a new header object from existing
//   message data.
//****************************************************************************
CDNS_Header::CDNS_Header( uint8_t * Data )
 : mID(0), mFlags(0), mQDCount(0), mANCount(0), mNSCount(0), mARCount(0) 
{
   Unpack( Data );
}


//****************************************************************************
// Function name: int CDNS_Header::Pack( uint8_t * Data )
//    returns: the number of bytes put into Data
//    Data: pointer to the destination buffer
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Pack() takes the data in CDNS_Header and packs it into a binary
//   representation that can be used in a DNS header message over the wire.
//****************************************************************************
int CDNS_Header::Pack( uint8_t * Data )
{
   uint16_t * Ptr = (uint16_t *)Data;
   *Ptr++ = htons(mID);
   *Ptr++ = htons(mFlags);
   *Ptr++ = htons(mQDCount);
   *Ptr++ = htons(mANCount);
   *Ptr++ = htons(mNSCount);
   *Ptr++ = htons(mARCount);

   return 12;  // Size of a DNS header
}


//****************************************************************************
// Function name: int CDNS_Header::Unpack( uint8_t * Data )
//    returns: number of bytes consumed from Data
//    Data: Pointer to the data to un-pack
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Parses the header message data from Data and places into the class
//   instance so it can be used.  Returns the number of bytes parsed through
//   so the caller is able to know the start position for the next portion
//   of the message so it can be passed to other Unpack() functions.
//****************************************************************************
int CDNS_Header::Unpack( uint8_t * Data )
{
   uint16_t * Ptr = (uint16_t *)Data;
   mID = ntohs(*Ptr++);
   mFlags = ntohs(*Ptr++);
   mQDCount = ntohs(*Ptr++);
   mANCount = ntohs(*Ptr++);
   mNSCount = ntohs(*Ptr++);
   mARCount = ntohs(*Ptr++);
   
   return 12;  // Size of a DNS header
}

//****************************************************************************
// Function name: ostream & operator<<( ostream & s, const CDNS_Header & Hdr)
//    returns: ostream &
//    s: ostream to write to
//    Hdr: Hdr object to write
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Standard operator<< overload to write debugging info about Hdr 
//****************************************************************************
ostream & operator<<( ostream & s, const CDNS_Header & Hdr)
{
   char oldfill = s.fill();
   s.fill('0');

   s << "Header" << endl;
   // We use masks below to defeat the sign-extension on the int cast
   s << std::hex;
   s << "  Id: ";
   s.width(4);
   s << (0x0000FFFF & Hdr.mID);
   s << " Flags: ";
   s.width(4);
   s << (0x0000FFFF & Hdr.mFlags);
   s << " QDCount: ";
   s.width(4);
   s << (0x0000FFFF & Hdr.mQDCount);
   s << " ANCount: ";
   s.width(4);
   s << (0x0000FFFF & Hdr.mANCount);
   s << " NSCount: ";
   s.width(4);
   s << (0x0000FFFF & Hdr.mNSCount);
   s << " ARCount: ";
   s.width(4);
   s << (0x0000FFFF & Hdr.mARCount);

   s << endl;
   s.fill(oldfill);  

   return s;
}




