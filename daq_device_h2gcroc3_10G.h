#ifndef __DAQ_DEVICE_H2GCROC3_10G_H__
#define __DAQ_DEVICE_H2GCROC3_10G_H__


#include <daq_device_h2gcroc3.h>
#include <h2gcroc3TriggerHandler.h>
#include <iostream>

#include <arpa/inet.h>
//#include <netinet/in.h>

class daq_device_h2gcroc3_10G : public  daq_device_h2gcroc3 {

public:

  daq_device_h2gcroc3_10G (const int eventtype
		  , const int subeventid
		  , const char *ipaddr
		  , const int trigger =1
		  , const int nr_packets=20);

  ~daq_device_h2gcroc3_10G();


  void identify(std::ostream& os = std::cout) const;


  int init();
  int endrun();


protected:

  
  
};

#endif

