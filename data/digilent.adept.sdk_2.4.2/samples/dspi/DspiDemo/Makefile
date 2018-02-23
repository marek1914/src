# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DspiDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DspiDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldspi -ldmgr

all: $(TARGETS)

DspiDemo:
	$(CC) -o DspiDemo DspiDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

