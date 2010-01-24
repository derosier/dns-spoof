//****************************************************************************
// Module name : dns_defs 
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
//   Constants and type definitions for DNS.
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef DNS_DEFS_H_
#define DNS_DEFS_H_

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
#include <stdint.h>

//****************************************************************************
//  Defines section
// Add all constants here
//
//****************************************************************************
typedef uint16_t tDNSType;

const tDNSType DNS_TYPE_A = 1;
const tDNSType DNS_TYPE_NS = 2; 
const tDNSType DNS_TYPE_MD = 3;
const tDNSType DNS_TYPE_MF = 4; 
const tDNSType DNS_TYPE_CNAME = 5;
const tDNSType DNS_TYPE_SOA = 6; 
const tDNSType DNS_TYPE_MB = 7;
const tDNSType DNS_TYPE_MG = 8; 
const tDNSType DNS_TYPE_MR = 9;
const tDNSType DNS_TYPE_NULL = 10; 
const tDNSType DNS_TYPE_WKS = 11;
const tDNSType DNS_TYPE_PTR = 12; 
const tDNSType DNS_TYPE_HINFO = 13;
const tDNSType DNS_TYPE_MINFO = 14; 
const tDNSType DNS_TYPE_MX = 15;
const tDNSType DNS_TYPE_TXT = 16; 

const tDNSType DNS_QTYPE_AXFR = 252;
const tDNSType DNS_QTYPE_MAILB = 253; 
const tDNSType DNS_QTYPE_MAILA = 254;
const tDNSType DNS_QTYPE_ALL = 255; 

typedef uint16_t tDNSClass;

const tDNSClass DNS_CLASS_IN = 1;
const tDNSClass DNS_CLASS_CS = 2;
const tDNSClass DNS_CLASS_CH = 3;
const tDNSClass DNS_CLASS_HS = 4;
const tDNSClass DNS_CLASS_ALL = 255;

const int DNS_LABEL_MAX = 63;
const int DNS_NAME_MAX = 255;


// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


