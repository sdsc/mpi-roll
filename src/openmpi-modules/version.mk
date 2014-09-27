ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
COMPILERNAME := $(firstword $(subst /, ,$(ROLLCOMPILER)))

ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

NAME        = openmpi-modules_$(COMPILERNAME)
RELEASE     = 0
PKGROOT     = /opt/modulefiles/mpi/.$(COMPILERNAME)/openmpi_$(ROLLNETWORK)

VERSION_SRC = $(REDHAT.ROOT)/src/openmpi/version.mk
VERSION_INC = version.inc
include $(VERSION_INC)

RPM.EXTRAS  = AutoReq:No
