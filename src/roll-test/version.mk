NAME       = sdsc-mpi-roll-test
VERSION    = 3
RELEASE    = 0
PKGROOT    = /root/rolltests

KERNEL         := $(shell uname -r)

RPM.EXTRAS = AutoReq:No
RPM.FILES  = $(PKGROOT)/mpi.t
