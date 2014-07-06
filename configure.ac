AC_INIT([libtins benchmark], 0.1)
AC_PROG_CXX
AC_LANG(C++)

libraries=""
old_LIBS="$LIBS"

CXXFLAGS="$CXXFLAGS -std=c++11"
LIBTINS_CXXFLAGS="$CXXFLAGS"
LIBCRAFTER_CXXFLAGS="$CXXFLAGS"
LIBPCAP_CXXFLAGS="$CXXFLAGS"

LIBTINS_LIBS="-ltins"
LIBPCAP_LIBS="-lpcap"
LIBCRAFTER_LIBS="-lcrafter -lpcap"

EXECUTION_LIB_PATH=""

AC_ARG_ENABLE(
    scapy, 
    [  --disable-scapy          disable scapy benchmark], 
    [],
    [ libraries="$libraries scapy" ]
)
AC_ARG_ENABLE(
    impacket, 
    [  --disable-impacket       disable impacket benchmark], 
    [],
    [ libraries="$libraries impacket" ]
)
AC_ARG_ENABLE(
    dpkt, 
    [  --disable-dpkt           disable dpkt benchmark], 
    [],
    [ libraries="$libraries dpkt" ]
)
AC_ARG_ENABLE(
    libtins, 
    [  --disable-libtins        disable libtins benchmark], 
    [],
    [ 
        libraries="$libraries libtins" 
        AC_ARG_WITH(
            [libtins-include-path],
            [AS_HELP_STRING([--with-libtins-include-path],
            [location of the libtins headers])],
            [LIBTINS_CXXFLAGS="-I$withval"]
        )
        AC_ARG_WITH(
            [libtins-library-path],
            [AS_HELP_STRING([--with-libtins-library-path],
            [location of the libtins library files])],
            [
                LIBTINS_LIBS="$LIBTINS_LIBS -L$withval"
                EXECUTION_LIB_PATH="$withval:$EXECUTION_LIB_PATH"
            ]
        )
        AC_CHECK_HEADERS(
            [tins/dns.h], 
            [], 
            [AC_MSG_ERROR([libtins headers are missing])]        
        )
        LIBS="-ltins"
        echo -n "Checking for libtins library... "
        AC_LINK_IFELSE(
            [AC_LANG_PROGRAM([#include <tins/dns.h>], 
            [Tins::DNS dummy])],
            [echo yes],
            [AC_MSG_ERROR([libtins library is not installed!])]
        )
    ]
)
AC_ARG_ENABLE(
    libpcap, 
    [  --disable-libpcap        disable libpcap benchmark], 
    [],
    [ 
        libraries="$libraries libpcap" 
        AC_ARG_WITH(
            [libpcap-include-path],
            [AS_HELP_STRING([--with-libpcap-include-path],
            [location of the libpcap headers])],
            [LIBPCAP_CXXFLAGS="-I$withval"]
        )
        AC_ARG_WITH(
            [libpcap-library-path],
            [AS_HELP_STRING([--with-libpcap-library-path],
            [location of the libpcap library files])],
            [
                LIBPCAP_LIBS="-L$withval"
                LIBCRAFTER_LIBS="-L$withval $LIBCRAFTER_LIBS"
                EXECUTION_LIB_PATH="$withval:$EXECUTION_LIB_PATH"
            ]
        )
        AC_CHECK_HEADERS(
            [pcap/pcap.h], 
            [], 
            [AC_MSG_ERROR([libpcap headers are missing])]        
        )
        AC_CHECK_LIB(
            pcap, 
            pcap_loop, 
            [], 
            [AC_MSG_ERROR([pcap library is missing!])]
        )
    ]
)
AC_ARG_ENABLE(
    libcrafter, 
    [  --disable-libcrafter     disable libcrafter benchmark], 
    [],
    [ 
        libraries="$libraries libcrafter" 
        AC_ARG_WITH(
            [libcrafter-include-path],
            [AS_HELP_STRING([--with-libcrafter-include-path],
            [location of the libcrafter headers])],
            [LIBCRAFTER_CXXFLAGS="-I$withval"]
        )
        AC_ARG_WITH(
            [libcrafter-library-path],
            [AS_HELP_STRING([--with-libcrafter-library-path],
            [location of the libcrafter library files])],
            [
                LIBCRAFTER_LIBS="-L$withval"
                EXECUTION_LIB_PATH="$withval:$EXECUTION_LIB_PATH"
            ]
        )
        AC_CHECK_HEADERS(
            [crafter.h], 
            [], 
            [AC_MSG_ERROR([libcrafter headers are missing])]        
        )
        echo -n "Checking for libcrafter library... "
        LIBS="-lcrafter"
        AC_LINK_IFELSE(
            [AC_LANG_PROGRAM([#include <crafter.h>], 
            [Crafter::Packet dummy])],
            [echo yes],
            [AC_MSG_ERROR([librafter library is not installed!])]
        )
    ]
)

LIBS="$old_LIBS"

AC_SUBST(LIBTINS_CXXFLAGS)
AC_SUBST(LIBCRAFTER_CXXFLAGS)
AC_SUBST(LIBPCAP_CXXFLAGS)

AC_SUBST(LIBTINS_LIBS)
AC_SUBST(LIBCRAFTER_LIBS)
AC_SUBST(LIBPCAP_LIBS)

AC_SUBST(libraries)
AC_SUBST(LIBS)
AC_OUTPUT(Makefile)
