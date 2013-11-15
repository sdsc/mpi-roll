NAME    = mpich_$(ROLLCOMPILER)_$(ROLLNETWORK)
VERSION = 1.2.7
RELEASE = 5

SRC_SUBDIR        = mpich

MPICH_NAME        = mpich
MPICH_VERSION     = $(VERSION)
MPICH_PKG_SUFFIX  = tgz
MPICH_SOURCE_PKG  = $(MPICH_NAME)-$(MPICH_VERSION).$(MPICH_PKG_SUFFIX)
MPICH_SOURCE_DIR  = $(MPICH_SOURCE_PKG:%.$(MPICH_PKG_SUFFIX)=%)

TGZ_PKGS          = $(MPICH_SOURCE_PKG)
