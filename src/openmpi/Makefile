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

REDHAT.ROOT = $(PWD)/../../
RPM.EXTRAS=%define __os_install_post /usr/lib/rpm/brp-compress

-include $(ROCKSROOT)/etc/Rules.mk
include pull.mk

CC = gcc
CXX = g++
F77 = gfortran
FC = gfortran
COMPILE_FLAGS = -O3 -mtune=core2 -march=core2 -mfpmath=sse -msse3 -fPIC -ffree-line-length-none
WRAPPER_FLAGS = -O3
ifeq ("$(COMPILERNAME)", "intel")
  CC = icc
  CXX = icpc
  F77 = ifort
  FC = ifort
  COMPILE_FLAGS = -O3 -fPIC
else ifeq ("$(COMPILERNAME)", "pgi")
  CC = pgcc
  CXX = pgCC
  F77 = pgf77
  FC = pgf90
  COMPILE_FLAGS = -fPIC -noswitcherror
endif

ifneq ("$(ROLLOPTS)", "$(subst avx,,$(ROLLOPTS))")
  ifeq ("$(COMPILERNAME)", "gnu")
    COMPILE_FLAGS += -mavx
  endif
endif

COMPILERSETUP =  \
  module load $(ROLLCOMPILER) || true; \
  echo === Compiler and version ===; \
  $(CC) --version

CONFIG_OPTS = \
  --disable-dlopen --enable-static --enable-mca-static \
  --enable-mpi-thread-multiple --with-mpi-f90-size=medium \
  --with-platform=optimized --with-wrapper-cflags='$(WRAPPER_FLAGS)' \
  --with-wrapper-cxxflags='$(WRAPPER_FLAGS)' \
  --with-wrapper-fflags='$(WRAPPER_FLAGS)' \
  --with-wrapper-fcflags='$(WRAPPER_FLAGS)'

ifeq ("$(ROLLNETWORK)", "eth")
  CONFIG_OPTS += --without-openib --with-knem=$(KNEM_CACHE)
else ifeq ("$(ROLLNETWORK)", "mx")
  MXHOME = /opt/mx
  COMPILE_FLAGS += -I$(MXHOME)/include
  CONFIG_OPTS += --with-mx=$(MXHOME) --without-openib
endif

ifneq ("$(ROLLOPTS)", "$(subst sge,,$(ROLLOPTS))")
  CONFIG_OPTS += --with-sge
endif

ifneq ("$(ROLLOPTS)", "$(subst torque,,$(ROLLOPTS))")
  CONFIG_OPTS += --with-tm=/opt/torque
endif

KNEM_CACHE = $(REDHAT.ROOT)/src/build-knem

build: $(SRC_DIRS)
	( \
	  $(COMPILERSETUP); \
	  export CC=$(CC); \
	  export CFLAGS="$(COMPILE_FLAGS)"; \
	  export CXX=$(CXX); \
	  export CXXFLAGS="$(COMPILE_FLAGS)"; \
	  export F77=$(F77); \
	  export FFLAGS="$(COMPILE_FLAGS)"; \
	  export FC=$(FC); \
	  export FCFLAGS="$(COMPILE_FLAGS)"; \
	  cd $(SOURCE_DIR); \
	  if test "$(ROLLNETWORK)" = "ib"; then \
	    mv ../$(KNEM_DIR) .; \
	    cd $(KNEM_DIR); \
	    ./configure --prefix=$(PKGROOT); \
	    make -j 16; \
	    mkdir -p -m 755 $(KNEM_CACHE); \
	    make prefix=$(KNEM_CACHE) -j 16 install; \
	    cd ..; \
	  fi; \
	  ./configure --prefix=$(PKGROOT) $(CONFIG_OPTS); \
	  make -j 16 \
	)

install::
	mkdir -p -m 755 $(ROOT)/$(PKGROOT)
	-mkdir -p -m 755 $(PKGROOT)
	mount --bind $(ROOT)/$(PKGROOT) $(PKGROOT)
	( \
	  $(COMPILERSETUP); \
	  cd $(SOURCE_DIR); \
	  if test "$(ROLLNETWORK)" = "ib"; then \
	    cd $(KNEM_DIR); \
	    make -j 16 install; \
	    cd ..; \
	  fi; \
	  make -j 16 install \
	)
	umount $(PKGROOT)
	-rmdir -p $(PKGROOT)
	find . -name $(KNEM_KO_NAME) -exec cp {} $(ROOT)/$(PKGROOT)/lib/ \;

clean::
	rm -fr $(SOURCE_DIR)
