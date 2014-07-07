#
# $Id$
#
# @Copyright@
#
# 				Rocks(tm)
# 		         www.rocksclusters.org
# 		        version 4.3 (Mars Hill)
#
# Copyright (c) 2000 - 2011 The Regents of the University of California.
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
# 	"This product includes software developed by the Rocks(tm)
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

PKGROOT = /opt/openmpi/$(COMPILERNAME)/$(ROLLNETWORK)
SOURCE = $(OPENMPI_SOURCE_DIR)
REDHAT.ROOT = $(PWD)/../../
RPM.EXTRAS=%define __os_install_post /usr/lib/rpm/brp-compress

-include $(ROCKSROOT)/etc/Rules.mk
include pull.mk

COMPILERSETUP = \
  if test -e /etc/profile.d/modules.sh; then \
    . /etc/profile.d/modules.sh; \
    module=`module avail 2>&1 | grep -m 1 -o "\<$(ROLLCOMPILER)\>"`; \
    if test "$${module}" != "$(ROLLCOMPILER)"; then \
      echo *** ERROR: Compiler $(ROLLCOMPILER) not found; \
      exit 2; \
    fi; \
    module load $(ROLLCOMPILER); \
  fi

CCOMP = gcc
CPPCOMP = g++
FCOMP = gfortran
F90COMP = gfortran
COMPILE_FLAGS = -O3 -mtune=core2 -march=core2 -mfpmath=sse -msse3 -fPIC -ffree-line-length-none
WRAPPER_FLAGS = -O3
ifeq ("$(COMPILERNAME)", "intel")
  CCOMP = icc
  CPPCOMP = icpc
  FCOMP = ifort
  F90COMP = ifort
  COMPILE_FLAGS = -O3   -fPIC
  WRAPPER_FLAGS = -O3
else ifeq ("$(COMPILERNAME)", "pgi")
  CCOMP = pgcc
  CPPCOMP = pgCC
  FCOMP = pgf77
  F90COMP = pgf90
  COMPILE_FLAGS = -fPIC -noswitcherror
  WRAPPER_FLAGS = -O3
else ifneq ("$(ROLLOPTS)", "$(subst avx,,$(ROLLOPTS))")
  COMPILE_FLAGS += -mavx
  WRAPPER_FLAGS = -mavx
endif
LOADMGR =
ifneq ("$(ROLLOPTS)", "$(subst sge,,$(ROLLOPTS))")
  LOADMGR += --with-sge
endif
ifneq ("$(ROLLOPTS)", "$(subst torque,,$(ROLLOPTS))")
  LOADMGR += --with-tm=/opt/torque
endif

CONFIG_OPTS = --disable-dlopen --enable-mca-static \
              --enable-mpi-thread-multiple --enable-static --with-mpi-f90-size=medium \
              --with-platform=optimized $(LOADMGR) --with-wrapper-cflags='$(WRAPPER_FLAGS)' --with-wrapper-cxxflags='$(WRAPPER_FLAGS)' --with-wrapper-fflags='$(WRAPPER_FLAGS)' --with-wrapper-fcflags='$(WRAPPER_FLAGS)'

CPPFLAGS=
ifeq ("$(ROLLNETWORK)", "eth")
  CONFIG_OPTS += --without-openib
else ifeq ("$(ROLLNETWORK)", "mx")
  MXHOME = /opt/mx
  COMPILE_FLAGS += -I$(MXHOME)/include
  CONFIG_OPTS += --with-mx=$(MXHOME) --without-openib
endif

build: $(SRC_DIRS)
	( \
	  $(COMPILERSETUP); \
	  export CC=$(CCOMP); \
	  export CFLAGS="$(COMPILE_FLAGS)"; \
	  export CXX=$(CPPCOMP); \
	  export CXXFLAGS="$(COMPILE_FLAGS)"; \
	  export F77=$(FCOMP); \
	  export FFLAGS="$(COMPILE_FLAGS)"; \
	  export FC=$(F90COMP); \
	  export FCFLAGS="$(COMPILE_FLAGS)"; \
	  export KNEM_OPTS=""; \
	  cd $(SOURCE); \
	  if test "$(ROLLNETWORK)" = "ib"; then \
	    mv ../$(KNEM_SOURCE_DIR) .; \
	    cd $(KNEM_SOURCE_DIR); \
	    ./configure --prefix=$(PKGROOT); \
	    make -j 16; \
	    mkdir -p -m 755 $(REDHAT.ROOT)/src/build-knem; \
	    make prefix=$(REDHAT.ROOT)/src/build-knem -j 16 install; \
	    export KNEM_OPTS=--with-knem=$(REDHAT.ROOT)/src/build-knem; \
	    cd ..; \
	  fi; \
	  ./configure --prefix=$(PKGROOT) $(CONFIG_OPTS) $${KNEM_OPTS}; \
	  make -j 16 \
	)

install::
	mkdir -p -m 755 $(ROOT)/$(PKGROOT)
	-mkdir -p -m 755 $(PKGROOT)
	mount --bind $(ROOT)/$(PKGROOT) $(PKGROOT)
	( \
	  $(COMPILERSETUP); \
	  cd $(SOURCE); \
	  if test "$(ROLLNETWORK)" = "ib"; then \
	    cd $(KNEM_SOURCE_DIR); \
	    make -j 16 install; \
	    cd ..; \
	  fi; \
	  make -j 16 install \
	)
	umount $(PKGROOT)
	-rmdir -p $(PKGROOT)
	find . -name $(KNEM_KO_NAME) -exec cp {} $(ROOT)/$(PKGROOT)/lib/ \;
	mkdir -p $(REDHAT.ROOT)/src/build-mpi-modules
	cat openmpi.module | perl -pi -e "s!MODULEROOT!/opt/modulefiles/mpi/.$(COMPILERNAME)/openmpi_$(ROLLNETWORK)!g;s!VERSION!$(VERSION)!g;s!COMPILERNAME!$(COMPILERNAME)!g;s!ROLLNETWORK!$(ROLLNETWORK)!g" \
          > $(REDHAT.ROOT)/src/build-mpi-modules/$(NAME).module

clean::
	rm -fr $(SOURCE)
