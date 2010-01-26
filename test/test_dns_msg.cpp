//****************************************************************************
// Module name : test_dns_msg
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
// First written on 1/25/2010 by Steve deRosier.
//
// Module Description:
//   Unit test driver for dns message classes.
//
//****************************************************************************
//  $Id$
//****************************************************************************

//****************************************************************************
//  Include section
// Add all #includes here
//
//****************************************************************************
#include <iostream>

#include <sys/types.h>
#include <stdlib.h>

#include "../dns_msg.h"
#include "../dns_header.h"
#include "../dns_query.h"
#include "../dns_answer.h"

//****************************************************************************
//  Defines section
// Add all #defines or consts here
//
//****************************************************************************
using namespace std;

const uint8_t FULL_DNS_MSG[] = 
{
   // First is header
   0x4e, 0x7a, 0x81, 0x80, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
   
   // Query
   0x0a, 0x63, 0x61, 0x6c, 0x2d, 0x73, 0x69, 0x65, 0x72, 0x72, 0x61, 
   0x03, 0x63, 0x6f, 0x6d, 0x00,
   0x00, 0x01, 0x00, 0x01, 
   
   // Answer
   0x0a, 0x63, 0x61, 0x6c, 0x2d, 0x73, 0x69, 0x65, 0x72, 0x72, 0x61, 
   0x03, 0x63, 0x6f, 0x6d, 0x00,
   0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x1c, 0x20, 
   0x00, 0x04, 0x06, 0x06, 0x06, 0x06
};

const uint8_t DNS_HEADER[] = 
{
   0x4e, 0x7a, 0x81, 0x80, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
};

const uint8_t DNS_QUERY[] = 
{
   0x0a, 0x63, 0x61, 0x6c, 0x2d, 0x73, 0x69, 0x65, 0x72, 0x72, 0x61, 
   0x03, 0x63, 0x6f, 0x6d, 0x00,
   0x00, 0x01, 0x00, 0x01
};

const uint8_t DNS_NAME1[] = 
{
   0x0a, 0x63, 0x61, 0x6c, 0x2d, 0x73, 0x69, 0x65, 0x72, 0x72, 0x61, 
   0x03, 0x63, 0x6f, 0x6d, 0x00
};

const uint8_t DNS_NAME2[] = 
{
   0x04, 0x66, 0x72, 0x65, 0x64, 0x03, 0x63, 0x6f, 0x6d, 0x00
};

const uint8_t DNS_ANSWER[] = 
{
   0x0a, 0x63, 0x61, 0x6c, 0x2d, 0x73, 0x69, 0x65, 0x72, 0x72, 0x61, 
   0x03, 0x63, 0x6f, 0x6d, 0x00,
   0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x1c, 0x20, 
   0x00, 0x04, 0x06, 0x06, 0x06, 0x06
};


//****************************************************************************
//  Function Prototype Section
// Add prototypes for all functions called by this
// module, with the exception of runtime routines,
// or prototypes already included via header files.
//****************************************************************************
void test_header( void );
void test_query( void );
void test_answer( void );
void test_msg( void );
void DumpHex( void * Data, int Size );


//****************************************************************************
// main
//****************************************************************************
int main( int argc, char * argv[] )
{
   cout << "DNS Message Class Unit Tests" << endl;
   cout << "****************************************************************************" << endl;
   test_header();
   test_query();
   test_answer();
   test_msg();

   return EXIT_SUCCESS;
}

