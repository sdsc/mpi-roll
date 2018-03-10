# --------------------------------------------------- -*- Makefile -*- --
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

REDHAT.ROOT = $(CURDIR)/../../

-include $(ROCKSROOT)/etc/Rules.mk
include $(SDSCDEVEL)/Pull.mk
include $(SDSCDEVEL)/Rules.mk

KNEM_CACHE = $(REDHAT.ROOT)/cache/build-knem
LIMIC_CACHE = $(REDHAT.ROOT)/cache/build-limic

KO_PKGROOT = /lib/modules/$(KERNEL)/extra
RC_PKGROOT = /etc/rc.d/rocksconfig.d

KNEM_LICENSE_FILES = knem.spec

$(NAME).spec: DESCRIPTION LICENSE

DESCRIPTION:
	( \
	  $(DESCRIBE_PKG); \
	  $(DESCRIBE_CC) \
	) > $@

LICENSE: $(SRC_DIRS)
	for F in $(KNEM_LICENSE_FILES); do \
	  cp knem*/$$F $(SOURCE_DIR)/knem-$$F; \
	done
	$(CHECK_LICENSE_FILES)
	echo BSD and GPLv2 > $@

$(MVAPICH2_PKGROOT_INC): $(MVAPICH2_PKGROOT_SRC)
	/bin/grep 'MVAPICH2_PKGROOT.*=' $(MVAPICH2_PKGROOT_SRC) > $@
	/bin/grep 'MVAPICH2_ROOT.*=' $(MVAPICH2_PKGROOT_SRC) >> $@

build: $(SRC_DIRS) $(MVAPICH2_PKGROOT_INC)
	echo $(MVAPICH2_PKGROOT) >o
	if test "$(ROLLNETWORK)" = "ib"; then \
	  cd $(SOURCE_DIR)/limic2-*; \
	  ./configure --prefix=$(MVAPICH2_PKGROOT) --enable-module; \
	  make -j 16; \
	fi
	( \
	  cd $(KNEM_DIR); \
	  ./configure --prefix=$(KO_PKGROOT); \
	  make -j 16; \
	)

install::
	mkdir -p -m 755 $(ROOT)/$(KO_PKGROOT) $(ROOT)/$(RC_PKGROOT)
	mkdir -p -m 755 $(ROOT)/$(KO_PKGROOT)/lib
	if test "$(ROLLNETWORK)" = "ib"; then \
          cd $(SOURCE_DIR)/limic2-*; \
          cp `find . -name \*.ko` $(ROOT)/$(KO_PKGROOT)/; \
          rm -fr $(LIMIC_CACHE); \
          mkdir -p -m 755 $(LIMIC_CACHE); \
          make -j 16 install; \
          cp -r $(MVAPICH2_PKGROOT)/* $(LIMIC_CACHE); \
          rm -rf $(MVAPICH2_PKGROOT); \
          cp -r $(LIMIC_CACHE)/lib/liblimic2.la $(LIMIC_CACHE)/lib/liblimic2.la.in; \
          rm -rf $(MVAPICH2_ROOT); \
        fi
	( \
	  cd $(KNEM_DIR); \
	  mkdir -p -m 755 $(KNEM_CACHE); \
	  cp `find . -name \*.ko` $(ROOT)/$(KO_PKGROOT)/; \
	  rm -fr $(KNEM_CACHE); \
	  make prefix=$(KNEM_CACHE) install \
	)
	cp post* $(ROOT)/$(RC_PKGROOT)/

clean::
	rm -fr $(SOURCE_DIR) DESCRIPTION LICENSE $(MVAPICH2_PKGROOT_INC)

