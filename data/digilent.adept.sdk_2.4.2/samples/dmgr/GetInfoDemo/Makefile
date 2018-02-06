# File: makefile
# Author: Aaron Odell
# Company: Digilent Inc.
# Date: 8/16/2010
# Description: makefile for Adept SDK GetInfoDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = GetInfoDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldmgr

all: $(TARGETS)

GetInfoDemo:
	$(CC) -o GetInfoDemo GetInfoDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

