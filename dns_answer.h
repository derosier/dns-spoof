//****************************************************************************
// Module name : dns_answer
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
//   Structure for DNS answer section.
//
// NOTE: dns-spoof only requires returning an IP address.  Thus, CDNS_Answer
//       is built to only support this type of RDATA.  It is critical to
//       change this if it this class is ever to be used in any general 
//       purpose DNS application.
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef DNSANSWER_H_
#define DNSANSWER_H_

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
class CDNS_Answer 
{
public:
   CDNS_Answer() : mQNameSize(0), mQType(0), mQClass(0) {};
   ~CDNS_Answer() {};
   
   int Pack( uint8_t * Data );
   int Unpack( uint8_t * Data );
   
   tDNSType QType( void ) { return mQType; };
   void QType( tDNSType Type ) { mQType = Type; };
   
   tDNSClass QClass( void ) { return mQClass; };
   void QClass( tDNSClass Class ) { mQClass = Class; };
   
   uint32_t TTL( void ) { return mTTL; };
   void TTL( uint32_t ttl ) { mTTL = ttl; };
   
   uint16_t RDLength( void ) { return mRDLength; };
   void RDLength( uint16_t RDL ) { mRDLength = RDL; };
   
   uint32_t RData( void ) { return *((uint32_t *)mRData); };
   void RData( uint32_t RD ) { *((uint32_t *)mRData) = RD; };
   
   int NameSize( void ) { return mQNameSize; };
   int GetName( uint8_t * Name );
   void SetName( uint8_t * Name );

   friend std::ostream & operator<<( std::ostream & s, const CDNS_Answer & Answer);

private:
   int mQNameSize;
   uint8_t mQName[DNS_NAME_MAX];
   tDNSType mQType;
   tDNSClass mQClass;
   uint32_t mTTL;
   uint16_t mRDLength;
   uint8_t mRData[4];  // Note, we only support IP address for dns-spoof

};

// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


