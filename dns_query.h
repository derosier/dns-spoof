//****************************************************************************
// Module name : dns_query
// Project     : dns-spoof
// Interface (header) File
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
//   dns query 
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef DNSQUERY_H_
#define DNSQUERY_H_

// Don't change the following 3 lines.
// This checks and throws an error in case you
// didn't customize the above define.              
#ifdef MODULENAME_H_
#error Module name define not set in header
#endif

// Everything that you put in this file goes below
// this line.  It is importaint to keep everything
// within the #ifndef MODULENAME_H_ .. #endif pair 

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "dns_defs.h"
#include <string>
#include <iostream>

//****************************************************************************
//  Defines section
// Add all constants here
//
//****************************************************************************

//****************************************************************************
//  Function Prototype Section
// Add prototypes for all functions called by this
// module, with the exception of runtime routines 
// or prototypes already included via #include.
// 
//****************************************************************************

//****************************************************************************
//  Class Interface
// Define the module's class (or classes) here.
//
//****************************************************************************
class CDNS_Query 
{
public:
   CDNS_Query() : mQNameSize(0), mQType(0), mQClass(0) {};
   CDNS_Query( uint8_t * Data ) { Unpack( Data ); };
   
   ~CDNS_Query() {};

   int Pack( uint8_t * Data );
   int Unpack( uint8_t * Data );
   
   tDNSType QType( void ) { return mQType; };
   void QType( tDNSType Type ) { mQType = Type; };
   
   tDNSClass QClass( void ) { return mQClass; };
   void QClass( tDNSClass Class ) { mQClass = Class; };
   
   int QNameSize( void ) { return mQNameSize; };
   int GetQName( uint8_t * QName );
   void SetQName( uint8_t * QName );
   
   friend std::ostream & operator<<( std::ostream & s, const CDNS_Query & Query );

private:
   int mQNameSize;
   uint8_t mQName[DNS_NAME_MAX];
   tDNSType mQType;
   tDNSClass mQClass;
};


// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


