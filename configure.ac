AC_INIT(configure.ac, 1.0)

AC_CANONICAL_HOST


AC_PROG_CXX(cxx CC g++)


AM_INIT_AUTOMAKE (GL1_readout,1.00)

AC_PROG_INSTALL

AC_PROG_LIBTOOL

AC_CHECK_HEADER([rpc/rpc.h], [ ],
                [AC_SUBST(RPC_INCLUDE, "-I/usr/include/tirpc")] [AC_SUBST(RPC_LDD, "-ltirpc")])



CXXFLAGS=" -g -O2"


AC_OUTPUT(Makefile)

