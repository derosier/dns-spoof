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
// Function name: CDNS_Msg::~CDNS_Msg()
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: destructor
// Notes: Will clean up both the Query and Answer objects if they exist.
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

//****************************************************************************
// Function name: int CDNS_Msg::Pack( uint8_t * Data )
//    returns: the number of bytes put into Data
//    Data: pointer to the destination buffer
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Pack() takes the data in CDNS_Msg and packs it into a binary
//   representation that can be used in a DNS message over the wire.
//****************************************************************************
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

//****************************************************************************
// Function name: void CDNS_Msg::Unpack( uint8_t * Data )
//    returns: nothing
//    Data: Pointer to the data to un-pack
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Parses the message data from Data and places into the class
//   instance so it can be used.
// Notes: * CDNS_Msg doesn't support Authority or Additional sections at this time. 
//          If these are pressent, they will be ignored.
//        * CDNS_Msg only supports a single query and answer section each. 
//          Further ones will both be ignored, and further an extra query section
//          will screw up the processing of the answer section.
//****************************************************************************
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

   // NOTE: CDNS_Msg doesn't support Authority or Additional sections at this time.   
}

//****************************************************************************
// Function name: void CDNS_Msg::Query( const CDNS_Query & Data )
//    returns: nothing
//    Data: Query data to set 
// Created by: Steve deRosier
// Date created: 1/24/2010
// Description: Set the query section of the message.
//****************************************************************************
void CDNS_Msg::Query( const CDNS_Query & Data )
{
   if( mQuery )
   {
      delete mQuery;
      mQuery = 0;
   }
   
   mQuery = new CDNS_Query;
   
   *mQuery = Data;
}

//****************************************************************************
// Function name: CDNS_Query CDNS_Msg::Query( void )
//    returns: Query data
// Created by: Steve deRosier
// Date created: 1/24/2010
// Description: Get the query section of the message.
//****************************************************************************
CDNS_Query CDNS_Msg::Query( void )
{
   CDNS_Query Q;
   if( mQuery )
      Q = *mQuery;
   
   return Q;
}
   
//****************************************************************************
// Function name: void CDNS_Msg::Answer( const CDNS_Answer & Data )
//    returns: nothing
//    Data: Answer data to set 
// Created by: Steve deRosier
// Date created: 1/24/2010
// Description: Set the answer section of the message.
//****************************************************************************
void CDNS_Msg::Answer( const CDNS_Answer & Data )
{
   if( mAnswer )
   {
     delete mAnswer;
     mAnswer = 0;
   }
     
   mAnswer = new CDNS_Answer;
   
   *mAnswer = Data;
}

//****************************************************************************
// Function name: CDNS_Answer CDNS_Msg::Answer( void )
//    returns: Answer data
// Created by: Steve deRosier
// Date created: 1/24/2010
// Description: Get the answer section of the message.
//****************************************************************************
CDNS_Answer CDNS_Msg::Answer( void )
{
   CDNS_Answer A;
   if( mAnswer )
      A = *mAnswer;
      
   return A;
}



//****************************************************************************
// Function name: ostream & operator<<( ostream & s, const CDNS_Msg & Msg)
//    returns: ostream &
//    s: ostream to write to
//    Msg: Msg object to write
// Created by: Steve deRosier
// Date created: 1/23/2010
// Description: Standard operator<< overload to write debugging info about Msg 
//****************************************************************************
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


