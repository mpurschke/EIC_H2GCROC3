AUTOMAKE_OPTIONS = foreign


allsources = \
	daq_device_h2gcroc3.cc  h2gcroc3_plugin.cc  h2gcroc3TriggerHandler.cc

AM_LDFLAGS = -L$(libdir)
AM_CXXFLAGS = -O2 -g -I$(includedir) -I$(ONLINE_MAIN)/include $(RPC_INCLUDE)
AM_CPPFLAGS = -O2 -g -I$(includedir) -I$(ONLINE_MAIN)/include $(RPC_INCLUDE)

INCLUDES = -I$(includedir) -I$(ONLINE_MAIN)/include 

#bin_SCRIPTS = getSpinPattern.py getBuckets.py getFillNumber.py getPolarimeter.py

#bin_PROGRAMS = udp_to_GL1
#udp_to_GL1_SOURCES = udp_to_GL1.cc

lib_LTLIBRARIES =  librcdaqplugin_h2gcroc3.la 

librcdaqplugin_h2gcroc3_la_SOURCES = \
	daq_device_h2gcroc3.cc  h2gcroc3_plugin.cc  h2gcroc3TriggerHandler.cc


librcdaqplugin_h2gcroc3_la_LIBADD = -lrcdaqutils $(RPC_LDD)



clean-local:
	rm -f $(BUILT_SOURCES)
