# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DemcStepDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DemcStepDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldemc -ldmgr

all: $(TARGETS)

DemcStepDemo:
	$(CC) -o DemcStepDemo DemcStepDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

