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

PKGROOT = /opt/mvapich2/$(ROLLCOMPILER)/$(ROLLNETWORK)
SOURCE = mvapich2-$(VERSION)
REDHAT.ROOT = $(PWD)/../../
RPM.EXTRAS=%define __os_install_post /usr/lib/rpm/brp-compress

-include $(ROCKSROOT)/etc/Rules.mk

COMPILERSETUP = \
  if test -e /etc/profile.d/modules.sh; then \
    . /etc/profile.d/modules.sh; \
    module load $(ROLLCOMPILER); \
  fi

CC=gcc
CXX=g++
F77=gfortran
FC=gfortran
FFLAGS=-O2 -mavx
FCFLAGS=-O2 -mavx
CFLAGS=-O2 -mavx
CXXFLAGS=-O2 -mavx
ifeq ("$(ROLLCOMPILER)", "intel")
  CC=icc
  CXX=icpc
  F77=ifort
  FC=ifort
  FFLAGS=-O -xHOST
  FCFLAGS=-O -xHOST
  CFLAGS=-O -xHOST
  CXXFLAGS=-O -xHOST
else ifeq ("$(ROLLCOMPILER)", "pgi")
  CC=pgcc
  CXX=pgCC
  F77=pgf77
  FC=pgf90
  FFLAGS=-fast
  FCFLAGS=-fast
  CFLAGS=-fast
  CXXFLAGS=-fast
endif

CONFIG_OPTS = \
  --prefix=$(PKGROOT) --enable-shared --enable-sharedlibs=gcc --with-hwloc
NETWORKSETUP = cat /dev/null
ifeq ("$(ROLLNETWORK)", "eth")
  CONFIG_OPTS += --with-device=ch3:sock
else ifeq ("$(ROLLNETWORK)", "mx")
  MXHOME = /opt/mx
  COMPILE_FLAGS += -I$(MXHOME)/include
  CONFIG_OPTS += --with-device=ch3:nemesis:mx --with-mx=$(MXHOME)
  NETWORKSETUP = export LD_LIBRARY_PATH=$(MXHOME)/lib:$$LD_LIBRARY_PATH
else ifeq ("$(ROLLNETWORK)", "ib")
  CONFIG_OPTS += --enable-f77 --enable-fc --enable-hybrid \
                 --with-ib-include=/usr/include/infiniband \
                 --with-ib-libpath=/usr/lib64 --enable-3dtorus-support \
                 --with-limic2=$(PKGROOT)
endif

build:
	tar zxvf $(SOURCE).tgz
	( \
	  $(COMPILERSETUP); \
	  $(NETWORKSETUP); \
	  cd $(SOURCE); \
	  if test "$(ROLLNETWORK)" = "ib"; then \
	    cd limic2-0.5.5; \
	    mkdir -p $(ROOT)/$(PKGROOT); \
	    if [ ! -d  $(PKGROOT) ]; then mkdir -p $(PKGROOT); fi; \
	    mount --bind $(ROOT)/$(PKGROOT) $(PKGROOT); \
	    ./configure --prefix=$(PKGROOT) --enable-module; \
	    make; \
	    make install; \
	    umount $(PKGROOT); \
            export LD_LIBRARY_PATH=$(PKGROOT)/lib:$${LD_LIBRARY_PATH};  \
	  fi; \
	  export CC=$(CC); \
	  export CXX=$(CXX); \
	  export F77=$(F77); \
	  export FC=$(FC); \
	  export FFLAGS=$(FFLAGS); \
	  export FCFLAGS=$(FCFLAGS); \
	  export CFLAGS=$(CFLAGS); \
	  export CXXFLAGS=$(CXXFLAGS); \
	  cd $(PWD)/$(SOURCE); \
	  if test "$(ROLLNETWORK)" = "ib"; then \
	    mkdir -p $(ROOT)/$(PKGROOT); \
	    if [ ! -d  $(PKGROOT) ]; then mkdir -p $(PKGROOT); fi; \
	    mount --bind $(ROOT)/$(PKGROOT) $(PKGROOT); \
          fi; \
	  ./configure $(CONFIG_OPTS); \
	  make; \
	  if test "$(ROLLNETWORK)" = "ib"; then \
            umount $(PKGROOT); \
          fi \
	)

# Clean immediately after install to free disk space
install::
	mkdir -p -m 755 $(ROOT)/$(PKGROOT)
	if [ ! -d  $(PKGROOT) ]; then mkdir -p $(PKGROOT); fi
	mount --bind $(ROOT)/$(PKGROOT) $(PKGROOT)
	( \
	  $(COMPILERSETUP); \
	  cd $(SOURCE); \
	  make install; \
	)
	umount $(PKGROOT)
	$(MAKE) clean

clean::
	rm -fr $(SOURCE)
