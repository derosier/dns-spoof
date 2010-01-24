//****************************************************************************
// Module name : dns_util
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
//   Utility helper functions for DNS classes.
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef DNS_UTIL_H_
#define DNS_UTIL_H_

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
#include <string>
#include "dns_defs.h"

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
const std::string Name( const uint8_t * NamePtr, const int Size );

//****************************************************************************
//  Class Interface
// Define the module's class (or classes) here.
//
//****************************************************************************

// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


