//****************************************************************************
// Module name : CError
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
// First written on 1/22/2010 by Steve deRosier.
//
// Module Description:
//   Error object for try/catch reporting
//
//****************************************************************************
//  $Id$
//****************************************************************************

// Change MODULENAME_H_ to match your module name.
// This ensures that this header doesn't get
// included multiple times accidently              
#ifndef CERROR_H_
#define CERROR_H_

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

// Generic error class
class CError 
{
public:
   virtual ~CError() {};
   
   virtual const std::string ErrString(void) { return "CError"; };
   virtual int ErrValue(void) { return 0xFFFFFFFF; };
};

// CE_DaemonFail
class CE_DaemonFail : public CError
{
public:
   enum tError { BAD_FORK, FAIL_SID, FAIL_CHDIR };
   
   CE_DaemonFail( const tError Type ) : mType(Type) { }; 
   virtual ~CE_DaemonFail() {};
   
   const std::string ErrString(void)
   {
      std::string RVal = "unknown failure";
      switch( mType )
      {
         case BAD_FORK:
            RVal = "call to fork failed";
            break;
         case FAIL_SID:
            RVal = "call to setsid failed";
            break;
         case FAIL_CHDIR:
            RVal = "call to chdir failed";
            break;
      }
      
      return RVal;
   }
   
   int ErrValue(void) { return mType; };
   
protected:
   tError mType;
   
};

class CE_Socket : public CError
{
};

extern std::string UDP_ErrorStrings[];

class CE_UDPSocket : public CE_Socket
{
public:
   enum tError { UNKNOWN = 0, SOCK_CREATE, SOCK_BIND, SOCK_CLOSE, 
                 SOCK_NOT_OPEN, SOCK_RX, SOCK_TX, SOCK_TYPE, MSG_SIZE_ZERO, 
                 MSG_ROUTE,
                 MAX_ERROR };
   
   CE_UDPSocket( const tError Type ) : mType(Type) {};
   virtual ~CE_UDPSocket() {};
   
   virtual tError Type(void) { return mType; };
   
   const std::string ErrString( void )
   {
      return UDP_ErrorStrings[mType];
   }

private:
   tError mType;
};

// Don't touch this last line.  Nothing should be 
// entered below here.
#endif


