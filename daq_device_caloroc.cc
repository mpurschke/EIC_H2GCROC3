#include "daq_device_caloroc.h"


#include <unistd.h>
#include <string.h>
#include <sys/time.h>

// the CALOROC board is controlled through IPbus (UDP port 50001)
#define IPBUSPORT 50001

// writing this value to register STREAMREGISTER starts the data stream, 0 stops it
#define STREAMREGISTER 0x9000
#define STREAMON 0xf1



#define MAX_PACKETS 9

using namespace std;

daq_device_caloroc::daq_device_caloroc(const int eventtype
			       , const int subeventid
			       , const char *ipaddr
			       , const int trigger
			       , const int nr_packets)
  :daq_device_h2gcroc3 ( eventtype, subeventid,ipaddr,trigger,nr_packets)
{

  _hitformat = 310;
  _my_port=50100;  

}

daq_device_caloroc::~daq_device_caloroc()
{

}




void daq_device_caloroc::identify(std::ostream& os) const
{
  
  os  << "CALOROC Device  Event Type: " << m_eventType << " Subevent id: " << m_subeventid 
      << " IP: " << _ip;
  os << " nr_packets: " << _nr_packets;
  if ( _th) os << " trigger ";
  if ( _broken) os << " ** not functional ** ";
  os << endl;

}

int  daq_device_caloroc::init()
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

      // don't wait forever for the board's reply
      struct timeval tv;
      tv.tv_sec = 1;
      tv.tv_usec = 0;
      setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
      
      _slen=sizeof(_server);
      memset ( (char *) &_server, 0, _slen);
      _server.sin_family  = AF_INET;
      _server.sin_addr = _rocip;
      _server.sin_port = htons(IPBUSPORT);
    }

  return writeRegister(STREAMREGISTER, STREAMON);

}

int  daq_device_caloroc::endrun()
{
  int ret = writeRegister(STREAMREGISTER, 0);

  close (_sockfd);
  _sockfd = 0;

  return ret;
}

// an IPbus 2.0 write of one register; the board answers with 8 bytes,
// and the low 4 bits of byte 4 are 0 if the write succeeded 
int  daq_device_caloroc::writeRegister(const unsigned int address, const unsigned int value)
{
  unsigned char Request[] =
    {
      0xf0, 0x00, 0x00, 0x20,  // packet header: version 2, packet id 0, little-endian
      0x1f, 0x01, 0x00, 0x20,  // transaction header: write 1 word, request
      (unsigned char) (address & 0xff),   (unsigned char) ((address >> 8) & 0xff),
      (unsigned char) ((address >> 16) & 0xff), (unsigned char) ((address >> 24) & 0xff),
      (unsigned char) (value & 0xff),     (unsigned char) ((value >> 8) & 0xff),
      (unsigned char) ((value >> 16) & 0xff),   (unsigned char) ((value >> 24) & 0xff)
    };

  int ret = sendData( Request, sizeof(Request));
  if ( ret) return ret;

  unsigned char Reply[64];
  int n = recv(_sockfd, Reply, sizeof(Reply), 0);
  if ( n < 0)
    {
      perror("CALOROC no reply from the board " );
      return -3;
    }
  if ( n < 8 || (Reply[4] & 0xf) != 0 )
    {
      cerr << "CALOROC register write to 0x" << hex << address << dec << " failed" << endl;
      return -4;
    }
  return 0;
}
