//****************************************************************************
// Module name : dns_msg
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
//   DNS message structure data.
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef DNSMSG_H_
#define DNSMSG_H_

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
#include "dns_header.h"
#include "dns_query.h"
#include "dns_msg.h"
#include "dns_defs.h"
#include "dns_answer.h"
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
class CDNS_Msg
{
public:
   CDNS_Msg() : mQuery(0), mAnswer(0) {};
   CDNS_Msg( uint8_t * Data ) : mQuery(0), mAnswer(0) { Unpack( Data ); };
   
   ~CDNS_Msg();
   
   int Pack( uint8_t * Data );
   void Unpack( uint8_t * Data );

   friend std::ostream & operator<<( std::ostream & s, const CDNS_Msg & Msg);

   CDNS_Header mHeader;
   CDNS_Query * mQuery;
   CDNS_Answer * mAnswer;   
};

// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


