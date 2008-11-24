######################################################################
# Automatically generated by qmake (2.01a) Wed Nov 19 14:09:13 2008
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../../util ../libbionet /usr/include/qwt-qt4 ../libbionet ../../util

QT += network
CONFIG += link_pkgconfig debug
PKGCONFIG += glib-2.0

LIBS += -L../lqwt-qt4 \
        ../libbionet/.libs/libbionet.a \
        ../../asn.1/.libs/libbionet-asn.a \
        ../../util/.libs/libbionet-util.a \
        ../../cal/mdnssd-bip/client/.libs/libcal-mdnssd-bip-client.a \
        ../../cal/mdnssd-bip/shared/.libs/libcal-mdnssd-bip-shared.a \
        ../../cal/util/.libs/libevent.a \
        -lqwt-qt4 -lpthread -lm -ldns_sd

# Input
HEADERS += bionetio.h \
           bionetmodel.h \
           mainwindow.h \
           plotwindow.h \
           resourceview.h \
           samplehistory.h
SOURCES += bionetio.cxx \
           bionetmodel.cxx \
           main.cxx \
           mainwindow.cxx \
           plotwindow.cxx \
           resourceview.cxx \
           samplehistory.cxx
