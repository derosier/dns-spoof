//****************************************************************************
// Module name : dns-spoof 
// Project     : dns-spoof
//
// Copyright (C) 2010 Steve deRosier.
// All rights reserved.
//
// The information contained herein is confidential 
// property of Company. The use, copying, transfer or 
// disclosure of such information is prohibited except
// by express written agreement with Company.
//
// First written on 1/21/2010 by Steve deRosier.
//
// Module Description:
// Main module of the dns-spoof program for Cozybit.
//
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include <iostream>
#include <string>
#include <getopt.h>

#include "version.h"

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
void PrintCopyright( void );
void PrintHelp( void );
void ExitClean( int ExitVal );
void Error( const string & ErrStr );


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
int main( int argc, char * argv[] )
{
   ///////////////////////////////////
   // Process Commandline
   ///////////////////////////////////
   
   // Define the options structure 
   static struct option longopt[] =
   {
      {"console",no_argument,NULL,'c'},
      {"help",no_argument,NULL,'h'},
      {"version",no_argument,NULL,'v'},
      {NULL, 0, NULL, 0}
   };
   
   // Printout the comandline for fun.
#ifdef DEBUG_BUILD
   string cmdline = "Program command: ";
   for( int i = 0; i < argc; i++ )
   {
      cmdline += argv[i];
      cmdline += " ";
   }
   cout << cmdline << endl;
#endif
   
   // Setup defaults.  These will be replaced by comandline options.
   bool ConsoleMode = false;
   
   // Process comandline options
   int c;
   int optidx=0;
   while ((c=getopt_long(argc,argv,"cvh",longopt,&optidx)) != -1)
   {
      switch(c)
      {
         // Console mode -> don't daemonize
         case 'c':
#ifdef DEBUG_BUILD
            cout << "Console mode enabled" << endl;
#endif         
            ConsoleMode = true;
            break;
         
         // Version
         case 'v':
            PrintCopyright();
            ExitClean(0);
         // Help 
         case 'h':
            PrintCopyright();
            PrintHelp();
            ExitClean( 0 );
            break;
      }
   }
   
   
   
   
   std::cout << "Hello, this is " << argv[0] << std::endl;
   
   ExitClean( 0 );
}


//****************************************************************************
// Function name: void PrintCopyright( void )
//    returns: nothing
// Created by: Steve deRosier
// Date created: 1/21/2010
// Description: Prints our copyright message.  Grabs the project name, 
//              version number and copyright from version.h
//****************************************************************************
void PrintCopyright( void )
{
/* Display the copyright message */
cout << "=================================================================" << endl;
cout << _PROJECT << " " << _VERSION << endl;
cout << _COPYRIGHT << endl;
cout << "=================================================================" << endl;

}

//****************************************************************************
// Function name: void PrintHelp( void ) 
//    returns: nothing
// Created by: Steve deRosier
// Date created: 1/21/2010
// Description: prints help and usage to cerr.
//****************************************************************************
void PrintHelp( void )
{
cerr << _PROJECT << " spoofs a DNS response to a hard-coded address.\n";
cerr << "usage: " << _PROJECT << " [options]\n";
cerr << endl;
cerr << "    -c --console   Don't daemonize, run in console mode.\n";
cerr << "    -v --version   Display the version and quit.\n";
cerr << "    -h --help      Display this message.\n";
cerr << endl;
}

//****************************************************************************
// Function name: void ExitClean( int ExitVal ) 
//    returns: NOTE: Does not return
//    ExitVal: Value to pass to the exit() function.
// Created by: Steve deRosier
// Date created: 1/21/2010
// Description: Does the cleanup of main() vars and then exits with the 
//              exit code given
//****************************************************************************
void ExitClean( int ExitVal )
{
//  using namespace MainLocals;

// Might as well make sure all output got out.
cout << flush;

// Call exit.  0 means normal exit, other values are error codes.  ExitVal
// is given to us by the caller to indicate
exit( ExitVal ); 

}


//****************************************************************************
// Function name: void Error( const string & ErrStr )
//    returns: none
//     ErrStr: Text string to write out.
// Created by: Steve deRosier
// Date created: 1/21/2010
// Description: Outputs an error to stderr 
//****************************************************************************
void Error( const string & ErrStr )
{
cerr << "Error: " << ErrStr << endl;
}
