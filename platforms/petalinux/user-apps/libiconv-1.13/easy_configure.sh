 ./configure -C --cache-file=mb.cache --enable-static --disable-shared --prefix=$PETALINUX/software/petalinux-dist/libiconv --build=i386 --host=none-uclinux-uclibc CC='ucfront microblaze-uclinux-gcc' LDFLAGS='-Wl, -elf2flt' 