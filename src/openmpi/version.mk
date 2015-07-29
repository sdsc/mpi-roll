ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
COMPILERNAME := $(firstword $(subst /, ,$(ROLLCOMPILER)))

ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

NAME           = sdsc-openmpi_$(COMPILERNAME)_$(ROLLNETWORK)
VERSION        = 1.8.4
RELEASE        = 1
PKGROOT        = /opt/openmpi/$(COMPILERNAME)/$(ROLLNETWORK)

SRC_SUBDIR     = openmpi

SOURCE_NAME    = openmpi
SOURCE_SUFFIX  = tar.gz
SOURCE_VERSION = $(VERSION)
SOURCE_PKG     = $(SOURCE_NAME)-$(SOURCE_VERSION).$(SOURCE_SUFFIX)
SOURCE_DIR     = $(SOURCE_PKG:%.$(SOURCE_SUFFIX)=%)

KNEM_NAME      = knem
KNEM_SUFFIX    = tar.gz
KNEM_VERSION   = 1.1.2
KNEM_PKG       = $(KNEM_NAME)-$(KNEM_VERSION).$(KNEM_SUFFIX)
KNEM_DIR       = $(KNEM_PKG:%.$(KNEM_SUFFIX)=%)

TAR_GZ_PKGS    = $(SOURCE_PKG) $(KNEM_PKG)

RPM.EXTRAS     = AutoReq:No
