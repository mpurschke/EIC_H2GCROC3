
#include "h2gcroc3TriggerHandler.h"
#include <iostream>

using namespace std;


int h2gcroc3TriggerHandler::wait_for_trigger( const int moreinfo)
{

  //cout << __LINE__ << "  " << __FILE__ << " waiting for data, fd =  " << _highest_fd  << endl;
  struct timeval tv;

  tv.tv_sec = 2;
  tv.tv_usec = 0;

  FD_ZERO(_fdset);
  FD_SET(_highest_fd , _fdset);

	 
  int retval = select(_highest_fd+1, _fdset, NULL, NULL, &tv);

  if ( retval >0) 
    {
      //cout << __LINE__ << "  " << __FILE__ << " trigger for evt type " << _etype  << endl;
      return _etype;
    }
  //cout << __LINE__ << "  " << __FILE__ << " trigger timeout "   << endl;
  return 0;
}
