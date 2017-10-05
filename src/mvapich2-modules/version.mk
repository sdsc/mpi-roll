ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

PACKAGE     = mvapich2
CATEGORY    = mpi

NAME        = sdsc-$(PACKAGE)-modules_$(ROLLNETWORK)
RELEASE     = 5
PKGROOT     = /opt/modulefiles/$(CATEGORY)/$(PACKAGE)_$(ROLLNETWORK)

VERSION_SRC = $(REDHAT.ROOT)/src/$(PACKAGE)/version.mk
VERSION_INC = version.inc
include $(VERSION_INC)

RPM.EXTRAS  = AutoReq:No\nObsoletes:sdsc-mvapich2-modules_gnu_ib,sdsc-mvapich2-modules_intel_ib,sdsc-mvapich2-modules_pgi_ib
RPM.PREFIX  = $(PKGROOT)
