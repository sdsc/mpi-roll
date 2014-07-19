# SDSC "mpi" roll

## Overview

This roll bundles various flavors of the MPI library.

For more information about the various packages included in the mpi roll please visit their official web pages:

- <a href="http://www.mpich.org" target="_blank">MPICH2</a> is a high
performance and widely portable implementation of the Message Passing Interface
(MPI) standard.
- <a href="http://mvapich.cse.ohio-state.edu/overview/mvapich2/"
target="_blank">MVAPICH2</a> is an MPI-3 implementation.
- <a href="http://www.open-mpi.org" target="_blank">Open MPI</a> is an open
source MPI-2 implementation that is developed and maintained by a consortium of
academic, research, and industry partners.


## Requirements

To build/install this roll you must have root access to a Rocks development
machine (e.g., a frontend or development appliance).

If your Rocks development machine does *not* have Internet access you must
download the appropriate mpi source file(s) using a machine that does
have Internet access and copy them into the `src/<package>` directories on your
Rocks development machine.


## Dependencies

autoconf >= 2.69 (mvapich2)


## Building

To build the mpi-roll, execute these instructions on a Rocks development
machine (e.g., a frontend or development appliance):

```shell
% make default 2>&1 | tee build.log
% grep "RPM build error" build.log
```

If nothing is returned from the grep command then the roll should have been
created as... `mpi-*.iso`. If you built the roll on a Rocks frontend then
proceed to the installation step. If you built the roll on a Rocks development
appliance you need to copy the roll to your Rocks frontend before continuing
with installation.

This roll source supports building with different compilers and for different
network fabrics and mpi flavors.  By default, it builds using the gnu compilers
for openmpi ethernet.  To build for a different configuration, use the
`ROLLCOMPILER`, `ROLLMPI` and `ROLLNETWORK` make variables, e.g.,

```shell
make ROLLCOMPILER=intel ROLLMPI=mpich2 ROLLNETWORK=mx 
```

The build process currently supports one or more of the values "intel", "pgi",
and "gnu" for the `ROLLCOMPILER` variable, defaulting to "gnu".  It supports
`ROLLMPI` values "openmpi", "mpich2", and "mvapich2", defaulting to "openmpi".
It uses any `ROLLNETWORK` variable value(s) to load appropriate mpi modules,
assuming that there are modules named `$(ROLLMPI)_$(ROLLNETWORK)` available
(e.g., `openmpi_ib`, `mpich2_mx`, etc.).  The build process uses the
ROLLCOMPILER value to load an environment module, so you can also use it to
specify a particular compiler version, e.g.,

```shell
% make ROLLCOMPILER=gnu/4.8.1
```

If the `ROLLCOMPILER`, `ROLLNETWORK` and/or `ROLLMPI` variables are specified,
their values are incorporated into the names of the produced rpms, e.g.,

```shell
make ROLLCOMPILER=intel ROLLMPI=mvapich2 ROLLNETWORK=ib
```
produces an rpm that begins "`mvapich2_intel_ib`".

For gnu compilers, the roll also supports a `ROLLOPTS` make variable value of
'avx', indicating that the target architecture supports AVX instructions.

If `ROLLOPTS` contains one or both of 'torque' and 'sge', then openmpi is built
to integrate with the specified scheduler(s).  If `ROLLOPTS` contains 'torus',
then mvapich2 is compiled with 3d torus support.

## Installation

To install, execute these instructions on a Rocks frontend:

```shell
% rocks add roll *.iso
% rocks enable roll mpi
% cd /export/rocks/install
% rocks create distro
% rocks run roll mpi | bash
```

In addition to the software itself, the roll installs mpi environment
module files in:

```shell
/opt/modulefiles/mpi/.(compiler)
```


## Testing

The mpi-roll includes a test script which can be run to verify proper
installation of the mpi-roll documentation, binaries and module files. To
run the test scripts execute the following command(s):

```shell
% /root/rolltests/mpi.t 
```
