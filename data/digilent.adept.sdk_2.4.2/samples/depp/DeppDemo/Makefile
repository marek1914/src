# File: Makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DeppDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DeppDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldepp -ldmgr

all: $(TARGETS)

DeppDemo:
	$(CC) -o DeppDemo DeppDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

