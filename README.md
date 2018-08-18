# SDSC "mpi" roll

## Overview

This roll bundles various flavors of the MPI library.

For more information about the various packages included in the mpi roll please visit their official web pages:

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

autoconf >= 2.69 (mvapich2).  You can get this from the SDSC gnutools-roll.

The sdsc-roll must be installed on the build machine, since the build process
depends on make include files provided by that roll.

The roll sources assume that modulefiles provided by SDSC compiler
rolls are available, but it will build without them as long as the environment
variables they provide are otherwise defined.


## Building

To build the mpi-roll, execute this on a Rocks development
machine (e.g., a frontend or development appliance):

```shell
% make 2>&1 | tee build.log
```

A successful build will create the file `mpi-*.disk1.iso`.  If you built the
roll on a Rocks frontend, proceed to the installation step. If you built the
roll on a Rocks development appliance, you need to copy the roll to your Rocks
frontend before continuing with installation.

This roll source supports building with different compilers.  The
`ROLLCOMPILER` make variable can be used to specify the names of compiler
modulefiles to use for building the software, e.g., 

```shell
% make ROLLCOMPILER='gnu intel' 2>&1 | tee build.log
```

The build processes recognizes the values `gnu`, `intel` and `pgi` for the
ROLLCOMPILER value, defaulting to gnu.

By default, the roll builds openmpi, openmpi2, and mvapich2 rpms.  You can
limit the build to a subset of these using the ROLLMPI make variable, e.g.,

```shell
% make ROLLMPI='mvapich2' 2>&1 | tee build.log
```

By default, the roll builds for ethernet network fabric.  You can expand this
by specifying one or more of the values 'ib' and 'mx' in the ROLLNETWORK make
varible, e.g.,


```shell
% make ROLLNETWORK='ib' 2>&1 | tee build.log
```

For gnu compilers, the roll also supports a `ROLLOPTS` make variable value of
'avx', indicating that the target architecture supports AVX instructions.
If `ROLLOPTS` contains one or more of 'torque', 'slurm', and 'sge', then
openmpi is built to integrate with the specified scheduler(s). 
mvapich2 uses the slurm and torque options as well, but does not have
an sge option.
If `ROLLOPTS` contains 'cuda', then openmpi is compiled with gpu support. 
If 'ROLLOPTS' contains 'lustre' then mvapich2 is compiled with i/o support for
If 'ROLLOPTS' contains 'slurm' then mvapich2 is compiled with support for the sllurm job scheduling system.


## Installation

To install, execute these instructions on a Rocks frontend:

```shell
% rocks add roll *.iso
% rocks enable roll mpi
% cd /export/rocks/install
% rocks create distro
```

Subsequent installs of compute and login nodes will then include the contents
of the mpi-roll.  To avoid cluttering the cluster frontend with unused
software, the mpi-roll is configured to install only on compute and
login nodes. To force installation on your frontend, run this command after
adding the mpi-roll to your distro

```shell
% rocks run roll mpi host=NAME | bash
```

where NAME is the DNS name of a compute or login node in your cluster.

In addition to the software itself, the roll installs package environment
module files in (for example with an ib network):

```shell
/opt/modulefiles/mpi/{mvapich2_ib,openmpi_ib,openmpi2_ib}
```


## Testing

The mpi-roll includes a test script which can be run to verify proper
installation of the roll documentation, binaries and module files. To
run the test scripts execute the following command(s):

```shell
% /root/rolltests/mpi.t 
```
