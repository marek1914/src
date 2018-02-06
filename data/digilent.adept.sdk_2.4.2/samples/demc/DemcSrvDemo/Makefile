# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DemcSrvDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DemcSrvDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldemc -ldmgr

all: $(TARGETS)

DemcSrvDemo:
	$(CC) -o DemcSrvDemo DemcSrvDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

