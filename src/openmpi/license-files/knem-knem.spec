# Copyright © inria 2009-2010
# Brice Goglin <Brice.Goglin@inria.fr>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

%define debug_package %{nil}

Summary: KNEM: High-Performance Intra-Node MPI Communication
Name: knem
Version: 1.1.0
Release: 0
License: BSD
Group: System Environment/Libraries
Packager: Brice Goglin
Source: knem-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-%{version}-build

%description
KNEM is a Linux kernel module enabling high-performance intra-node MPI communication for large messages. KNEM offers support for asynchronous and vectorial data transfers as well as offloading memory copies on to Intel I/OAT hardware.
See http://runtime.bordeaux.inria.fr/knem/ for details.

%prep
%setup -n knem-%{version}

%build
./configure --prefix=/opt/knem-%{version}
make

%install
make DESTDIR=$RPM_BUILD_ROOT install
mkdir -p $RPM_BUILD_ROOT/etc/udev/rules.d
mkdir -p $RPM_BUILD_ROOT/lib/modules/$(uname -r)/extra
DESTDIR=$RPM_BUILD_ROOT $RPM_BUILD_ROOT/opt/knem-%{version}/sbin/knem_local_install

%clean
rm -rf $RPM_BUILD_ROOT

%post
getent group rdma >/dev/null 2>&1 || groupadd -r rdma
touch /etc/udev/rules.d/10-knem.rules
depmod -a

%files
%defattr(-, root, root)
/opt
/lib/modules

%config(noreplace)
/etc/udev/rules.d/10-knem.rules
