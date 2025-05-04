#ifndef __H2GCROC3_PLUGIN_H__
#define __H2GCROC3_PLUGIN_H__

#include <rcdaq_plugin.h>

#include <daq_device_h2gcroc3.h>
#include <iostream>


class h2gcroc3_plugin : public RCDAQPlugin {

 public:
  int  create_device(deviceblock *db);

  void identify(std::ostream& os = std::cout, const int flag=0) const;

};


#endif
