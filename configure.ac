
# Copyright (c) 2008-2010, Regents of the University of Colorado.
# This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
# NNC07CB47C.


# Process this file with autoconf to produce a configure script.

# Prelude.
AC_PREREQ([2.59])
AC_INIT([Bionet], [2.6], [bionet@bioserve.colorado.edu])
# unique source file --- primitive safety check 
AC_CONFIG_SRCDIR([cal/cal-server.h])

# place to put some extra build scripts installed
AC_CONFIG_AUX_DIR([build-aux])

# local m4 scripts
AC_CONFIG_MACRO_DIR([build-aux/m4])

#
# Needed to support cross-compiling
AC_CANONICAL_SYSTEM

#AC_CANONICAL_TARGET

# fairly severe build strictness
# change foreign to gnu or gnits to comply with gnu standards
AM_INIT_AUTOMAKE([-Wall -Werror foreign 1.9.2])



# Checks for programs.
# We don't want to use the default AC_PROG_* flags set by these macros,
# but we do want to allow the user to specify custom flags.
# Save the user's settings, and override the default set by AC_PROG_*
USERCFLAGS="$CFLAGS"
USERCPPFLAGS="$CPPFLAGS"
USERCXXFLAGS="$CXXFLAGS"
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_YACC
AM_PROG_LEX

if test "`which $YACC`x" = "x"; then
    AC_MSG_ERROR([some version of yacc is required to build], [1])
fi

CFLAGS="$USERCFLAGS"
CPPFLAGS="$USERCPPFLAGS"
CXXFLAGS="$USERCXXFLAGS"

AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_CHECK_TOOL([STRIP],[strip])

AC_SUBST(docdir)

AM_CONDITIONAL(CC_CLANG, test "x$CC" = "xclang")

AC_ARG_VAR([PKG_CONFIG_LIBDIR], [Replacement search path for pkg-config. Set when cross-compiling])
# $cross_compiling is set automatically by this point, 
if test "x$cross_compiling" = "xyes"; then
    AM_CONDITIONAL(CROSS_COMPILING, test "x" = "x")
    if test "x$PKG_CONFIG_LIBDIR" = "x"; then
        AC_MSG_FAILURE(["If cross compiling, you MUST set PKG_CONFIG_LIBDIR to the directory containing .pc files for $host_alias"])
    fi
else
    AM_CONDITIONAL(CROSS_COMPILING, test "x" = "y")
fi


# Checks for libraries.
AC_ARG_ENABLE(check, AC_HELP_STRING([--disable-check],
      [disable check unit testing]),
      [ENABLE_CHECK=$enableval],
      [ENABLE_CHECK=yes])
if test "x$ENABLE_CHECK" = "xyes"; then
PKG_CHECK_MODULES([CHECK], [ check >= 0.9.3 ])
AC_SUBST(CHECK_CFLAGS)
AC_SUBST(CHECK_LIBS)
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_CHECKING(["Checking OS"])
case "$host_os" in
     linux*)
         AC_MSG_RESULT([Linux])
         HAVE_LINUX=yes
         HAVE_DARWIN=no
         HAVE_WIN32=no
         ALSA_ABLE=yes
	 SYSAPI=posix
     ;;
     darwin*)
         AC_MSG_RESULT([Darwin/OSX])
         HAVE_DARWIN=yes
         HAVE_LINUX=no
         HAVE_WIN32=no
         ALSA_ABLE=no
	 SYSAPI=posix
     ;;

     *wince* | *mingwce* | *mingw32ce* | *pe*)
         AC_MSG_RESULT([Windows CE])
         HAVE_DARWIN=no
         HAVE_LINUX=no
         HAVE_WIN32=yes
         ALSA_ABLE=no
	 SYSAPI=wince
	 #AC_DEFINE([_WIN32_WINNT], 0x0501, [Define to '0x0501' for Windows 2000 APIs.])
     ;;

     mingw32*)
         AC_MSG_RESULT([Windows (mingw32)])
         HAVE_DARWIN=no
         HAVE_LINUX=no
         HAVE_WIN32=yes
         ALSA_ABLE=no
	 SYSAPI=win32
	 AC_SUBST([_WIN32_WINNT], 0x0501, [Define to '0x0501' for Windows 2000 APIs.])
	 CPPFLAGS+="-D_WIN32_WINNT=0x0501"
     ;;
     *)
         AC_MSG_FAILURE(["System architecture '$host_os' is not supported"])
     ;;
