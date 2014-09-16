ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
COMPILERNAME := $(firstword $(subst /, ,$(ROLLCOMPILER)))

ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

NAME           = mvapich2_$(COMPILERNAME)_$(ROLLNETWORK)
VERSION        = 1.9
RELEASE        = 3
PKGROOT        = /opt/mvapich2/$(COMPILERNAME)/$(ROLLNETWORK)

SRC_SUBDIR     = mvapich2

SOURCE_NAME    = mvapich2
SOURCE_SUFFIX  = tgz
SOURCE_VERSION = $(VERSION)
SOURCE_PKG     = $(SOURCE_NAME)-$(SOURCE_VERSION).$(SOURCE_SUFFIX)
SOURCE_DIR     = $(SOURCE_PKG:%.$(SOURCE_SUFFIX)=%)

LIMIC_KO_NAME  = limic.ko

BARE_FILES     = $(LIMIC_KO_NAME)
TGZ_PKGS       = $(SOURCE_PKG)

RPM.EXTRAS     = AutoReq:No
