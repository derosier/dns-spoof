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
// Function name: TYPE foo(TYPE arg1, TYPE arg2)
//    returns: return value description
//    arg1: description
//    arg2: description
// Created by: author's name
// Date created: date
// Description: detailed description 
// Notes: restrictions, odd modes
//****************************************************************************
CDNS_Header::CDNS_Header( uint8_t * Data )
 : mID(0), mFlags(0), mQDCount(0), mANCount(0), mNSCount(0), mARCount(0) 
{
   Unpack( Data );
}


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




