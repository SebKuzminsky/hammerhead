 ./configure -C --cache-file=mb.cache --enable-static --disable-shared --prefix=$PETALINUX/software/petalinux-dist/dbus --build=i386 --host=none-uclinux-uclibc CC='ucfront microblaze-uclinux-gcc' LDFLAGS='-Wl, -elf2flt' --disable-selinux --disable-inotify --disable-dnotify --disable-doxygen-docs --disable-abstract-sockets --with-x=no --with-xml=expat XML_CFLAGS=-I$PETALINUX/software/petalinux-dist/expat/include XML_LIBS=$PETALINUX/software/petalinux-dist/expat/lib/libexpat.a LIBXML_CFLAGS=-I$PETALINUX/software/petalinux-dist/expat/include LIBXML_LIBS=$PETALINUX/software/petalinux-dist/expat/lib/libexpat.a CFLAGS="-I$PETALINUX/software/petalinux-dist/expat/include -I$PETALINUX/software/linux-2.6.x-petalogix/include" LIBS=$PETALINUX/software/petalinux-dist/expat/lib/libexpat.a