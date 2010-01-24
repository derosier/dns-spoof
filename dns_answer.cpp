//****************************************************************************
// Module name : dns_answer
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
//   Structure for DNS answer section.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "dns_answer.h"
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
int CDNS_Answer::Pack( uint8_t * Data )
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
   
   // TTL is next, 4 bytes
   mTTL = *((uint32_t *)(DataPtr));
   DataPtr += 4;
   Count += 4;
   
   // RDLength, 2 bytes
   mRDLength = *((uint16_t *)(DataPtr));
   DataPtr += 2;
   Count += 2;

   // RData, RDLength, but we will only take 4 bytes
   mRDLength = (mRDLength > 4) ? 4 : mRDLength;
   memcpy( mRData, DataPtr, mRDLength );
   Count += mRDLength;
   
   return Count;
}

int CDNS_Answer::Unpack( uint8_t * Data )
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

   // TTL is next, 4 bytes
   *((uint32_t *)(DataPtr)) = mTTL;
   DataPtr += 4;
   Count += 4;
   
   // RDLength, 2 bytes
   *((uint16_t *)(DataPtr)) = mRDLength;
   DataPtr += 2;
   Count += 2;

   // RData
   memcpy( DataPtr, mRData, mRDLength );
   Count += mRDLength;
   
   return Count;
}

int CDNS_Answer::GetName( uint8_t * Name )
{
   if( mQNameSize > 0 )
   {
      memcpy( Name, mQName, mQNameSize );
   }
   
   return mQNameSize;
}

void CDNS_Answer::SetName( uint8_t * Name )
{
   uint8_t * DataPtr = Name;
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
  
ostream & operator<<( ostream & s, const CDNS_Answer & Answer )
{
   char oldfill = s.fill();
   s.fill('0');

   s << "Answer" << endl;
   s << "  Type: " << int(Answer.mQType);
   s << "  Class: " << int(Answer.mQClass);
   s << "  NameSize: " << Answer.mQNameSize;
   s << "  Name: " << Name(Answer.mQName, Answer.mQNameSize) << endl;
   s << "  RawName: ";
   s << std::hex;
   for( int i = 0; i < Answer.mQNameSize; i++ )
   {
      s.fill('0');
      s.width(2);
      // The mask below defeats the sign-extension on the int cast
      s << (0x000000FF & (unsigned int)(Answer.mQName[i]));
      s << " ";
   }
   s << endl;
   s << dec;

   s << "  TTL: " << int(Answer.mTTL);
   s << "  RDLength: " << int(Answer.mRDLength);
   s << "  mRData: ";
   s << std::hex;
   for( int i = 0; i < Answer.mRDLength; i++ )
   {
      s.fill('0');
      s.width(2);
      // The mask below defeats the sign-extension on the int cast
      s << (0x000000FF & (unsigned int)(Answer.mRData[i]));
      s << " ";
   }
   s << endl;
   s << dec;

   s.fill(oldfill);  

   return s;
}


