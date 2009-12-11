#
# $Id$
#
# @Copyright@
#
# 				Rocks(tm)
# 		         www.rocksclusters.org
# 		        version 4.3 (Mars Hill)
#
# Copyright (c) 2000 - 2007 The Regents of the University of California.
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
#

PKGROOT = /opt/mpich_mx
INSTDIR = $(ROOT)/$(PKGROOT)
SOURCE = mpich-$(VERSION)
REDHAT.ROOT = $(PWD)/../../
RPM.EXTRAS=%define __os_install_post /usr/lib/rpm/brp-compress

-include $(ROCKSROOT)/etc/Rules.mk

INTEL_BASE = /opt/intel/Compiler/11.1/046
C_INTEL_ENV = $(INTEL_BASE)/bin/intel64/iccvars_intel64.sh
F_INTEL_ENV = $(INTEL_BASE)/bin/intel64/ifortvars_intel64.sh
INTEL_LICENSE_FILE = /opt/flexlm/license/license-intel.txt

MXHOME = /opt/mx
COMPILE_FLAGS = -I$(MXHOME)/include -O

build:
	tar zxvf $(REDHAT.ROOT)/src/$(SOURCE).tgz
	( \
	  . $(C_INTEL_ENV); \
	  . $(F_INTEL_ENV); \
	  export INTEL_LICENSE_FILE=$(INTEL_LICENSE_FILE); \
	  export CC=icc; \
	  export CFLAGS="$(COMPILE_FLAGS)"; \
	  export CXX=icpc; \
	  export F90=ifort; \
	  export F90FLAGS="$(COMPILE_FLAGS)"; \
	  export FC=ifort; \
	  export FFLAGS="$(COMPILE_FLAGS)"; \
	  export LINKOPT="-Wl,-rpath,$(MXHOME)/lib64,-rpath,$(MXHOME)/lib -L$(MXHOME)/lib64 -L$(MXHOME)/lib -lmyriexpress";  \
	  export MX_HOME=$(MXHOME); \
	  export PREFIX=$(PKGROOT); \
	  export RSHCOMMAND=ssh; \
	  cd $(SOURCE); \
	  ./configure --prefix=$(PKGROOT) -lib="$$LINKOPT" -opt="-O" \
	              --with-device=ch_mx --enable-sharedlib; \
	  make \
	)

# Clean immediately after install to free disk space
install::
	mkdir -p $(INSTDIR)
	if [ ! -d $(PKGROOT) ]; then mkdir -p $(PKGROOT); fi
	mount --bind $(INSTDIR) $(PKGROOT)
	( \
	  . $(C_INTEL_ENV); \
	  . $(F_INTEL_ENV); \
	  export INTEL_LICENSE_FILE=$(INTEL_LICENSE_FILE); \
	  cd $(SOURCE); \
	  make install \
	)
	umount $(PKGROOT)
	mv $(INSTDIR)/bin $(INSTDIR)/binintel
	mkdir $(INSTDIR)/bin
	mv $(INSTDIR)/binintel $(INSTDIR)/bin/intel
	mv $(INSTDIR)/lib $(INSTDIR)/libintel
	mkdir $(INSTDIR)/lib
	mv $(INSTDIR)/libintel $(INSTDIR)/lib/intel
	rm -fr $(SOURCE)

clean::
	rm -fr $(SOURCE)