esac



AC_SUBST([SYSAPI])


case "$SYSAPI" in
  win32)
    AC_CHECK_HEADER([winsock2.h], [], [AC_MSG_FAILURE(["Must have winsock2.h for windows"])])
    AC_CHECK_LIB([ws2_32], [main], [], [AC_MSG_FAILURE(["Must have ws2_32 for windows"])])
    ;;


  wince)
    AC_CHECK_HEADER([winsock2.h], [], [AC_MSG_FAILURE(["Must have winsock2.h for windows"])])
    AC_CHECK_LIB([ws2], [WSAGetLastError], [], [AC_MSG_FAILURE(["Must have ws2 lib for windows"])])
    ;;
esac


# User Selected features template
#AC_ARG_ENABLE([featureX],
#            [AS_HELP_STRING([--enable-featureX],
#              [Enable featureX @<:@default=yes@:>@])],
#            [],
#            [enable_featureX=yes])
#          
#  ENABLE_FEATUREX=
#  AS_IF([test "x$enable_featureX" != xno],
#      [AC_SUBST([ENABLE_FEATUREX], [1])
#       AC_DEFINE([ENABLE_FEATUREX], [1],
#                 [Define if you have want featureX support])
#    ])

if test "x$GLIB_CFLAGS" = "x" && test "x$GLIB_LIBS" = "x"; then
    AM_PATH_GLIB_2_0([2.12.0], 
        [], [], [gthread])
fi
if test "x$GLIB_CFLAGS" = "x" && test "x$GLIB_LIBS" = "x"; then
    AC_MSG_ERROR([GLIB required to build], [1])
fi
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(OPENSSL, [ openssl >= 0.9.8 ])
AC_SUBST(OPENSSL_CFLAGS)
AC_SUBST(OPENSSL_LIBS)

AC_ARG_WITH(
    [emdnssd],
    AC_HELP_STRING([--with-emdnssd], [Use embedded mdnssd library @<:@default=no@:>@]),
        [with_emdnssd=$withval],
        [with_emdnssd=no]
    )

if test "x$with_emdnssd" = "x" || test "x$with_emdnssd" = "xno"; then
    if test "$cross_compiling" != "yes"; then
        if test "x$HAVE_LINUX" = "xyes"; then
            PKG_CHECK_MODULES(AVAHI_COMPAT_LIBDNS_SD, [ avahi-compat-libdns_sd >= 0.6.16 ])
            AC_DEFINE([HAVE_AVAHI], [1], ["Define if AVAHI is being used"])
            #AC_DEFINE([HAVE_EMBEDDED_MDNSSD], [0], ["Define if embedded mdns is being used"])
        else 
            if test "x$SYSAPI" != "xposix"; then
                # Look for bonjour library elsewhere
                AC_MSG_ERROR([unable to find emdnssd library])
            fi
        fi
    fi
elif test "x$with_emdnssd" = "xyes" ; then
    AVAHI_COMPAT_LIBDNS_SD_LIBS="-lembeddeddns_sd"
    AC_DEFINE([HAVE_EMBEDDED_MDNSSD], [1], ["Define if embedded mdns is being used"])
else
    AVAHI_COMPAT_LIBDNS_SD_CFLAGS="-I$with_emdnssd/mDNSPosix/ -I$with_emdnssd/mDNSShared/ -I$with_emdnssd/mDNSCore/"
    AVAHI_COMPAT_LIBDNS_SD_LIBS="-L$with_emdnssd/mDNSPosix/build/prod -lembeddeddns_sd"
    AC_DEFINE([HAVE_EMBEDDED_MDNSSD], [1], ["Define if embedded mdns is being used"])
fi
AC_SUBST(AVAHI_COMPAT_LIBDNS_SD_CFLAGS)
AC_SUBST(AVAHI_COMPAT_LIBDNS_SD_LIBS)

