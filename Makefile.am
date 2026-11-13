AUTOMAKE_OPTIONS = foreign


#allsources = \
#	daq_device_h2gcroc3.cc  h2gcroc3_plugin.cc  h2gcroc3TriggerHandler.cc 

AM_LDFLAGS = -L$(libdir)
AM_CXXFLAGS = -O2 -g -I$(includedir) -I$(ONLINE_MAIN)/include $(RPC_INCLUDE)
AM_CPPFLAGS = -O2 -g -I$(includedir) -I$(ONLINE_MAIN)/include $(RPC_INCLUDE)

INCLUDES = -I$(includedir) -I$(ONLINE_MAIN)/include 

bin_SCRIPTS = RequestScript.py

lib_LTLIBRARIES =  librcdaqplugin_h2gcroc3.la 

librcdaqplugin_h2gcroc3_la_SOURCES = \
	daq_device_h2gcroc3.cc daq_device_h2gcroc3_10G.cc daq_device_caloroc.cc  h2gcroc3_plugin.cc  h2gcroc3TriggerHandler.cc


librcdaqplugin_h2gcroc3_la_LIBADD = -lrcdaqutils $(RPC_LDD)



clean-local:
	rm -f $(BUILT_SOURCES)