void test_header( void )
{
   uint8_t h_buff[12];
   int Size = 0;

   cout << "----------------------------------------------------------------------------" << endl; 
   cout << " Testing CDNS_Header" << endl;
   cout << "----------------------------------------------------------------------------" << endl; 
   
   ///////////////////////
   // Test 1
   ///////////////////////
   cout << "Test H1 - Construct empty header" << endl;
   const uint8_t DNS_EMPTY_HEADER[] = 
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   };

   CDNS_Header h1_header;
   Size = h1_header.Pack( h_buff );
   if( memcmp( h_buff, DNS_EMPTY_HEADER, Size ) == 0 )
   {  
      cout << "   empty header: ";
      DumpHex( h_buff, Size );
      cout << endl << "Test H1 passed" << endl;
   }
   else
   {  
      cout << "Test H1 failed" << endl;
   }

   ///////////////////////
   // Test 2
   ///////////////////////
   cout << "Test H2 - Fill with values" << endl;
   const uint8_t DNS_T2_HEADER[] = 
   {
     0x00, 0x53, 0x8f, 0x81, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05
   };
   h1_header.ID( 0x53);
   h1_header.Type( RESPONSE );
   h1_header.OpCode( 1 );
   h1_header.AA( true );
   h1_header.Trunc( true );
   h1_header.Recurse( true );
   h1_header.RecurseAvail( true );
   h1_header.Response( 1 );
   h1_header.QDCount( 2 );
   h1_header.ANCount( 3 );
   h1_header.NSCount( 4 );
   h1_header.ARCount( 5 );
   
   Size = h1_header.Pack( h_buff );
   if( memcmp( h_buff, DNS_T2_HEADER, Size ) == 0 )
   {  
      cout << "   filled header: ";
      DumpHex( h_buff, Size );
      cout << endl << "Test H2 passed" << endl;
   }
   else
   {
      cout << "Test H2 failed" << endl;
   }
   
   ///////////////////////
   // Test 3
   ///////////////////////
   cout << "Test H3 - Test accessors" << endl;
   int Test3Pass = 0;
   if(  0x53 != h1_header.ID() ) Test3Pass = 0x01;
   if( RESPONSE != h1_header.Type() ) Test3Pass |= 0x02;
   if( 1 != h1_header.OpCode() ) Test3Pass |= 0x04;
   if( true != h1_header.AA() ) Test3Pass |= 0x08;
   if( true != h1_header.Trunc() ) Test3Pass |= 0x10;
   if( true != h1_header.Recurse() ) Test3Pass |= 0x20;
   if( true != h1_header.RecurseAvail() ) Test3Pass |= 0x40;
   if( 1 != h1_header.Response() ) Test3Pass |= 0x80;
   if( 2 != h1_header.QDCount() ) Test3Pass |= 0x100;
   if( 3 != h1_header.ANCount() ) Test3Pass |= 0x200;
   if( 4 != h1_header.NSCount() ) Test3Pass |= 0x400;
   if( 5 != h1_header.ARCount() ) Test3Pass |= 0x800;
   
   if( 0 == Test3Pass )
   {  
      cout << "Test H3 passed" << endl;
   }
   else
   {
      cout << "Test H3 failed. Code: " << Test3Pass << endl;
   }

   ///////////////////////
   // Test 4
   ///////////////////////
   cout << "Test H4 - Unpack" << endl;
   Size = h1_header.Unpack((uint8_t*)DNS_HEADER);
   Size = h1_header.Pack( h_buff );
   if( memcmp( h_buff, DNS_HEADER, Size ) == 0 )
   {  
      cout << "   real header: ";
      DumpHex( h_buff, Size );
      cout << endl << "Test H4 passed" << endl;
   }  
   else
   {
      cout << "Test H4 failed" << endl;
   }
   
   ///////////////////////
   // Test 5
   ///////////////////////

   cout << "Test H5 - Construct from data" << endl;
   CDNS_Header h2_header((uint8_t*)DNS_HEADER);
   Size = h2_header.Pack( h_buff );
   if( memcmp( h_buff, DNS_HEADER, Size ) == 0 )
   {  
      cout << "   real header: ";
      DumpHex( h_buff, Size );
      cout << endl << "Test H5 passed" << endl;
   }  
   else
   {
      cout << "Test H5 failed" << endl;
   }

}

void test_query( void )
{
   cout << "----------------------------------------------------------------------------" << endl; 
   cout << " Testing CDNS_Query" << endl;
   cout << "----------------------------------------------------------------------------" << endl; 

   uint8_t q_buff[DNS_NAME_MAX+4];
   int Size = 0;

   ///////////////////////
   // Test 1
   ///////////////////////
   cout << "Test Q1 - Construct empty query" << endl;
   const uint8_t DNS_EMPTY_QUERY[] = 
   {
      0x00, 0x00, 0x00, 0x00, 0x00
   };

   CDNS_Query t1_query;
   Size = t1_query.Pack( q_buff );
   if( memcmp( q_buff, DNS_EMPTY_QUERY, Size ) == 0 )
   {  
      cout << "   empty query: ";
      DumpHex( q_buff, Size );
      cout << endl << "Test Q1 passed" << endl;
   }
   else
   {  
      cout << "Test Q1 failed" << endl;
   }

   ///////////////////////
   // Test 2
   ///////////////////////
   cout << "Test Q2 - Fill with values" << endl;
   t1_query.QType(DNS_TYPE_A);
   t1_query.QClass(DNS_CLASS_IN);
   t1_query.SetQName( (uint8_t*)DNS_NAME1 );
   
   Size = t1_query.Pack( q_buff );
   if( memcmp( q_buff, DNS_QUERY, Size ) == 0 )
   {  
      cout << "   filled header: ";
      DumpHex( q_buff, Size );
      cout << endl << "Test Q2 passed" << endl;
   }
   else
   {
      cout << "Test Q2 failed" << endl;
   }
   
   ///////////////////////
   // Test 3
   ///////////////////////
   cout << "Test Q3 - Test accessors" << endl;
   int Test3Pass = 0;
   if( DNS_TYPE_A != t1_query.QType() ) Test3Pass = 0x01;
   if( DNS_CLASS_IN != t1_query.QClass() ) Test3Pass |= 0x02;
   Size = t1_query.GetQName( q_buff );
   if( memcmp( q_buff, DNS_NAME1, Size ) != 0 )
   {  
      Test3Pass |= 0x04;
   }
   
   if( 0 == Test3Pass )
   {  
      cout << "Test Q3 passed" << endl;
   }
   else
   {
      cout << "Test Q3 failed. Code: " << Test3Pass << endl;
   }

   ///////////////////////
   // Test 4
   ///////////////////////
   cout << "Test Q4 - Unpack" << endl;
   Size = t1_query.Unpack((uint8_t*)DNS_QUERY);
   Size = t1_query.Pack( q_buff );
   if( memcmp( q_buff, DNS_QUERY, Size ) == 0 )
   {  
      cout << "   real query: ";
      DumpHex( q_buff, Size );
      cout << endl << "Test Q4 passed" << endl;
   }  
   else
   {
      cout << "Test Q4 failed" << endl;
   }
   
   ///////////////////////
   // Test 5
   ///////////////////////

   cout << "Test Q5 - Construct from data" << endl;
   CDNS_Query t2_query((uint8_t*)DNS_QUERY);
   Size = t2_query.Pack( q_buff );
   if( memcmp( q_buff, DNS_QUERY, Size ) == 0 )
   {  
      cout << "   real header: ";
      DumpHex( q_buff, Size );
      cout << endl << "Test Q5 passed" << endl;
   }  
   else
   {
      cout << "Test Q5 failed" << endl;
   }

}

