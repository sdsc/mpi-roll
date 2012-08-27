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

PKGROOT = /opt/mpich2/$(ROLLCOMPILER)/$(ROLLNETWORK)
SOURCE = mpich2-$(VERSION)
REDHAT.ROOT = $(PWD)/../../
RPM.EXTRAS=%define __os_install_post /usr/lib/rpm/brp-compress

-include $(ROCKSROOT)/etc/Rules.mk

COMPILERSETUP = \
  if test -e /etc/profile.d/modules.sh; then \
    . /etc/profile.d/modules.sh; \
    latestModule=`module avail 2>&1 | grep -o '$(ROLLCOMPILER)/[^ (]*' | sort -t. -k1,1n -k2,2n -k3,3n | tail -1`; \
    module load $${latestModule}; \
  fi

CCOMP = gcc
CPPCOMP = g++
FCOMP = gfortran
F90COMP = gfortran
COMPILE_FLAGS = -mavx -O2
ifeq ("$(ROLLCOMPILER)", "intel")
  CCOMP = icc
  CPPCOMP = icpc
  FCOMP = ifort
  F90COMP = ifort
  COMPILE_FLAGS = -O -xHOST -fPIC
else ifeq ("$(ROLLCOMPILER)", "pgi")
  CCOMP = pgcc
  CPPCOMP = pgCC
  FCOMP = pgf90
  F90COMP = pgf90
  COMPILE_FLAGS = -fast -fPIC
endif

CONFIG_OPTS = --prefix=$(PKGROOT) --enable-shared --enable-sharedlibs=gcc
NETWORKSETUP = cat /dev/null
ifeq ("$(ROLLNETWORK)", "mx")
  MXHOME = /opt/mx
  COMPILE_FLAGS += -I$(MXHOME)/include
  CONFIG_OPTS += --with-device=ch3:nemesis:mx \
                 --with-mx-include=$(MXHOME)/include \
                 --with-mx-lib=$(MXHOME)/lib
  NETWORKSETUP = export LD_LIBRARY_PATH=$(MXHOME)/lib:$$LD_LIBRARY_PATH
endif

build:
	tar zxvf $(SOURCE).tgz
	( \
	  $(COMPILERSETUP); \
	  $(NETWORKSETUP); \
	  export CC=$(CCOMP); \
	  export CFLAGS="$(COMPILE_FLAGS)"; \
	  export CXX=$(CPPCOMP); \
	  export CXXFLAGS="$(COMPILE_FLAGS)"; \
	  export FC=$(FCOMP); \
	  export FCFLAGS="$(COMPILE_FLAGS)"; \
	  export PREFIX=$(PKGROOT); \
	  cd $(SOURCE); \
	  ./configure $(CONFIG_OPTS); \
	  make \
	)

# Clean immediately after install to free disk space
install::
	mkdir -p -m 755 $(ROOT)/$(PKGROOT)
	mkdir -p $(REDHAT.ROOT)/src/build-mpi-modules
	if [ ! -d $(PKGROOT) ]; then mkdir -p $(PKGROOT); fi
	mount --bind $(ROOT)/$(PKGROOT) $(PKGROOT)
	( \
	  $(COMPILERSETUP); \
	  cd $(SOURCE); \
	  make install \
	)
	cat mpich2.module | perl -pi -e "s!MODULEROOT!/opt/modulefiles/mpi/.$(ROLLCOMPILER)/mpich2_$(ROLLNETWORK)!g;s!VERSION!$(VERSION)!g;s!ROLLCOMPILER!$(ROLLCOMPILER)!g;s!ROLLNETWORK!$(ROLLNETWORK)!g" \
          > $(REDHAT.ROOT)/src/build-mpi-modules/$(NAME).module
	umount $(PKGROOT)
	$(MAKE) clean

clean::
	rm -fr $(SOURCE)
