# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK DpioDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DpioDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldpio -ldmgr

all: $(TARGETS)

DpioDemo:
	$(CC) -o DpioDemo DpioDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

