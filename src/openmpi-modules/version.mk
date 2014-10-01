ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
COMPILERNAME := $(firstword $(subst /, ,$(ROLLCOMPILER)))

ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

PACKAGE     = openmpi
CATEGORY    = mpi

NAME        = $(PACKAGE)-modules_$(COMPILERNAME)
RELEASE     = 1
PKGROOT     = /opt/modulefiles/$(CATEGORY)/.$(COMPILERNAME)/$(PACKAGE)_$(ROLLNETWORK)

VERSION_SRC = $(REDHAT.ROOT)/src/$(PACKAGE)/version.mk
VERSION_INC = version.inc
include $(VERSION_INC)

RPM.EXTRAS  = AutoReq:No