void test_answer( void )
{
   cout << "----------------------------------------------------------------------------" << endl; 
   cout << " Testing CDNS_Answer" << endl;
   cout << "----------------------------------------------------------------------------" << endl; 

   uint8_t a_buff[DNS_NAME_MAX+40];
   int Size = 0;

   ///////////////////////
   // Test 1
   ///////////////////////
   cout << "Test A1 - Construct empty answer" << endl;
   const uint8_t DNS_EMPTY_ANSWER[] = 
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00,
      0x00
   };

   CDNS_Answer t1_answer;
   Size = t1_answer.Pack( a_buff );
   if( memcmp( a_buff, DNS_EMPTY_ANSWER, Size ) == 0 )
   {  
      cout << "   empty answer: ";
      DumpHex( a_buff, Size );
      cout << endl << "Test A1 passed" << endl;
   }
   else
   {  
      cout << "Test A1 failed" << endl;
   }

   ///////////////////////
   // Test 2
   ///////////////////////
   cout << "Test A2 - Fill with values" << endl;
   t1_answer.QType(DNS_TYPE_A);
   t1_answer.QClass(DNS_CLASS_IN);
   t1_answer.SetName( (uint8_t*)DNS_NAME1 );
   t1_answer.TTL( 0x00001c20 );
   t1_answer.RDLength( 4 );
   t1_answer.RData( 0x06060606 );
   
   Size = t1_answer.Pack( a_buff );
   if( memcmp( a_buff, DNS_ANSWER, Size ) == 0 )
   {  
      cout << "   filled answer: ";
      DumpHex( a_buff, Size );
      cout << endl << "Test A2 passed" << endl;
   }
   else
   {
      cout << "Test A2 failed" << endl;
   }
   
   ///////////////////////
   // Test 3
   ///////////////////////
   cout << "Test A3 - Test accessors" << endl;
   int Test3Pass = 0;
   if( DNS_TYPE_A != t1_answer.QType() ) Test3Pass = 0x01;
   if( DNS_CLASS_IN != t1_answer.QClass() ) Test3Pass |= 0x02;
   Size = t1_answer.GetName( a_buff );
   if( memcmp( a_buff, DNS_NAME1, Size ) != 0 )
   {  
      Test3Pass |= 0x04;
   }
   if( 0x00001c20 != t1_answer.TTL() ) Test3Pass = 0x08;
   if( 4 != t1_answer.RDLength() ) Test3Pass = 0x10;
   if( 0x06060606 != t1_answer.RData() ) Test3Pass = 0x20;
   
   if( 0 == Test3Pass )
   {  
      cout << "Test A3 passed" << endl;
   }
   else
   {
      cout << "Test A3 failed. Code: " << Test3Pass << endl;
   }

   ///////////////////////
   // Test 4
   ///////////////////////
   cout << "Test A4 - Unpack" << endl;
   Size = t1_answer.Unpack((uint8_t*)DNS_ANSWER);
   Size = t1_answer.Pack( a_buff );
   if( memcmp( a_buff, DNS_ANSWER, Size ) == 0 )
   {  
      cout << "   real answer: ";
      DumpHex( a_buff, Size );
      cout << endl << "Test A4 passed" << endl;
   }  
   else
   {
      cout << "Test A4 failed" << endl;
   }
   
}

