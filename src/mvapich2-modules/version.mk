ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
COMPILERNAME := $(firstword $(subst /, ,$(ROLLCOMPILER)))

ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

NAME        = mvapich2-modules_$(COMPILERNAME)
RELEASE     = 0
PKGROOT     = /opt/modulefiles/mpi/.$(COMPILERNAME)/mvapich2_$(ROLLNETWORK)

VERSION_SRC = $(REDHAT.ROOT)/src/mvapich2/version.mk
VERSION_INC = version.inc
include $(VERSION_INC)

RPM.EXTRAS  = AutoReq:No
