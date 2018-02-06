# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DstmDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DstmDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldstm -ldmgr

all: $(TARGETS)

DstmDemo:
	$(CC) -o DstmDemo DstmDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

