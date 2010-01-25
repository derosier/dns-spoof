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
// Function name: int CDNS_Answer::Pack( uint8_t * Data )
//    returns: the number of bytes put into Data
//    Data: pointer to the destination buffer
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Pack() takes the data in CDNS_Answer and packs it into a binary
//   representation that can be used in a DNS answer message over the wire.
//****************************************************************************
int CDNS_Answer::Pack( uint8_t * Data )
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

   // TTL is next, 4 bytes
   *((uint32_t *)(DataPtr)) = htonl(mTTL);
   DataPtr += 4;
   Count += 4;
   
   // RDLength, 2 bytes
   *((uint16_t *)(DataPtr)) = htons(mRDLength);
   DataPtr += 2;
   Count += 2;

   // RData
   memcpy( DataPtr, mRData, mRDLength );
   Count += mRDLength;
   
   return Count;
}


//****************************************************************************
// Function name: int CDNS_Answer::Unpack( uint8_t * Data )
//    returns: number of bytes consumed from Data
//    Data: Pointer to the data to un-pack
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Parses the query message data from Data and places into the class
//   instance so it can be used.  Returns the number of bytes parsed through
//   so the caller is able to know the start position for the next portion
//   of the message so it can be passed to other Unpack() functions.
//****************************************************************************
int CDNS_Answer::Unpack( uint8_t * Data )
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
   
   // TTL is next, 4 bytes
   mTTL = ntohl(*((uint32_t *)(DataPtr)));
   DataPtr += 4;
   Count += 4;
   
   // RDLength, 2 bytes
   mRDLength = ntohs(*((uint16_t *)(DataPtr)));
   DataPtr += 2;
   Count += 2;

   // RData, RDLength, but we will only take 4 bytes
   mRDLength = (mRDLength > 4) ? 4 : mRDLength;
   memcpy( mRData, DataPtr, mRDLength );
   Count += mRDLength;
   
   return Count;
}


//****************************************************************************
// Function name: int CDNS_Answer::GetName( uint8_t * Name )
//    returns: size of name copied to the buffer
//    Name: Buffer to copy the name to
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Copies name data from instance to Name buffer.
// Notes: Name buffer must be at least DNS_NAME_MAX in size.
//****************************************************************************
int CDNS_Answer::GetName( uint8_t * Name )
{
   if( mQNameSize > 0 )
   {
      memcpy( Name, mQName, mQNameSize );
   }
   
   return mQNameSize;
}

//****************************************************************************
// Function name: void CDNS_Answer::SetName( uint8_t * Name )
//    returns: nothing
//    Name: Name data to set instance to.
//    arg2: description
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Copies Name data in to instance variable.
// Note: CopyName can forcefully truncate Name data if it is too large.  However
//       any data that is too large violates the RFC, so this shouldn't happen.
//****************************************************************************
void CDNS_Answer::SetName( uint8_t * Name )
{
   mQNameSize = CopyName(mQName, Name);
}
  
//****************************************************************************
// Function name: ostream & operator<<( ostream & s, const CDNS_Answer & Answer )
//    returns: ostream &
//    s: ostream to write to
//    Answer: Query object to write
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Standard operator<< overload to write debugging info about Answer 
//****************************************************************************
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


