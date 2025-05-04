#include "h2gcroc3_plugin.h"
#include <daq_device_h2gcroc3.h>

#include <strings.h>

int h2gcroc3_plugin::create_device(deviceblock *db)
{

  //std::cout << __LINE__ << "  " << __FILE__ << "  " << db->argv0 << std::endl;

  if ( strcasecmp(db->argv0,"device_h2gcroc3") == 0 ) 
    {
      // we need at least 3 params
      if ( db->npar <4 ) return 1; // indicate wrong params
      
      int eventtype  = atoi ( db->argv1); // event type
      int subid = atoi ( db->argv2); // subevent id


      if ( db->npar ==4 ) 
	{
	  daq_device *x = new daq_device_h2gcroc3( eventtype,
					      subid,
					      db->argv3 );
	  //      x->identify();
	  
	  add_readoutdevice (x);
	  return 0;  // say "we handled this request" 
	}  

      else if ( db->npar ==5 ) 
	{
	  daq_device *x = new daq_device_h2gcroc3( eventtype,
					      subid,
					      db->argv3,
					      atoi (db->argv4) );
	  //      x->identify();
	  
	  add_readoutdevice (x);
	  return 0;  // say "we handled this request" 
	}  

      else if ( db->npar ==6 ) 
	{
	  daq_device *x = new daq_device_h2gcroc3( eventtype,
						   subid,
						   db->argv3,
						   atoi (db->argv4),
						   atoi (db->argv5));
	  //      x->identify();
	  
	  add_readoutdevice (x);
	  return 0;  // say "we handled this request" 
	}  


    }
  
  return -1; // say " this is not our device"
}

void h2gcroc3_plugin::identify(std::ostream& os, const int flag) const
{

  if ( flag <=2 )
    {
      os << " - H2GCROC3 Plugin" << std::endl;
    }
  else
    {
      os << " - H2GCROC3 Plugin, provides - " << std::endl;
      os << " -     device_h2gcroc3 (evttype, subid, IP addr, trigger, nr_packets) - readout an H2GCROC3 " << std::endl;

    }
      
}

h2gcroc3_plugin _h2gcroc3;

