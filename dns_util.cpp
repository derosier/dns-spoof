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
//  $Id: module_template.cpp 29 2010-01-22 18:17:49Z derosier $
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
// Function name: TYPE foo(TYPE arg1, TYPE arg2)
//    returns: return value description
//    arg1: description
//    arg2: description
// Created by: author's name
// Date created: date
// Description: detailed description 
// Notes: restrictions, odd modes
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
   while( !(*NamePtr) )
   {
      string Part( (char *)(NamePtr+1), *NamePtr );
      N += Part + ".";
      NamePtr += (*NamePtr) + 1; // Don't forget the length byte
   }
   
   return N.substr(0, N.length()-1 );  // Truncate the extra '.' we tacked on the end of the loop above.
}


