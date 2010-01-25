#
# Project name: dns-spoof
# Makefile
#
# Copyright (C) 2010 Steve deRosier.
# All rights reserved.
#
#  The information contained herein is confidential
# property of Company. The use, copying, transfer or
# disclosure of such information is prohibited except
# by express written agreement with Cal-Sierra Communications.
#
# First written on 1/25/2010 by Steve deRosier.
#
##############################################################################
#  $Id$
##############################################################################

#
# INCLUDE DIRECTORIES AND OPERATING SYSTEM LIBRARY
#
INCLUDES = #-I and then include paths
TARGET1  = test_dns_msg
TARGET2  = test_socket
LDFLAGS =
LIBS =

#
# COMPILER/ASSEMBLER INVOCATIONS
#
# Define RELEASE=1 on the command line to get 
# We redefine CC to ensure gcc is used as 'cc' is the make default
ifdef RELEASE
  CFLAGS += -Wall -O3 $(INCLUDES)
  CXXFLAGS += -Wall -O3 $(INCLUDES)
else
  define DEBUGTXT
    @printf "\n#\n# Define RELEASE=1 on the command line to compile release version.\n"
    @printf "# Assuming debug compile. \n#\n"
  endef
  CFLAGS += -ggdb -fno-inline -Wall -DDEBUG_BUILD $(INCLUDES)
  CXXFLAGS += -ggdb -fno-inline -Wall -DDEBUG_BUILD $(INCLUDES)
endif

CC = gcc
CXX = g++

#
# Source and object files
#
SOURCES := $(wildcard *.cpp *.c)
OBJECTS1 := test_dns_msg.o
OBJECTS2 := test_socket.o
DEPENDS := $(addsuffix .d,$(basename $(SOURCES)))

#
# GENERATION RULES
#
# These automatically create all of your object files
# and depencency files as long as you have your OBJECTS
# setup right

%.o: %.cpp
	$(COMPILE.cpp) -MMD -MP -o $@ $<

%.o: %.c
	$(COMPILE.c) -MMD -MP -o $@ $<

#%.o : %.s:
#	$(asm) $<

.DONE:


#
# TARGETS
#

$(TARGET1) : $(OBJECTS1)
	$(CXX) $(LDFLAGS) -o $(TARGET1) $(OBJECTS1) $(LIBS)

$(TARGET2) : $(OBJECTS2)
	$(CXX) $(LDFLAGS) -o $(TARGET2) $(OBJECTS2) $(LIBS)

.PHONY: all
all : $(TARGET1) $(TARGET2)

.PHONY: clean
clean :
	-rm -f $(OBJECTS1)
	-rm -f $(OBJECTS2)
	-rm -f $(DEPENDS)
	-rm -f $(TARGET1)
	-rm -f $(TARGET2)

.PHONY: cleanall
cleanall: clean

# help 
.PHONY: help
help:
	@make --print-data-base --question | \
      awk '/^[^.%][-A-Za-z0-9_]*:/ \
      { print substr($$1, 1, length($$1)-1) }' | \
      sort | \
      pr --omit-pagination --width=80 --columns=4
	
# dependency generation
ifneq "$(MAKECMDGOALS)" "clean"
  -include $(addsuffix .d,$(basename $(SOURCES)))
endif
