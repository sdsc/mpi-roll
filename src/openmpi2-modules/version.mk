ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

PACKAGE     = openmpi2
CATEGORY    = mpi

NAME        = sdsc-$(PACKAGE)-modules_$(ROLLNETWORK)
RELEASE     = 1
PKGROOT     = /opt/modulefiles/$(CATEGORY)/$(PACKAGE)_$(ROLLNETWORK)

VERSION_SRC = $(REDHAT.ROOT)/src/$(PACKAGE)/version.mk
VERSION_INC = version.inc
include $(VERSION_INC)

RPM.EXTRAS  = AutoReq:No\nObsoletes:sdsc-openmpi2-modules_gnu_ib,sdsc-openmpi2-modules_intel_ib,sdsc-openmpi2-modules_pgi_ib
