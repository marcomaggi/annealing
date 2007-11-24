# prepare.sh --

set -x

(cd ..
    if test ! -f config.h.in ; then
        autoheader
    fi)
(cd ..
    if test configure.ac -nt configure -o \
        ../configuration/configure.ds -nt configure -o \
        ../infrastructure/acmacros.m4 -nt configure
        then
        autoconf
        autoheader
    fi)

../configure --enable-static CFLAGS="-O3 -g -march=i686 -mtune=i686" "$@"

### end of file