AM_CONDITIONAL(ENABLE_DARWIN, test "x$HAVE_DARWIN" = "xyes")
AM_CONDITIONAL(ENABLE_LINUX, test "x$HAVE_LINUX" = "xyes")
AM_CONDITIONAL(ENABLE_WIN32, test "x$HAVE_WIN32" = "xyes")


AC_ARG_ENABLE(gcov,
	      AS_HELP_STRING([--enable-gcov],
                             [enable test coverage with gcov @<:@default=no@:>@]),
              [case "${enableval}" in
                 yes) gcov=true ;;
                 no)  gcov=false ;;
                 *)   AC_MSG_ERROR([bad value ${enableval} for --enable-gcov]) ;;
               esac],
              [gcov=false])

AM_CONDITIONAL(GCOV, test x"$gcov" = "xtrue")

AC_ARG_ENABLE(gprof,
	      AS_HELP_STRING([--enable-gprof],
                             [enable profiling with gprof @<:@default=no@:>@]),
              [case "${enableval}" in
                 yes) gprof=true ;;
                 no)  gprof=false ;;
                 *)   AC_MSG_ERROR([bad value ${enableval} for --enable-gprof]) ;;
               esac],
              [gprof=false])

AM_CONDITIONAL(GPROF, test x"$gprof" = "xtrue")


###
#
# see if the user wants ALSA stuff, and if so if it's available, only on Linux
#
###

if test "x$ALSA_ABLE" = "xyes"; then
    AC_ARG_ENABLE(
        alsa,
        AC_HELP_STRING([--disable-alsa], [disable ALSA dependent parts]),
        [ENABLE_ALSA=$enableval],
        [ENABLE_ALSA=yes]
    )
else
    ENABLE_ALSA=no
fi

if test "x$ENABLE_ALSA" = "xyes" ; then
    PKG_CHECK_MODULES(ALSA, [ alsa >= 1.0.16 ])
    AC_SUBST(ALSA_CFLAGS)
    AC_SUBST(ALSA_LIBS)
fi

AM_CONDITIONAL(ENABLE_ALSA, test "x$ENABLE_ALSA" = "xyes")

# disable all executables
AC_ARG_ENABLE(exe, AC_HELP_STRING([--disable-exe],
      [disable building of executables]),
      [ENABLE_EXE=$enableval],
      [ENABLE_EXE=yes])
AM_CONDITIONAL(ENABLE_EXE, test "x$ENABLE_EXE" = "xyes")

# disable HABs
AC_ARG_ENABLE(
    habs,
    AC_HELP_STRING([--disable-habs], [disable Bionet HABs]),
        [ENABLE_HABS=$enableval],
        [ENABLE_HABS=yes]
    )
AM_CONDITIONAL(ENABLE_HABS, test "x$ENABLE_HABS" = "xyes")

# disable Clients
AC_ARG_ENABLE(
    clients,
    AC_HELP_STRING([--disable-clients], [disable Bionet Clients]),
        [ENABLE_CLIENTS=$enableval],
        [ENABLE_CLIENTS=yes]
    )
AM_CONDITIONAL(ENABLE_CLIENTS, test "x$ENABLE_CLIENTS" = "xyes")


###
# 
# make sure there is sqlite3 (required for BDM)
#
###
AC_ARG_ENABLE(
    bdm,
    AC_HELP_STRING([--disable-bdm], [disable Bionet Data Manager]),
        [ENABLE_BDM=$enableval],
        [ENABLE_BDM=yes]
    )
if test "x$ENABLE_BDM" = "xyes"; then
    AC_CHECK_LIB(
        sqlite3,
        sqlite3_libversion_number,
        [ HAVE_SQLITE3=yes ],
        [ AC_MSG_ERROR([could not find libsqlite3-dev]) ],
        [ ]
    )


fi
AM_CONDITIONAL(ENABLE_BDM, test "x$ENABLE_BDM" = "xyes")




# 
# look for different versions of ION
#

