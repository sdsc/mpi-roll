ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
COMPILERNAME := $(firstword $(subst /, ,$(ROLLCOMPILER)))

ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

CUDAVERSION=cuda
ifneq ("$(ROLLOPTS)", "$(subst cuda=,,$(ROLLOPTS))")
  CUDAVERSION = $(subst cuda=,,$(filter cuda=%,$(ROLLOPTS)))
endif

NAME           = sdsc-mvapich2_$(COMPILERNAME)_$(ROLLNETWORK)
VERSION        = 2.3
RELEASE        = 0
PKGROOT        = /opt/mvapich2/$(COMPILERNAME)/$(ROLLNETWORK)

MVAPICH2_PKGROOT = /opt/mvapich2/COMPILERNAME/$(ROLLNETWORK)
MVAPICH2_ROOT    = /opt/mvapich2

SRC_SUBDIR     = mvapich2

SOURCE_NAME    = mvapich2
SOURCE_SUFFIX  = tar.gz
SOURCE_VERSION = $(VERSION)
SOURCE_PKG     = $(SOURCE_NAME)-$(SOURCE_VERSION).$(SOURCE_SUFFIX)
SOURCE_DIR     = $(SOURCE_PKG:%.$(SOURCE_SUFFIX)=%)

TAR_GZ_PKGS    = $(SOURCE_PKG)

RPM.EXTRAS     = AutoReq:No
RPM.PREFIX     = $(PKGROOT)
