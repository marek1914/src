# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DtwiDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DtwiDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldtwi -ldmgr

all: $(TARGETS)

DtwiDemo:
	$(CC) -o DtwiDemo DtwiDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

