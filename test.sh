#!/bin/bash

#
# test.sh
# by: Steve deRosier
# 1/25/2010
#
# Simple validation test to see if dns-spoof functions.
#

dnssport=9797

exval=1
./dns-spoof -p $dnssport $> /dev/null
if [ $? -ne 0 ]
then
   echo "dns-spoof failed to start"
   echo "dns-spoof check failed"
   exit 1
fi

if dig @localhost -p $dnssport textureandlight.com | grep -q -e "textureandlight.com.[[:space:]]*7200[[:space:]]IN[[:space:]]A[[:space:]]6.6.6.6" 
then
   echo "dns-spoof check passed"
   exval=0
else
   echo "dns-spoof check failed"
fi
killall dns-spoof  

exit $exval
