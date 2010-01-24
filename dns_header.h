//****************************************************************************
// Module name : dns_header
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
//   Header for DNS messages.
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef DNSHEADER_H_
#define DNSHEADER_H_

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
#define QRMASK     0x8000
#define OPCODEMASK 0x7800 
#define AAMASK     0x0400
#define TCMASK     0x0200
#define RDMASK     0x0100
#define RAMASK     0x0080
#define RCODEMASK  0x000F

typedef unsigned int tMsgType;
#define QUERY    0
#define RESPONSE 1

typedef unsigned int tOpType;
#define QUERY   0
#define IQUERY  1
#define STATUS  2

typedef unsigned int tRCode;



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
class CDNS_Header
{
public:
   CDNS_Header() : mID(0), mFlags(0), mQDCount(0), mANCount(0), mNSCount(0), mARCount(0) {};
   CDNS_Header( uint8_t * Data );
   
   ~CDNS_Header() {};
   
   int Pack( uint8_t * Data );
   int Unpack( uint8_t * Data );

   // Access functions for all data members.
   uint16_t ID( void ) { return mID; };
   void ID( uint16_t id ) { mID = id; };

   tMsgType Type( void ) { return (mFlags & QRMASK); };
   void Type( tMsgType t ) { mFlags |= (QRMASK & (t<<15)); };
   
   tOpType OpCode( void ) { return ((mFlags & OPCODEMASK) >> 11); };
   void OpCode( tOpType t ) { mFlags |= (t << 11); };
   
   bool AA( void ) { return (mFlags & AAMASK); };
   void AA( bool a ) { if(a){ mFlags |= AAMASK; } 
                       else{ mFlags &= ~AAMASK; } 
                     };
   
   bool Trunc( void ) { return (mFlags & TCMASK); };
   void Trunc( bool t ) { if(t){ mFlags |= TCMASK; } 
                          else{ mFlags &= ~TCMASK; } 
                        };
   
   bool Recurse( void ) { return (mFlags & RDMASK); };
   void Recurse( bool r ) { if(r){ mFlags |= RDMASK; } 
                            else{ mFlags &= ~RDMASK; } 
                          };
   
   bool RecurseAvail( void ) { return (mFlags & RAMASK); };
   void RecurseAvail( bool r ) { if(r){ mFlags |= RAMASK; } 
                                 else{ mFlags &= ~RAMASK; }
                               };
   
   tRCode Response( void ) { return (mFlags & RCODEMASK); };
   void Response( tRCode r ) { mFlags |= (r & RCODEMASK); };
   
   uint16_t QDCount( void ) { return mQDCount; };
   void QDCount( uint16_t qdc ) { mQDCount = qdc; };
   
   uint16_t ANCount( void ) { return mANCount; };
   void ANCount( uint16_t anc ) { mANCount = anc; };
   
   uint16_t NSCount( void ) { return mNSCount; };
   void NSCount( uint16_t nsc ) { mNSCount = nsc; };
   
   uint16_t ARCount( void ) { return mARCount; };
   void ARCount( uint16_t arc ) { mARCount = arc; };

   friend std::ostream & operator<<( std::ostream & s, const CDNS_Header & Hdr);
   
private:
   uint16_t mID;
   uint16_t mFlags;
   uint16_t mQDCount;
   uint16_t mANCount;
   uint16_t mNSCount;
   uint16_t mARCount;
};

// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


