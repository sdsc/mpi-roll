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

ifndef ROLLCOMPILER
  ROLLCOMPILER = gnu
endif
ifndef ROLLNETWORK
  ROLLNETWORK = eth
endif

PKGROOT = /opt/openmpi/$(ROLLCOMPILER)/$(ROLLNETWORK)
SOURCE = openmpi-$(VERSION)
REDHAT.ROOT = $(PWD)/../../

INSTDIR = $(ROOT)/$(PKGROOT)
RPM.EXTRAS=%define __os_install_post /usr/lib/rpm/brp-compress

-include $(ROCKSROOT)/etc/Rules.mk

COMPILERSETUP = \
  if test -e /etc/profile.d/modules.sh; then \
    . /etc/profile.d/modules.sh; \
    module load $(ROLLCOMPILER); \
  fi

CCOMP = gcc
CPPCOMP = g++
FCOMP = gfortran
F90COMP = gfortran
COMPILE_FLAGS = -O2 -mtune=core2 -march=core2 -mfpmath=sse -msse3 -g -fPIC
ifeq ("$(ROLLCOMPILER)", "intel")
  CCOMP = icc
  CPPCOMP = icpc
  FCOMP = ifort
  F90COMP = ifort
  COMPILE_FLAGS = -O
else ifeq ("$(ROLLCOMPILER)", "pgi")
  CCOMP = pgcc
  CPPCOMP = pgCC
  FCOMP = pgf77
  F90COMP = pgf90
  COMPILE_FLAGS = -O
endif

CONFIG_OPTS = --prefix=$(PKGROOT) --disable-dlopen --enable-mca-static \
              --enable-mpi-threads --enable-static --with-mpi-f90-size=large \
              --with-platform=optimized --with-tm=/opt/torque
ifeq ("$(ROLLNETWORK)", "mx")
  MXHOME = /opt/mx
  COMPILE_FLAGS += -I$(MXHOME)/include
  CONFIG_OPTS += --with-mx=$(MXHOME) --without-openib
endif

build:
	tar zxvf $(SOURCE).tar.gz
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
	  cd $(SOURCE); \
	  ./configure $(CONFIG_OPTS); \
	  make \
	)

# Clean immediately after install to free disk space
install::
	mkdir -p $(INSTDIR)
	( \
	  $(COMPILERSETUP); \
	  cd $(SOURCE); \
	  make prefix=$(INSTDIR) install \
	)
	rm -fr $(SOURCE)

clean::
	rm -fr $(SOURCE)