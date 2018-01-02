#!/usr/bin/python
 
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
 
SSDP_ADDR = '239.255.255.250'
SSDP_PORT = 1900
  
class Client(DatagramProtocol):
    def __init__(self):
        ssdp = reactor.listenMulticast(SSDP_PORT, self, listenMultiple=True)
        ssdp.joinGroup(SSDP_ADDR)

    def datagramReceived(self, datagram, address):
        first_line = datagram.rsplit('\r\n')[0]
        print "Received %s from %r" % (first_line, address)
 
def main():
    obj = Client()
 
reactor.callWhenRunning(main)
reactor.run()
