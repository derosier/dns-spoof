//****************************************************************************
// Module name : dns_util
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
//   Utility helper functions for DNS classes.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include <string>
#include "dns_util.h"

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
// Function name: const string Name( const uint8_t * NamePtr, const int Size )
//    returns: string of the parsed name
//    NamePtr: Pointer to the name data to parse
//    Size: Size of the name data to parse.
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Name parses through a DNS domain name record and constructs
//   a readable string from it.  The record is an 8-bit binary string of data,
//   composed of label chunks. Each label starts with a 1-byte length, followed
//   by data.  This in turn is followed by another label, until a label of 
//   0 length is encoutered.  
//   For example, the domain name cal-sierra.com is (lengths are in numbers):
//     0a 63 61 6c 2d 73 69 65 72 72 61 03 63 6f 6d 00
//     10  c  a  l  -  s  i  e  r  r  a  3  c  o  m  0
//   Name() constructs the string by collecting all the labels and gluing 
//   them together with '.' as in a domain name.
//****************************************************************************
const string Name( const uint8_t * NamePtr, const int Size )
{
   if( Size <= 0 )
   {
      return "";
   }
   
   string N = "";
   
   // First part is the name, which is made of chunks of an 8bit length, followed
   // by that number of bytes.  End is a 0 length byte.
   while( *NamePtr )
   {
      string Part( (char *)(NamePtr+1), *NamePtr );
      N += Part + ".";
      NamePtr += (*NamePtr) + 1; // Don't forget the length byte
   }
   
   return N.substr(0, N.length()-1 );  // Truncate the extra '.' we tacked on the end of the loop above.
}

//****************************************************************************
// Function name: int CopyName( uint8_t * Dest, const uint8_t * Src )
//    returns: number of bytes copied
//    Dest: Pointer to detination buffer
//    Src: Pointer to data to copy
// Created by: Steve deRosier
// Date created: 1/24/2010
// Description: Copies name data from source buffer to destination buffer.
// Note: The destination buffer must be sized to be large enough for the largest
//       allowed name data per the RFC.  This function will forcefully
//       truncate this data to avoid exceeding the length of the buffer.
//****************************************************************************
int CopyName( uint8_t * Dest, uint8_t * Src )
{
   uint8_t * DataPtr = Src;
   uint8_t * QNamePtr = Dest;
   int Count = 0;
   
   // First part is the name, which is made of chunks of an 8bit length, followed
   // by that number of bytes.  End is a 0 length byte.
   while( (*DataPtr) && ((Count+1) < DNS_NAME_MAX))
   {
      int CopyLen = (*DataPtr) + 1;
      if( (Count + CopyLen) < DNS_NAME_MAX ) 
      {
         memcpy( QNamePtr, DataPtr, CopyLen ); // copy the length also!
      }
      DataPtr += CopyLen; // Don't forget the length byte
      QNamePtr += CopyLen;
      Count += CopyLen;
   }

   // append the 0 length byte to the end.
   *QNamePtr = 0;
   Count++;
   
   return Count;
}
