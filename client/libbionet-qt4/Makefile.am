
# Copyright (c) 2008-2010, Regents of the University of Colorado.
# This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
# NNC07CB47C.


include $(top_srcdir)/common.am

include $(top_srcdir)/build-aux/autotroll.mk

lib_LTLIBRARIES = libbionet-qt4.la

libbionet_qt4_la_DEPENDENCIES = $(INCLUDES_BIONET_CLIENT) $(INCLUDES_BIONET_UTIL) $(INCLUDES_BIONET_ASN1) $(INCLUDES_BIONET_CAL_CLIENT)

libbionet_qt4_la_SOURCES = io.cxx bionetio.cxx bionetmodel.cxx io.moc.cpp bionetio.moc.cpp bionetmodel.moc.cpp icons.qrc.cpp

BUILT_SOURCES = io.moc.cpp bionetmodel.moc.cpp bionetio.moc.cpp icons.qrc.cpp

libbionet_qt4_la_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) $(GLIB_CFLAGS) $(CFLAGS_BIONET_CLIENT) $(CFLAGS_BIONET_UTIL)

libbionet_qt4_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) $(GLIB_CFLAGS) $(CFLAGS_BIONET_CLIENT) $(CFLAGS_BIONET_UTIL)

libbionet_qt4_la_LDFLAGS = $(AM_LDFLAGS) $(QT_LDFLAGS) $(AM_LDFLAGS) $(AVAHI_COMPAT_LIBDNS_SD_LIBS) $(LDFLAGS_BIONET_CLIENT) $(LDFLAGS_BIONET_ASN1) $(LDFLAGS_BIONET_UTIL) $(LDFLAGS_BIONET_CAL_CLIENT) -version-info $(BIONET_INTERFACE_VERSION) 

libbionet_qt4_la_LIBADD = $(QT_LIBS) $(AM_LDADD) -lpthread -lm $(LIBS_BIONET_CLIENT) $(LIBS_BIONET_UTIL) $(LIBS_BIONET_ASN1) $(LIBS_BIONET_CAL_CLIENT)

clean-local:
	rm -f $(BUILT_SOURCES)
