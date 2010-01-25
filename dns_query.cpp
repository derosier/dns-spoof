//****************************************************************************
// Module name : dns_query
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
//   DNS querry structure.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "dns_query.h"
#include <string>
#include <cstring>
#include "dns_util.h"

//****************************************************************************
//  Defines section
// Add all #defines or consts here
//
//****************************************************************************
using namespace std;

//****************************************************************************
//  Function Prototype Section
// Add prototypes for all functions called by this
// module, with the exception of runtime routines,
// or prototypes already included via header files.
//****************************************************************************

//****************************************************************************
// Function name: int CDNS_Query::Pack( uint8_t * Data )
//    returns: the number of bytes put into Data
//    Data: pointer to the destination buffer
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Pack() takes the data in CDNS_Query and packs it into a binary
//   representation that can be used in a DNS query message over the wire.
//****************************************************************************
int CDNS_Query::Pack( uint8_t * Data )
{
   if( mQNameSize == 0 )
     return 0;
     
   uint8_t * DataPtr = Data;
   int Count = 0;
   
   // First part is the name, which is made of chunks of an 8bit length, followed
   // by that number of bytes.  End is a 0 length byte.  Conviently we know the size 
   // we're storing.
   memcpy( DataPtr, mQName, mQNameSize );
   Count += mQNameSize;
   DataPtr += mQNameSize;
   
   // Do type and class, each are 2 bytes
   *((uint16_t *)(DataPtr)) = htons(mQType);
   DataPtr += 2;
   Count += 2;
   *((uint16_t *)(DataPtr)) = htons(mQClass);
   DataPtr += 2;
   Count += 2;
   
   return Count;
}

//****************************************************************************
// Function name: int CDNS_Query::Unpack( uint8_t * Data )
//    returns: number of bytes consumed from Data
//    Data: Pointer to the data to un-pack
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Parses the query message data from Data and places into the class
//   instance so it can be used.  Returns the number of bytes parsed through
//   so the caller is able to know the start position for the next portion
//   of the message so it can be passed to other Unpack() functions.
//****************************************************************************
int CDNS_Query::Unpack( uint8_t * Data )
{
   uint8_t * DataPtr = Data;
   int Count = 0;

   // First grab the name, which must be parsed to figure out the length
   Count = CopyName(mQName, Data);
   mQNameSize = Count;
   DataPtr += Count;
   
   // Do type and class, each are 2 bytes
   mQType = ntohs(*((uint16_t *)(DataPtr)));
   DataPtr += 2;
   Count += 2;
   mQClass = ntohs(*((uint16_t *)(DataPtr)));
   DataPtr += 2;
   Count += 2;
   
   return Count;
}

   
//****************************************************************************
// Function name: int CDNS_Query::GetQName( uint8_t * QName )
//    returns: number of bytes placed in QName buffer
//    QName: destination buffer
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: copies the name data to the QName destination buffer.
// Note: destination buffer needs to be at least DNS_NAME_MAX in size.
//****************************************************************************
int CDNS_Query::GetQName( uint8_t * QName )
{
   if( mQNameSize > 0 )
   {
      memcpy( QName, mQName, mQNameSize );
   }
   
   return mQNameSize;
}

//****************************************************************************
// Function name: void CDNS_Query::SetQName( uint8_t * QName )
//    returns: nothing
//    QName: Name to set 
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Copies name data from source buffer to class data.
// Note: The destination buffer is sized to be large enough for the largest
//       allowed name data per the RFC.  However, the called function will forcefully
//       truncate this data to avoid exceeding the length of the buffer.
//****************************************************************************
void CDNS_Query::SetQName( uint8_t * QName )
{
   mQNameSize = CopyName(mQName, QName);
}

//****************************************************************************
// Function name: ostream & operator<<( ostream & s, const CDNS_Query & Query )
//    returns: ostream &
//    s: ostream to write to
//    Query: Query object to write
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Standard operator<< overload to write debugging info about Query 
//****************************************************************************
ostream & operator<<( ostream & s, const CDNS_Query & Query )
{
   char oldfill = s.fill();
   s.fill('0');

   s << "Question" << endl;
   s << "  Type: " << int(Query.mQType);
   s << "  Class: " << int(Query.mQClass);
   s << "  NameSize: " << Query.mQNameSize;
   s << "  Name: " << Name(Query.mQName, Query.mQNameSize) << endl;
   s << "  RawName: ";
   s << std::hex;
   for( int i = 0; i < Query.mQNameSize; i++ )
   {
      s.fill('0');
      s.width(2);
      // The mask below defeats the sign-extension on the int cast
      s << (0x000000FF & (unsigned int)(Query.mQName[i]));
      s << " ";
   }

   s << endl;
   s << dec;
   s.fill(oldfill);  

   return s;
}

