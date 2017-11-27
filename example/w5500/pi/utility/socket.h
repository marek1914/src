#ifndef	_SOCKET_H_
#define	_SOCKET_H_

#include "utility/w5500.h"
// Opens a socket(TCP or UDP or IP_RAW mode)
extern uint8_t socket(SOCKET s, uint8_t protocol, uint16_t port, uint8_t flag); 
extern void close(SOCKET s);
// Establish TCP connection (Active)
extern uint8_t connect(SOCKET s, uint8_t * addr, uint16_t port); 
extern void disconnect(SOCKET s); 
// Establish TCP connection (Passive)
extern uint8_t listen(SOCKET s);
// Send data (TCP)
extern uint16_t send(SOCKET s, const uint8_t * buf, uint16_t len); 
// Receive data (TCP)
extern int16_t recv(SOCKET s, uint8_t * buf, int16_t len);	
extern uint16_t peek(SOCKET s, uint8_t *buf);
// Send data (UDP/IP RAW)
extern uint16_t sendto(SOCKET s, const uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port); 
// Receive data (UDP/IP RAW)
extern uint16_t recvfrom(SOCKET s, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port); 
// Wait for transmission to complete
extern void flush(SOCKET s); 

extern uint16_t igmpsend(SOCKET s, const uint8_t * buf, uint16_t len);

// To allow buffered UDP send (i.e. where the UDP datagram is built up over a
// number of calls before being sent
/* This function sets up a UDP datagram, the data for which will be provided by one
  or more calls to bufferData and then finally sent with sendUDP.
  return 1 if successful, or 0 if  error
*/
extern int startUDP(SOCKET s, uint8_t* addr, uint16_t port);
/*
  copies up to len bytes of data from buf into a UDP datagram to be
  sent later by sendUDP.  Allows datagrams to be built up from a series of bufferData calls.
  return Number of bytes successfully buffered
*/
uint16_t bufferData(SOCKET s, uint16_t offset, const uint8_t* buf, uint16_t len);
/*
  Send a UDP datagram built up from a sequence of startUDP followed by one or more
  calls to bufferData.
  return 1 if the datagram was successfully sent, or 0 if there was an error
*/
int sendUDP(SOCKET s);

#endif
