# prepare.sh --

set -x

(cd ..
    if test ! -f config.h.in ; then
        autoheader
    fi)
(cd ..
    if test configure.ac -nt configure ; then
        autoconf
        autoheader
    fi)

../configure "$@"

### end of file
