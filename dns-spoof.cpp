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
// $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include <iostream>
#include <string>
#include <getopt.h>

// Below required for daemonizing code
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

// My include
#include "version.h"
#include "server.h"
#include "cerror.h"

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
void Daemonize( void );

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
   int Port = 53;
   ///////////////////////////////////
   // Process Commandline
   ///////////////////////////////////
   
   // Define the options structure 
   static struct option longopt[] =
   {
      {"port", required_argument, NULL, 'p'},
      {"console", no_argument, NULL, 'c'},
      {"help", no_argument, NULL, 'h'},
      {"version", no_argument, NULL, 'v'},
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
   while ((c=getopt_long(argc,argv,"p:cvh",longopt,&optidx)) != -1)
   {
      switch(c)
      {
         case 'p':
            Port = atoi(optarg); 
#ifdef DEBUG_BUILD
            cout << "Using port: " << Port << endl;
#endif         
            break;
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
   
   
   ///////////////////////////////////
   // Do setup tasks
   ///////////////////////////////////
   if( !ConsoleMode )
   {
#ifdef DEBUG_BUILD
      cout << "Daemonizing" << endl;
#endif         
      try
      {
         Daemonize();
      }
      catch( CE_DaemonFail & e )
      {
         cerr << "Error: unable to daemonize the process -> " << e.ErrString() << endl;
         ExitClean( EXIT_FAILURE );
      }
      catch(...)
      {
         cerr << "Error: Unexpected error." << endl;
         ExitClean( EXIT_FAILURE );
      }
   }
   else
   {
#ifdef DEBUG_BUILD
      cout << "--console flag given, not going to Daemon mode" << endl;
#endif         
   }
   

   ///////////////////////////////////
   // Run main server
   ///////////////////////////////////
   CServer SpoofServer( Port );
   
   SpoofServer.RunServer();

   ExitClean( EXIT_SUCCESS );
}

//****************************************************************************
// Function name: void Daemonize( void )
//    returns: nothing
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description: Deamonizes the process.  
// Credit: Adapted from code by Devin Watson under the BSD license.
//         http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
//****************************************************************************
void Daemonize( void )
{
   pid_t pid;  // our process id
   
   // Fork off the parent process
   pid = fork();
   if( pid < 0 )
   {
      throw CE_DaemonFail( CE_DaemonFail::BAD_FORK );
   }
   
   // If we got a good PID, then we can exit the parent process.
   if( pid > 0 ) 
   {
      ExitClean(EXIT_SUCCESS);
   }
   
   // Change the file mode mask
   umask(0);
   
   // Open any logs here 
   
   // Create a new SID for the child process 
   pid_t sid;  // Our session id
   sid = setsid();
   if( sid < 0 ) 
   {
      // Log the failure 
      throw CE_DaemonFail( CE_DaemonFail::FAIL_SID );
   }

   // Change the current working directory
   if( (chdir("/")) < 0 ) 
   {
      // Log the failure
      throw CE_DaemonFail( CE_DaemonFail::FAIL_CHDIR );
   }
   
   // Close out the standard file descriptors
   close(STDIN_FILENO);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);
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
   // Display the copyright message
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
