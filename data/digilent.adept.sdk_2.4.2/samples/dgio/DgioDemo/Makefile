# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DgioDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DgioDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldgio -ldmgr

all: $(TARGETS)

DgioDemo:
	$(CC) -o DgioDemo DgioDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

