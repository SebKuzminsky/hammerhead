#!/bin/sh

./configure --enable-static --disable-shared -build=i386 --host=none-uclinux-uclibc CC='ucfront microblaze-uclinux-gcc' LDFLAGS='-Wl, -elf2flt' GLIB20_CFLAGS=-I$PETALINUX/software/petalinux-dist/glib/include/glib-2.0 GLIB20_LIBS=$PETALINUX/software/petalinux-dist/glib/lib/libglib-2.0.la DBUS_CFLAGS="-I$PETALINUX/software/petalinux-dist/dbus/include/dbus-1.0 -I$PETALINUX/software/petalinux-dist/dbus/lib/dbus-1.0/include" DBUS_LIBS=$PETALINUX/software/petalinux-dist/dbus/lib/libdbus-1.la AVAHI_COMPAT_LIBDNS_SD_CFLAGS=-I$PETALINUX/software/petalinux-dist/avahi/include/avahi-compat-libdns_sd AVAHI_COMPAT_LIBDNS_SD_LIBS=$PETALINUX/software/petalinux-dist/avahi/lib/libdns_sd.la AVAHI_GLIB_CFLAGS=-I$PETALINUX/software/petalinux-dist/avahi/include AVAHI_GLIB_LIBS="$PETALINUX/software/petalinux-dist/avahi/lib/libavahi-glib.la $PETALINUX/software/petalinux-dist/avahi/lib/libavahi-core.la $PETALINUX/software/petalinux-dist/avahi/lib/libavahi-common.la $PETALINUX/software/petalinux-dist/avahi/lib/libavahi-client.la $PETALINUX/software/petalinux-dist/avahi/lib/libavahi-gobject.la" LIBS=$PETALINUX/software/petalinux-dist/dbus/lib/libdbus-1.a

