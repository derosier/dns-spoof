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
// Function name: TYPE foo(TYPE arg1, TYPE arg2)
//    returns: return value description
//    arg1: description
//    arg2: description
// Created by: author's name
// Date created: date
// Description: detailed description 
// Notes: restrictions, odd modes
//****************************************************************************
int CDNS_Query::Pack( uint8_t * Data )
{
   uint8_t * DataPtr = Data;
   uint8_t * QNamePtr = mQName;
   int Count = 0;
   
   // First part is the name, which is made of chunks of an 8bit length, followed
   // by that number of bytes.  End is a 0 length byte.
   while( !(*DataPtr) )
   {
      int CopyLen = (*DataPtr) + 1;
      memcpy( QNamePtr, DataPtr, CopyLen ); // copy the length also!
      DataPtr += CopyLen; // Don't forget the length byte
      QNamePtr += CopyLen;
      Count += CopyLen;
   }
   // append the 0 length byte to the end.
   *QNamePtr = 0;
   Count++;
   mQNameSize = Count;
   DataPtr++;
   
   // Do type and class, each are 2 bytes
   mQType = *((uint16_t *)(DataPtr));
   DataPtr += 2;
   Count += 2;
   mQClass = *((uint16_t *)(DataPtr));
   DataPtr += 2;
   Count += 2;
   
   return Count;
}

int CDNS_Query::Unpack( uint8_t * Data )
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
   *((uint16_t *)(DataPtr)) = mQType;
   DataPtr += 2;
   Count += 2;
   *((uint16_t *)(DataPtr)) = mQClass;
   DataPtr += 2;
   Count += 2;
   
   return Count;
}
   
int CDNS_Query::GetQName( uint8_t * QName )
{
   if( mQNameSize > 0 )
   {
      memcpy( QName, mQName, mQNameSize );
   }
   
   return mQNameSize;
}

void CDNS_Query::SetQName( uint8_t * QName )
{
   uint8_t * DataPtr = QName;
   uint8_t * QNamePtr = mQName;
   int Count = 0;
   
   // First part is the name, which is made of chunks of an 8bit length, followed
   // by that number of bytes.  End is a 0 length byte.
   while( !(*DataPtr) )
   {
      int CopyLen = (*DataPtr) + 1;
      memcpy( QNamePtr, DataPtr, CopyLen ); // copy the length also!
      DataPtr += CopyLen; // Don't forget the length byte
      QNamePtr += CopyLen;
      Count += CopyLen;
   }
   // append the 0 length byte to the end.
   *QNamePtr = 0;
   Count++;
   mQNameSize = Count;
}

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

