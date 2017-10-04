ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

PACKAGE     = openmpi
CATEGORY    = mpi

NAME        = sdsc-$(PACKAGE)-modules_$(ROLLNETWORK)
RELEASE     = 5
PKGROOT     = /opt/modulefiles/$(CATEGORY)/$(PACKAGE)_$(ROLLNETWORK)

VERSION_SRC = $(REDHAT.ROOT)/src/$(PACKAGE)/version.mk
VERSION_INC = version.inc
include $(VERSION_INC)

RPM.EXTRAS  = AutoReq:No\nObsoletes:sdsc-openmpi-modules_gnu_ib,sdsc-openmpi-modules_intel_ib,sdsc-openmpi-modules_pgi_ib
RPM.PREFIX  = $(PKGROOT)
