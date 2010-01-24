//****************************************************************************
// Module name : dns_msg
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
//   DNS message structure data.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include "dns_msg.h"
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
CDNS_Msg::~CDNS_Msg()
{
   if( mQuery )
   {
      delete mQuery;
      mQuery = 0;
   }

   if( mAnswer )
   {
      delete mAnswer;
      mAnswer = 0;
   }
}

int CDNS_Msg::Pack( uint8_t * Data )
{
   int Length = 0;
   Length = mHeader.Pack( Data );
   
   if( mQuery )
   {
      Length += mQuery->Pack( (Data + Length) );
   }

   if( mAnswer )
   {
      Length += mAnswer->Pack( (Data + Length) );
   }
   
   return Length;
}

void CDNS_Msg::Unpack( uint8_t * Data )
{
   int Offset = 0;

   // First things first, clean up if we have held a message before.
   if( mQuery )
   {
      delete mQuery;
      mQuery = 0;
   }

   if( mAnswer )
   {
      delete mAnswer;
      mAnswer = 0;
   }

   // Start by unpacking the header.  All unpack functions (other than this one) return the 
   // number of bytes they've used from Data.  This is necessary because the different sections
   // can be different lengths.
   Offset = mHeader.Unpack( Data );
   
   // FIXME: support multiple question sections.  For now we assume a single question.  This could break!
   if( mHeader.QDCount() > 0 )
   {
      mQuery = new CDNS_Query;
      Offset += mQuery->Unpack( Data + Offset );
   }

   // FIXME: support multiple answer sections.  For now we assume a single answer.  This could break!
   if( mHeader.ANCount() > 0 )
   {
      mAnswer = new CDNS_Answer;
      Offset += mAnswer->Unpack( Data + Offset );
   }

   // NOTE: CNDS_Msg doesn't support Authority or Additional sections at this time.   
}

ostream & operator<<( ostream & s, const CDNS_Msg & Msg)
{
   s << Msg.mHeader;
   if( Msg.mQuery )
   {
      s << *(Msg.mQuery);
   }
   
   if( Msg.mAnswer )
   {
      s << *(Msg.mAnswer);
   }
   
   return s;
}


