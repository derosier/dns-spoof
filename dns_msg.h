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
   CDNS_Msg( int size, char * Data ) : mQuery(0), mAnswer(0) { Unpack( size, Data ); };
   
   ~CDNS_Msg();
   
   int Pack( char * Data );
   void Unpack( int size, char * Data );

private:
   CDNS_Header mHeader;
   CDNS_Query * mQuery;
   CDNS_Answer * mAnswer;   
};

// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


