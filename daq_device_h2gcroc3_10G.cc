#include "daq_device_h2gcroc3_10G.h"


#include <unistd.h>
#include <string.h>

#define CONTROLPORT 11000
#define PORT 11001

#define NumberOfASIC  2
#define ASIC_SELECT  3



#define MAX_PACKETS 9

using namespace std;

daq_device_h2gcroc3_10G::daq_device_h2gcroc3_10G(const int eventtype
			       , const int subeventid
			       , const char *ipaddr
			       , const int trigger
			       , const int nr_packets)
  :daq_device_h2gcroc3 ( eventtype, subeventid,ipaddr,trigger,nr_packets)
{

  _hitformat = 302;

}

daq_device_h2gcroc3_10G::~daq_device_h2gcroc3_10G()
{

}




void daq_device_h2gcroc3_10G::identify(std::ostream& os) const
{
  
  os  << "H2GCROC3 10G/s Device  Event Type: " << m_eventType << " Subevent id: " << m_subeventid 
      << " IP: " << _ip;
  os << " nr_packets: " << _nr_packets;
  if ( _th) os << " trigger ";
  if ( _broken) os << " ** not functional ** ";
  os << endl;

}

int  daq_device_h2gcroc3_10G::init()
{

  if (! _sockfd)
    {
      if (inet_aton(_ip, &_rocip)==0)
	{
	  cerr << "invalid IP address " << _ip << endl;
	  return 1;
	}
      
      
      if ( ( _sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) ==-1)
	{
	  perror("socket " );
	  return -2;
	}
      
      _slen=sizeof(_server);
      memset ( (char *) &_server, 0, _slen);
      _server.sin_family  = AF_INET;
      _server.sin_addr = _rocip;
      _server.sin_port = htons(CONTROLPORT);
    }

  unsigned char Start[]   ={0x0,0x0,0x0,0x0,0x0,0x0, 0xa0,0x00,0x09,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  
  return sendData( Start, sizeof(Start));


}
int  daq_device_h2gcroc3_10G::endrun()
{
  int ret=0;
  
  unsigned char Stop[]   ={0x0,0x0,0x0,0x0,0x0,0x0, 0xa0,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  ret =  sendData( Stop, sizeof(Stop));

  close (_sockfd);
  _sockfd = 0;

  
  return 0;
}



