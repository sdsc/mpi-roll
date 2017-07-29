ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
COMPILERNAME := $(firstword $(subst /, ,$(ROLLCOMPILER)))

ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

NAME           = sdsc-openmpi_$(COMPILERNAME)_$(ROLLNETWORK)
VERSION        = 1.8.4
RELEASE        = 4
PKGROOT        = /opt/openmpi/$(COMPILERNAME)/$(ROLLNETWORK)

SRC_SUBDIR     = openmpi

SOURCE_NAME    = openmpi
SOURCE_SUFFIX  = tar.gz
SOURCE_VERSION = $(VERSION)
SOURCE_PKG     = $(SOURCE_NAME)-$(SOURCE_VERSION).$(SOURCE_SUFFIX)
SOURCE_DIR     = $(SOURCE_PKG:%.$(SOURCE_SUFFIX)=%)

TAR_GZ_PKGS    = $(SOURCE_PKG)

RPM.EXTRAS     = AutoReq:No
