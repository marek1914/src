# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK EnumDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = EnumDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldmgr

all: $(TARGETS)

EnumDemo:
	$(CC) -o EnumDemo EnumDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