AC_ARG_WITH(
    [ion],
    AC_HELP_STRING([--with-ion], [Use ION for BP @<:@default=check@:>@]),
        [with_ion=$withval],
        [with_ion=check]
    )

# Search for ION libs/Headers, and modify CFLAGS/LDFLAGS if needed 
#(These all add to the list of libs to link into future tests)
# Set ION_LIBS if ion enabled

if test "x$with_ion" != "xno"; then
    if test "$cross_compiling" != yes; then
        AX_EXT_HAVE_HEADER(
            [ion.h], 
            [/usr/include/ion /usr/local/include /opt/include],
            [ have_ion_h=yes ],
            [
                if test "x$with_ion" != xcheck; then
                    AC_MSG_FAILURE([--with-ion was given, but test for ion failed])
                fi
                with_ion=no
            ]
        ) 
    fi
fi


#
# If we have found ion.h, next we make sure we have *some* version of libbp.so.
#

if test "x$with_ion" != "xno"; then

    # this first check looks for ION NASA Baseline 1.1.0
    AX_EXT_HAVE_LIB(
        [/usr/lib /usr/local/lib /opt/lib], 
        [bp],
        [bp_send], 
        [
            have_libbp=inb-1.1.0
            AC_MSG_RESULT([Found ION INB-1.1.0])
            AC_DEFINE([ENABLE_ION], 1, [Define if you have BP support via ION])
            AC_DEFINE([ION_VERSION], [INB-1.1.0], [Select ION version, INB-1.1.0 or BST])
            AC_SUBST([ENABLE_ION], 1)
            AC_SUBST([LIBS_ION], ["-lbp -lbpP -lici"])
        ],
        [ have_libbp=no ],
        [ -lbpP -lici ]
    )

    if test "x$have_libbp" == "xno"; then
        # didnt find inb-1.1.0, look for the bioserve version of ion instead
        AX_EXT_HAVE_LIB(
            [/usr/lib /usr/local/lib /opt/lib], 
            [bp],
            [bp_detach], 
            [
                HAVE_BP_DETACH=yes
                AC_DEFINE([ENABLE_ION], 1, [Define if you have BP support via ION])
                AC_DEFINE([ION_VERSION], [BST], [Select ION version, INB-1.1.0 or BST])
                AC_SUBST([ENABLE_ION], 1)
                AC_SUBST([LIBS_ION], ["-lbp -lici"])
            ],
            [ 
                if test "x$with_ion" != xcheck; then
                    AC_MSG_FAILURE([--with-ion was given, but test for ion failed])
                fi
                with_ion=no
            ],
            [ -lici ]
        )

        # We leave this check here, mainly so the check tests
        # know what to expect
        if test "x$HAVE_BP_DETACH" != "x"; then
            AC_CHECK_FUNCS([bp_add_endpoint sm_set_basekey])
            if test "x$ac_cv_func_bp_add_endpoint" != "x"; then
                AC_MSG_RESULT([Found ION with Bioserve mods])
            fi
        fi
    fi
fi

if test "x$with_ion" == "xno"; then
    AC_MSG_RESULT([No ION found, disabling BDM sync over BP])
fi

AM_CONDITIONAL(ENABLE_ION, [test "x$with_ion" != "xno"])


# optionally compile the mmod hab
#
AC_ARG_ENABLE(
    mmod,
    AC_HELP_STRING([--disable-mmod], [disable MMOD HAB]),
        [ENABLE_MMOD=$enableval],
        [ENABLE_MMOD=yes]
    )
AM_CONDITIONAL(ENABLE_MMOD, test "x$ENABLE_MMOD" = "xyes")

# optionally compile the env hab
#
AC_ARG_ENABLE(
    env,
    AC_HELP_STRING([--disable-env], [disable Environmental Mote HAB]),
        [ENABLE_ENV=$enableval],
        [ENABLE_ENV=yes]
    )
AM_CONDITIONAL(ENABLE_ENV, test "x$ENABLE_ENV" = "xyes")


#
# optionally compile the parsec hab
#
AC_ARG_ENABLE(
    parsec,
    AC_HELP_STRING([--disable-parsec], [disable Parsec HAB]),
        [ENABLE_PARSEC=$enableval],
        [ENABLE_PARSEC=yes]
    )
