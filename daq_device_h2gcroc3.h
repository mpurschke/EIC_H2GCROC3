#ifndef __DAQ_DEVICE_H2GCROC3_H__
#define __DAQ_DEVICE_H2GCROC3_H__


#include <daq_device.h>
#include <h2gcroc3TriggerHandler.h>
#include <iostream>

#include <arpa/inet.h>
//#include <netinet/in.h>

class daq_device_h2gcroc3 : public  daq_device {

public:

  daq_device_h2gcroc3 (const int eventtype
		  , const int subeventid
		  , const char *ipaddr
		  , const int trigger =1
		  , const int nr_packets=20);

  ~daq_device_h2gcroc3();


  void identify(std::ostream& os = std::cout) const;

  int max_length(const int etype) const;

  // functions to do the work

  int put_data(const int etype, int * adr, const int length);

  int init();
  int endrun();

  int rearm( const int etype);

protected:

  int sendData(const unsigned char *data, const long unsigned int len);

  int _hitformat;
  int _my_port;
  
  subevtdata_ptr sevt;
  unsigned int number_of_words;

  char _ip[512];
  int _s;
  struct sockaddr_in _si_me;
  int _broken;
  int _nr_packets;

  struct sockaddr_in _server;
  int _sockfd;
  in_addr _rocip;

  socklen_t _slen;

  
  h2gcroc3TriggerHandler *_th;
  fd_set read_flag;

};

#endif