void test_msg( void )
{
   cout << "----------------------------------------------------------------------------" << endl; 
   cout << " Testing CDNS_Msg" << endl;
   cout << "----------------------------------------------------------------------------" << endl; 
   
   uint8_t m_buff[2*DNS_NAME_MAX+80];
   int Size = 0;

   ///////////////////////
   // Test 1
   ///////////////////////
   cout << "Test M1 - Construct empty message" << endl;
   const uint8_t DNS_EMPTY_MESSAGE[] = 
   {
      // an empty message is only a header
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   };

   CDNS_Msg t1_msg;
   Size = t1_msg.Pack( m_buff );
   if( memcmp( m_buff, DNS_EMPTY_MESSAGE, Size ) == 0 )
   {  
      cout << "   empty message: ";
      DumpHex( m_buff, Size );
      cout << endl << "Test M1 passed" << endl;
   }
   else
   {  
      cout << "Test M1 failed" << endl;
   }

   ///////////////////////
   // Test 2
   ///////////////////////
   cout << "Test M2 - Fill with values" << endl;
   CDNS_Answer t1_answer;
   t1_answer.Unpack( (uint8_t *)DNS_ANSWER );
   CDNS_Query t1_query( (uint8_t *)DNS_QUERY );
   CDNS_Header t1_header( (uint8_t *)DNS_HEADER );
   
   t1_msg.Header( t1_header );
   t1_msg.Query( t1_query );
   t1_msg.Answer( t1_answer );
   
   Size = t1_msg.Pack( m_buff );
   if( memcmp( m_buff, FULL_DNS_MSG, Size ) == 0 )
   {  
      cout << "   filled message: ";
      DumpHex( m_buff, Size );
      cout << endl << "Test M2 passed" << endl;
   }
   else
   {
      cout << "Test M2 failed" << endl;
   }
   
   ///////////////////////
   // Test 3
   ///////////////////////
   cout << "Test M3 - Test accessors" << endl;
   int Test3Pass = 0;

   CDNS_Answer t3_answer = t1_msg.Answer();
   CDNS_Query t3_query = t1_msg.Query();
   CDNS_Header t3_header = t1_msg.Header(); 
   
   Size = t3_answer.Pack( m_buff );
   if( memcmp( m_buff, DNS_ANSWER, Size ) != 0 )
   {  
      Test3Pass |= 0x01;
   }
   Size = t3_query.Pack( m_buff );
   if( memcmp( m_buff, DNS_QUERY, Size ) != 0 )
   {  
      Test3Pass |= 0x02;
   }
   Size = t3_header.Pack( m_buff );
   if( memcmp( m_buff, DNS_HEADER, Size ) != 0 )
   {  
      Test3Pass |= 0x04;
   }
   
   if( 0 == Test3Pass )
   {  
      cout << "Test M3 passed" << endl;
   }
   else
   {
      cout << "Test M3 failed. Code: " << Test3Pass << endl;
   }

   ///////////////////////
   // Test 4
   ///////////////////////
   cout << "Test M4 - Unpack" << endl;
   t1_msg.Unpack((uint8_t*)FULL_DNS_MSG);
   Size = t1_msg.Pack( m_buff );
   if( memcmp( m_buff, FULL_DNS_MSG, Size ) == 0 )
   {  
      cout << "   real message: ";
      DumpHex( m_buff, Size );
      cout << endl << "Test M4 passed" << endl;
   }  
   else
   {
      cout << "Test M4 failed" << endl;
   }
   
   ///////////////////////
   // Test 5
   ///////////////////////
   cout << "Test Q5 - Construct from data" << endl;
   CDNS_Msg t2_msg((uint8_t*)FULL_DNS_MSG);
   Size = t2_msg.Pack( m_buff );
   if( memcmp( m_buff, FULL_DNS_MSG, Size ) == 0 )
   {  
      cout << "   real message: ";
      DumpHex( m_buff, Size );
      cout << endl << "Test M5 passed" << endl;
   }  
   else
   {
      cout << "Test M5 failed" << endl;
   }

}

//****************************************************************************
// Function name: void CUDPMessage::DumpHex()
//    returns: nothing
// Created by: Steve deRosier
// Date created: 1/22/2010
// Description:  Debugging dump of the message in hex mode.
//****************************************************************************
void DumpHex( void * Data, int Size )
{
   char oldfill = cout.fill();

   cout << "0x ";
   cout << std::hex;
   for( int i = 0; i < Size; i++ )
   {
      cout.fill('0');
      cout.width(2);
      // The mask below defeats the sign-extension on the int cast
      cout << (0x000000FF & (unsigned int)(((uint8_t *)Data)[i]));
      cout << " ";
   }

   cout << dec;
   cout.fill(oldfill);  
}

