# File: makefile
# Author: MTA
# Company: Digilent Inc.
# Date: 12/29/2015
# Description: makefile for Adept SDK DptiDemo

CC = gcc
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib/digilent/adept
TARGETS = DptiDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldpti -ldmgr

all: $(TARGETS)

DptiDemo:
	$(CC) -o DptiDemo DptiDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

