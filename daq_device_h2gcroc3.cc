#include "daq_device_h2gcroc3.h"


#include <unistd.h>
#include <string.h>

#define CONTROLPORT 11000
#define PORT 11001

#define NumberOfASIC  2
#define ASIC_SELECT  3



#define MAX_PACKETS 9

using namespace std;

daq_device_h2gcroc3::daq_device_h2gcroc3(const int eventtype
			       , const int subeventid
			       , const char *ipaddr
			       , const int trigger
			       , const int nr_packets)
{

  m_eventType  = eventtype;
  m_subeventid = subeventid;
  strcpy ( _ip, ipaddr);
  _s = 0;
  _sockfd = 0;
  _th = 0;


  _broken = 0;
  _nr_packets = nr_packets;

  if ((_s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
      _broken =1;
      cout << __LINE__ << "  " << __FILE__ << " broken 1" << endl;
      perror("h2gcroc3");
    }

  memset((char *) &_si_me, 0, sizeof(_si_me));
  _si_me.sin_family = AF_INET;
  _si_me.sin_port = htons(PORT);
  _si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(_s, (sockaddr *)&_si_me, sizeof(_si_me))==-1)
   {
      _broken =1;
      cout << __LINE__ << "  " << __FILE__ << " broken 1" << endl;
      perror("bind");

    }

  FD_ZERO(&read_flag);
  FD_SET(_s, &read_flag);


  if ( trigger) 
    {
      //cout << __LINE__ << "  " << __FILE__ << " registering triggerhandler with fd " << _s << endl;
      _th  = new h2gcroc3TriggerHandler ( &read_flag, _s, m_eventType);
      registerTriggerHandler ( _th);
    }

  _hitformat = 301;

}

daq_device_h2gcroc3::~daq_device_h2gcroc3()
{
  if ( _th) 
    {
      clearTriggerHandler();
      delete _th;
      _th = 0;
    }
  close( _s );
}



// the put_data function

int daq_device_h2gcroc3::put_data(const int etype, int * adr, const int length )
{
  //  cout << __LINE__ << "  " << __FILE__ << " etype= " << etype << " adr= " << adr << " length= " << length << endl;

  if ( _broken ) 
    {
      cout << __LINE__ << "  " << __FILE__ << " broken ";
      identify();
      return 0; //  we had a catastrophic failure
    }

  int len = 0;

  if (etype != m_eventType )  // not our id
    {
      return 0;
    }

  sevt =  (subevtdata_ptr) adr;
  // set the initial subevent length
  sevt->sub_length =  SEVTHEADERLENGTH;

  // update id's etc
  sevt->sub_id =  m_subeventid;
  sevt->sub_type=4;
  sevt->sub_decoding = _hitformat; //301;
  sevt->reserved[0] = 0;
  sevt->reserved[1] = 0;

  if ( length <  max_length(etype) ) 
    {
      cout << __LINE__ << "  " << __FILE__ << " length exceeds limit " << length <<  " limit is " << max_length(etype) << endl;
      return 0;
    }

  //  unsigned int data;

  unsigned int *data = (unsigned int *) &sevt->data; 

  struct timeval tv;

  struct sockaddr_in si_src;
  socklen_t src_len;
  int ia;
  int received_len; 



  for ( ia = 0; ia < _nr_packets; ia++)  //receive 
    {

      tv.tv_sec = 1;
      tv.tv_usec = 0;
      int retval = select(_s+1, &read_flag, NULL, NULL, &tv);
      if ( retval > 0) 
      	{
	  *data++ = 0xAABBCCDD;
	  unsigned int *xlen = data; // that's where the later length goes 
	  *data++ = 0;
      
	  char *d = (char *) data;

	  src_len = sizeof (si_src);
	  if ( ( received_len = recvfrom(_s, d, 8192, 0, (sockaddr *) &si_src, &src_len) ) ==-1)
	    {
	      perror ( "receive ");
	      cout << __LINE__ << "  " << __FILE__ << " error " << received_len <<endl;
	      return 0;
	    }
	  *xlen = received_len;
	  //cout << __FILE__ << " " << __LINE__ << " got " << received_len << " bytes xlen ptr = " << xlen << endl;
	  
	  len += received_len + 8 ;   // we are still counting bytes here, but we add the 2 words in bytes
	  d+= received_len;     // and here - update the write pointer
	  data = (unsigned int *) d; // and update the data pointer
	}
      else
	{
	  //cout << __LINE__ << "  " << __FILE__ << " " << retval << " timeout in ";
	  //identify();
	  break;
	}

    }

  len /= 4;  // now we have ints
  
  sevt->sub_padding = len%2;
  len = len + (len%2);
  sevt->sub_length += len;
  //cout << __LINE__ << "  " << __FILE__ << " returning "  << sevt->sub_length << endl;
  return  sevt->sub_length;
  
}


void daq_device_h2gcroc3::identify(std::ostream& os) const
{
  
  os  << "H2GCROC3 Device  Event Type: " << m_eventType << " Subevent id: " << m_subeventid 
      << " IP: " << _ip;
  os << " nr_packets: " << _nr_packets;
  if ( _th) os << " trigger ";
  if ( _broken) os << " ** not functional ** ";
  os << endl;

}

int daq_device_h2gcroc3::max_length(const int etype) const
{
  if (etype != m_eventType) return 0;
  return  (MAX_PACKETS*1500 + SEVTHEADERLENGTH);
}

int  daq_device_h2gcroc3::init()
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

  unsigned char Start[]   ={0xa0,0x00,0x09,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  
  return sendData( Start, sizeof(Start));



}
int  daq_device_h2gcroc3::endrun()
{
  int ret=0;
  
  unsigned char Stop[]   ={0xa0,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  ret =  sendData( Stop, sizeof(Stop));

  close (_sockfd);
  _sockfd = 0;

  
  return 0;
}

// the rearm() function
int  daq_device_h2gcroc3::rearm(const int etype)
{
  if (etype != m_eventType) return 0;
  return 0;
}

int  daq_device_h2gcroc3::sendData(const unsigned char *data, const long unsigned int len)
{
  if (sendto(_sockfd, data, len , 0, (sockaddr *) &_server, _slen) == -1 )
	{
	  perror("sendto  " );
	  return -2;
	}
  return 0;
}


