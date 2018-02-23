# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DemcBrdcDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DemcBrdcDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldemc -ldmgr

all: $(TARGETS)

DemcBrdcDemo:
	$(CC) -o DemcBrdcDemo DemcBrdcDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

