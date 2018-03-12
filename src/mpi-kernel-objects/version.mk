KERNEL         := $(shell uname -r)

NAME           = sdsc-mpi-kernel-objects
VERSION        = $(firstword $(subst -, ,$(KERNEL)))
RELEASE        = $(lastword $(subst -, ,$(KERNEL)))
PKGROOT        = /

MVAPICH2_PKGROOT_SRC = $(REDHAT.ROOT)/src/mvapich2/version.mk
MVAPICH2_PKGROOT_INC = mvapich2_pkgroot.inc
include $(MVAPICH2_PKGROOT_INC)


SRC_SUBDIR     = mpi-kernel-objects

SOURCE_NAME    = mvapich2
SOURCE_SUFFIX  = tar.gz
SOURCE_VERSION = 2.1
SOURCE_PKG     = $(SOURCE_NAME)-$(SOURCE_VERSION).$(SOURCE_SUFFIX)
SOURCE_DIR     = $(SOURCE_PKG:%.$(SOURCE_SUFFIX)=%)

KNEM_NAME      = knem
KNEM_SUFFIX    = tar.gz
KNEM_VERSION   = 1.1.2
KNEM_PKG       = $(KNEM_NAME)-$(KNEM_VERSION).$(KNEM_SUFFIX)
KNEM_DIR       = $(KNEM_PKG:%.$(KNEM_SUFFIX)=%)

TAR_GZ_PKGS    = $(SOURCE_PKG) $(KNEM_PKG)

RPM.EXTRAS     = AutoReq:No
RPM.FILES      := /lib/modules/$(KERNEL)/extra/knem.ko\n/etc/rc.d/rocksconfig.d/post-97-mpiko
ifeq ("$(ROLLNETWORK)", "ib")
  RPM.FILES += \n/lib/modules/$(KERNEL)/extra/limic.ko
endif