AM_CONDITIONAL(ENABLE_PARSEC, test "x$ENABLE_PARSEC" = "xyes")


#
# optionally compile the uwb hab
#
AC_ARG_ENABLE(
    uwb,
    AC_HELP_STRING([--disable-uwb], [disable UWB HAB]),
        [ENABLE_UWB=$enableval],
        [ENABLE_UWB=yes]
    )
AM_CONDITIONAL(ENABLE_UWB, test "x$ENABLE_UWB" = "xyes")


#
# Check for isfinite() in <math.h>. isfinite() is a c99 standard,
# but finite() is the older BSD standard is isfinite is not availble
AC_CHECK_DECL(isfinite, 
  [AC_DEFINE([HAVE_ISFINITE],[1],[<math.h> provides isfinite])], 
  , 
  [[#include <math.h>]])



AC_ARG_ENABLE(python, AC_HELP_STRING([--disable-python],
      [disable python dependent parts]),
      [ENABLE_PYTHON=$enableval],
      [ENABLE_PYTHON=yes])
if test "x$ENABLE_PYTHON" = "xyes" ; then
   AM_PATH_PYTHON([2.5])
   AC_PROG_SWIG
   SWIG_PYTHON
fi
AM_CONDITIONAL(ENABLE_PYTHON, test "x$ENABLE_PYTHON" = "xyes")

# QT and qt libs
AC_ARG_ENABLE(qt4, AC_HELP_STRING([--disable-qt4],
      [disable QT4 dependent parts]),
      [ENABLE_QT4=$enableval],
      [ENABLE_QT4=yes])

# The following is more reliable, but requires autoconf >= 2.62
#    AC_CACHE_CHECK([for qmake for QT4], [ac_cv_path_QMAKE],
#        [AC_PATH_PROGS_FEATURE_CHECK([QMAKE], [qmake-qt4 qmake-4.4 qmake qmake-mac],
#            [[$ac_path_QMAKE -version | grep -iq 'qt version 4' && ac_cv_path_QMAKE=$ac_path_QMAKE ac_path_QMAKE_found=:]],
#            [AC_MSG_ERROR([could not find qmake for QT4 ])])])
if test "x$ENABLE_QT4" = "xyes" ; then
    AT_WITH_QT([svg])

    qt_ok=yes
    AC_PATH_PROGS([QMAKE], 
        [qmake-qt4 qmake-4.4 qmake-mac qmake],
        [not-found])
    if test "x$QMAKE" = "xnot-found" ; then
        AC_MSG_WARN([could not find qmake for QT4 ])
        qt_ok=no
    else
        AC_SUBST([QMAKE], [$ac_cv_path_QMAKE])

        AC_FIND_QWT(5.0.0,4,
            [qwt-qt4 qwt], 
            [/usr/include/qwt5 /usr/include/qwt-qt4 /opt/local/include /opt/local/include/qwt],
            [/opt/local/lib],
            [qt_ok=no])
    fi

    if test "x$qt_ok" = "xno"; then
        if test "x$HAVE_LINUX" = "xyes"; then
            AC_MSG_ERROR([QT or QWT could not be found. See previous errors.])
        fi
        ENABLE_QT4=no
    fi

fi

AM_CONDITIONAL(ENABLE_QT4, [test "x$ENABLE_QT4" = "xyes"])


if test "x$HAVE_DARWIN" = "xyes"; then
    echo "checking for LTKC... disabled"
    AM_CONDITIONAL(ENABLE_LTKC, test "x$HAVE_DARWIN" = "false")
else
    # FIXME(lenny) LTKC does not compile due to bad initializers and what not.
    # Turned off until someone makes it work.
    echo "checking for LTKC... yes"
    AM_CONDITIONAL(ENABLE_LTKC, test "x$HAVE_LINUX" = "false")
fi

if test "x$TOSROOT" = "x"; then
echo "checking for TinyOS... no"
else
echo "checking for TinyOS... yes"
fi
AM_CONDITIONAL([TINYOSMIG_DISABLED], [test "x$TOSROOT" = "x"])

#doxygen
AC_PATH_PROG(DOT, dot, no)
AC_ARG_ENABLE(doxygen, AC_HELP_STRING([--disable-doxygen],
      [disable doxygen documentation generation]),
      [ENABLE_DOXYGEN=$enableval],
      [ENABLE_DOXYGEN=yes])
if test "x$ENABLE_DOXYGEN" = "xyes" ; then
   AC_PATH_PROG(DOXYGEN, doxygen, no)
   if test "x$DOT" = "xno"; then
      AC_MSG_WARN([Bionet Doxygen requires dot, disabling doxygen])
      AM_CONDITIONAL([ENABLE_DOXYGEN], [test "x" = "y"])
   else
      AM_CONDITIONAL([ENABLE_DOXYGEN], [test "x$DOXYGEN" != "xno"])
   fi
else
   AM_CONDITIONAL([ENABLE_DOXYGEN], [test "x" = "y"])
fi


#ccache
AC_ARG_ENABLE(ccache, AC_HELP_STRING([--disable-ccache],
      [disable ccash compilation speedup]),
      [ENABLE_CCACHE=$enableval],
      [ENABLE_CCACHE=check])
CCACHE=no
if test "x$ENABLE_CCACHE" != "xno" ; then
  AC_PATH_PROG(CCACHE, ccache, no)
  if test "x$CCACHE" != "xno"; then
     CC="ccache $CC"
     CXX="ccache $CXX"
  else
    if test "x$ENABLE_CCACHE" = "xyes"; then
      AC_MSG_FAILURE([--enable-ccache was given, but test for ccache failed])
    fi
  fi
fi
AM_CONDITIONAL([ENABLE_CCACHE], [test "x$CCACHE" != "xno"])

#bazaar
AC_PATH_PROG(BAZAAR, bzr, no)
AM_CONDITIONAL([DISABLE_BZR], [test "x$BAZAAR" = "xno"])
if test "x$BAZAAR" = "xno"; then
   test "$BIONET_BUILD_TAG" == "" && BIONET_BUILD_TAG="+$USER"
else
   test "$BIONET_BUILD_TAG" == "" && BIONET_BUILD_TAG="+`bzr revno`+$USER"
fi
AC_ARG_VAR([BIONET_BUILD_TAG], [Bionet build ID])

#bluetooth
AC_ARG_ENABLE(bluetooth, AC_HELP_STRING([--disable-bluetooth],
      [disable bluetooth dependent parts]),
      [ENABLE_BLUETOOTH=$enableval],
      [ENABLE_BLUETOOTH=yes])
if test "x$HAVE_LINUX" = "xyes"; then
    if test "x$ENABLE_BLUETOOTH" = "xyes"; then
        PKG_CHECK_MODULES(BLUEZ, [ bluez >= 0.22 ])
        AC_SUBST(BLUEZ_CFLAGS)
        AC_SUBST(BLUEZ_LIBS)
        AM_CONDITIONAL([ENABLE_BLUETOOTH], [test "x$ENABLE_BLUETOOTH" = "xyes"])	
    else
        AM_CONDITIONAL([ENABLE_BLUETOOTH], [test "x$ENABLE_BLUETOOTH" = "xyes"])
    fi
else
    AM_CONDITIONAL([ENABLE_BLUETOOTH], [test "x$ENABLE_BLUETOOTH" = "false"])
fi

#xsltproc
AM_CONDITIONAL([HAVE_XSLTPROC], [test "x" == "y"])
AC_ARG_ENABLE(xslt, AC_HELP_STRING([--disable-xslt],
      [disable xslt dependent parts]),
      [ENABLE_XSLT=$enableval],
      [ENABLE_XSLT=yes])
if test "x$ENABLE_XSLT" = "xyes"; then
   AC_PATH_PROG(XSLTPROC, xsltproc, no)
   AM_CONDITIONAL([HAVE_XSLTPROC], [test "x$XSLTPROC" != "xno"])
fi

#bison & flex
AM_CONDITIONAL([ENABLE_BISON], [test "x$ac_cv_prog_YACC" != "x"])
AM_CONDITIONAL([ENABLE_FLEX], [test "x$ac_cv_prog_LEX" != "x"])

# GWT UIs depend on Java at compile-time.
AC_PATH_PROG([JAVA], [java], [no], [])
AM_CONDITIONAL([ENABLE_JAVA], [test "x$JAVA" != "xno"])

# Google Web Toolkit is required for compiling the webstats app from Java
# into JavaScript.  Both the GWT jars and java (not javac) are required.
# GWT doesn't have a normal system path, so the user has to pass a path.
AC_ARG_WITH([gwt], AC_HELP_STRING([--with-gwt],
    [Path to Google Web Toolkit to build web UIs @<:@default=none@:>@]),
    [if test "x$JAVA" == "xno"; then
		AC_MSG_ERROR([--with-gwt specified but java could not be found], [1])
	 fi
	 AC_SUBST([GWT_PATH], [$withval])],
    [with_gwt=none])
AM_CONDITIONAL([ENABLE_GWT], [test "x$with_gwt" != "xnone"])

# Checks for library functions.
if test "$cross_compiling" != yes; then
AC_FUNC_MALLOC
fi

AC_ARG_VAR([REBUILD_ASN], [Rebuild ASN automatically])
AM_CONDITIONAL([REBUILD_ASN], [test "x$REBUILD_ASN" != "x"])

# Output files
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile
                 cal/Makefile
		 cal/util/Makefile
		 cal/mdnssd-bip/Makefile
		 cal/mdnssd-bip/shared/Makefile
		 cal/mdnssd-bip/client/Makefile
		 cal/mdnssd-bip/server/Makefile
		 cal/examples/Makefile
                 asn.1/Makefile
                 util/Makefile
		 util/version.c
		 util/python/Makefile
                 libhab/Makefile
                 libbionet/Makefile
                 libbdm/Makefile
                 client/Makefile
		 client/commander/Makefile
		 client/get/Makefile
                 client/libbionet-qt4/Makefile
		 client/monitor/Makefile
		 client/plot/Makefile
                 client/py-libbionet/Makefile
                 client/realtime-watcher/Makefile
                 client/stream/Makefile
		 client/stream-manager/Makefile
		 client/twisted/Makefile
                 client/watcher/Makefile
                 hab/Makefile
		 hab/py-libhab/Makefile
		 hab/libamedevice/Makefile
                 hab/csa-cp/Makefile
                 hab/cpod/Makefile
		 hab/environmental-mote/Makefile
		 hab/mmod/Makefile
                 hab/info/Makefile
                 hab/random/Makefile
                 hab/test-pattern/Makefile
                 hab/alsa/Makefile
                 hab/parsec/Makefile
		 hab/stethoscope/Makefile
                 hab/syshealth/Makefile
                 hab/speedway/Makefile
                 hab/pal-650/Makefile
                 hab/uwb/Makefile
                 hab/streamy/Makefile
                 hab/bpstats2/Makefile
                 hab/mon/Makefile
                 hab/commandable/Makefile
                 data-manager/Makefile
                 data-manager/server/Makefile
                 data-manager/client/Makefile
                 data-manager/client/py-libbdm/Makefile
                 data-manager/client/bdm-client/Makefile
                 data-manager/client/bdm-watcher/Makefile
                 data-manager/client/bdm-watcher-py/Makefile
                 data-manager/client/bdmplot2/Makefile
                 data-manager/client/bp-commander/Makefile
                 data-manager/client/twisted/Makefile
                 data-manager/client/bdmplot/Makefile
		 data-manager/client/bdm-get/Makefile
                 bps/Makefile
                 bps/utils/Makefile
                 bps/check/Makefile
		 security/Makefile
		 security/ca/Makefile
		 apps/Makefile
		 apps/webstats/Makefile
		 test/config.sh
		 test/Makefile
		 test/src/Makefile
		 test/src/bonjour-py/Makefile
		 check/Makefile
		 check/libutil/Makefile
		 check/data-manager/Makefile
		 check/asn1/Makefile
		 check/cal/Makefile
                 ])

AC_OUTPUT

#
# vim: sw=2 sta expandtab
#
