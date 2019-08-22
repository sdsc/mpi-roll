#
# $Id$
#
# @Copyright@
#
# 				Rocks(r)
# 		         www.rocksclusters.org
# 		         version 7.0 (Manzinita)
#
# Copyright (c) 2000 - 2017 The Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice unmodified and in its entirety, this list of conditions and the
# following disclaimer in the documentation and/or other materials provided
# with the distribution.
#
# 3. All advertising and press materials, printed or electronic, mentioning
# features or use of this software must display the following acknowledgement:
#
# 	"This product includes software developed by the Rocks(r)
# 	Cluster Group at the San Diego Supercomputer Center at the
# 	University of California, San Diego and its contributors."
#
# 4. Except as permitted for the purposes of acknowledgment in paragraph 3,
# neither the name or logo of this software nor the names of its
# authors may be used to endorse or promote products derived from this
# software without specific prior written permission.  The name of the
# software includes the following terms, and any derivatives thereof:
# "Rocks", "Rocks Clusters", and "Avalanche Installer".  For licensing of
# the associated name, interested parties should contact Technology
# Transfer & Intellectual Property Services, University of California,
# San Diego, 9500 Gilman Drive, Mail Code 0910, La Jolla, CA 92093-0910,
# Ph: (858) 534-5815, FAX: (858) 534-7345, E-MAIL:invent@ucsd.edu
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
# BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# @Copyright@

REDHAT.ROOT = $(PWD)/../../
RPM.EXTRAS=%define __os_install_post /usr/lib/rpm/brp-compress

-include $(ROCKSROOT)/etc/Rules.mk
include $(SDSCDEVEL)/Pull.mk
include $(SDSCDEVEL)/Rules.mk

COMPILE_FLAGS = -fPIC -O3

LIMIC_CACHE = $(REDHAT.ROOT)/cache/build-limic

# Parallel make breaks build with 2016 Intel compiler

CONFIG_OPTS = --enable-shared --enable-sharedlibs=gcc --with-hwloc

CPPFLAGS=
ifeq ("$(COMPILERNAME)", "intel")
  COMPILE_FLAGS = -fPIC -O3
else ifeq ("$(COMPILERNAME)", "pgi")
  COMPILE_FLAGS = -fPIC -O3 -noswitcherror
  CONFIG_OPTS += --disable-rpath --disable-static --disable-rdma-cm --without-hydra-ckpointlib --enable-f77 --enable-fc --enable-hybrid
endif

NETWORKSETUP = cat /dev/null
ifeq ("$(ROLLNETWORK)", "eth")
  CONFIG_OPTS += --with-device=ch3:sock
else ifeq ("$(ROLLNETWORK)", "mx")
  MXHOME = /opt/mx
  COMPILE_FLAGS += -I$(MXHOME)/include
  CONFIG_OPTS += --with-device=ch3:nemesis:mx --with-mx=$(MXHOME)
  NETWORKSETUP = export LD_LIBRARY_PATH=$(MXHOME)/lib:$$LD_LIBRARY_PATH
else ifeq ("$(ROLLNETWORK)", "ib")
  CONFIG_OPTS += --with-ib-include=/usr/include/infiniband \
                 --with-ib-libpath=/usr/lib64 \
                 --enable-fast=O3 --with-limic2=$(LIMIC_CACHE)
endif

ifneq ("$(ROLLOPTS)", "$(subst avx,,$(ROLLOPTS))")
  ifneq ("$(COMPILERNAME)", "gnu")
    CONFIG_OPTS += --enable-avx
  endif
endif

ifneq ("$(ROLLOPTS)", "$(subst torus,,$(ROLLOPTS))")
  CONFIG_OPTS += --enable-3dtorus-support
endif

ifneq ("$(ROLLOPTS)", "$(subst torque,,$(ROLLOPTS))")
  CONFIG_OPTS += --with-pbs=/opt/torque
endif

ifneq ("$(ROLLOPTS)", "$(subst slurm,,$(ROLLOPTS))")
  CONFIG_OPTS += --with-slurm=/usr/lib64/slurm
endif

ifneq ("$(ROLLOPTS)", "$(subst lustre,,$(ROLLOPTS))")
  CONFIG_OPTS += --with-file-system=lustre
endif

LIMIC_CACHE = $(REDHAT.ROOT)/cache/build-limic

$(NAME).spec: DESCRIPTION LICENSE

DESCRIPTION:
	$(MODULE_LOAD_CC); \
	( \
	  $(DESCRIBE_PKG); \
	  $(DESCRIBE_CC); \
	) > $@

LICENSE: $(SRC_DIRS)
	$(CHECK_LICENSE_FILES)
	echo BSD > $@

build: $(SRC_DIRS)
	patch $(SOURCE_DIR)/src/pm/hydra/configure patch-files/configure.hydra.patch
	( \
	  $(MODULE_LOAD_CC); \
	  $(NETWORKSETUP); \
	  export CC=$(CC); \
	  export CXX=$(CXX); \
	  export F77=$(FC); \
	  export FC=$(FC); \
	  export FFLAGS="$(COMPILE_FLAGS)"; \
	  export FCFLAGS="$(COMPILE_FLAGS)"; \
	  export CFLAGS="$(COMPILE_FLAGS)"; \
	  export CXXFLAGS="$(COMPILE_FLAGS)"; \
          export CPPFLAGS="$(CPPFLAGS)"; \
	  export LD_LIBRARY_PATH=/opt/torque/lib64:$(LIMIC_CACHE)/lib:$${LD_LIBRARY_PATH}; \
	  if test "$(ROLLNETWORK)" = "ib"; then \
             /bin/cp $(LIMIC_CACHE)/lib/liblimic2.la.in $(LIMIC_CACHE)/lib/liblimic2.la; \
             perl -pi -e "s!COMPILERNAME!$(COMPILERNAME)!" $(LIMIC_CACHE)/lib/liblimic2.la; \
          fi; \
	  if test "$(COMPILERNAME)" = "gnu"; then \
	    export MPICHLIB_CFLAGS='-std=gnu90'; \
	  fi; \
          cd $(SOURCE_DIR); \
	  ./configure --prefix=$(PKGROOT) $(CONFIG_OPTS); \
	  make -j 16; \
	)

install::
	mkdir -p -m 755 $(ROOT)/$(PKGROOT)
	$(PKGROOT_BIND_MOUNT)
	( \
	   $(MODULE_LOAD_CC); \
	   cd $(SOURCE_DIR); \
	   make -j 16 $(MAKE_OPTS) install; \
	   if test "$(ROLLNETWORK)" = "ib"; then \
	     /bin/cp -r $(LIMIC_CACHE)/* $(ROOT)/$(PKGROOT)/; \
             rm -f $(ROOT)/$(PKGROOT)/lib/liblimic2.la.in; \
	   fi \
        )
	$(PKGROOT_BIND_UMOUNT)
	$(INSTALL_LICENSE_FILES)

clean::
	rm -fr $(SOURCE_DIR) DESCRIPTION LICENSE

