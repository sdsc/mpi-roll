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
#
# $Log$
# Revision 1.11  2011/12/07 21:08:29  jpg
#
#
# remove mpich exclusion
#
# Revision 1.10  2011/11/29 23:53:11  jpg
#
#
# skipping mpich
#
# Revision 1.9  2011/11/07 23:07:00  jpg
#
#
# reset this to old version
#
# Revision 1.8  2011/11/07 22:54:08  jpg
#
#
# added limic2 for mvapich2
#
# Revision 1.7  2011/11/06 01:59:13  jpg
#
#
# turned back on mpich
#
# Revision 1.6  2011/11/01 19:56:12  jpg
#
#
# temporarily block mpich until I can eliminate mx dependencies
#
# Revision 1.5  2011/03/01 19:12:22  jhayes
# Support ROLLCOMPILER and ROLLNETWORK make variables.  Upgrade mpich2 version.
#
# Revision 1.2  2011/02/16 19:18:27  jhayes
# Update copyrights.
#
# Revision 1.1  2010/07/20 20:52:31  jpg
#
#
# source for amber
#
# Jerry
#
# Revision 1.1  2010/05/14 20:45:14  jpg
#
#
# amber files
#
# Jerry
#
# Revision 1.1  2010/02/08 01:06:51  jhayes
# Add build files.
#
# Revision 1.1  2009/07/29 03:21:52  jpg
#
#  nwchem Makefile linux.mk
#
# Revision 1.1  2009/05/17 19:56:22  jpg
#
#
# Changed the build target in the Makefile so that one can pick a build target,and "actvte"
# target(99.99% of the time it's *UNX). Next, I will add FORTRAN targets where the GAMESS build scripts allows for a choice
#
#

SRCDIRS = `find . -type d -maxdepth 1 \
	-not -name CVS \
	-not -name .`
