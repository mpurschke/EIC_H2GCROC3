#ifndef __H2GCROC3TRIGGERHANDLER_H__
#define __H2GCROC3TRIGGERHANDLER_H__

#include <sys/select.h>
#include "TriggerHandler.h"


class h2gcroc3TriggerHandler : public TriggerHandler {

public:

  h2gcroc3TriggerHandler(fd_set *theset, const int highest_fd, const int etype =1)
    {
      _highest_fd = highest_fd;
      _fdset = theset;
      _etype = etype;

    }

  ~h2gcroc3TriggerHandler() {};

  //  virtual void identify(std::ostream& os = std::cout) const = 0;

  // this is the virtual worker routine
  int wait_for_trigger( const int moreinfo=0);

 protected:
  
  int _highest_fd;
  fd_set *_fdset;
  int _etype;

};

#endif
