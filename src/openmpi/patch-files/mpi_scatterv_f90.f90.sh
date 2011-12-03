#! /bin/sh

#
# Copyright (c) 2004-2006 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2006-2011 Cisco Systems, Inc.  All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

#
# This file generates a Fortran code to bridge between an explicit F90
# generic interface and the F77 implementation.
#
# This file is automatically generated by either of the scripts
#   ../xml/create_mpi_f90_medium.f90.sh or
#   ../xml/create_mpi_f90_large.f90.sh
#

. "$1/fortran_kinds.sh"

# This entire file is only generated in large modules.  So if
# we're not at least large, bail now.

check_size large
if test "$output" = "0"; then
    exit 0
fi

# Ok, we should continue.

allranks="0 $ranks"


output() {
    procedure=$1
    rank=$2
    type=$4
    proc="$1$2D$3"

    cat <<EOF

! Because we can't break ABI in the middle of the 1.4 series, also
! provide the old/bad/incorrect MPI_Scatterv binding
subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)
  include "mpif-config.h"
  ${type}, intent(in) :: sendbuf
  integer, intent(in) :: sendcounts
  integer, intent(in) :: displs
  integer, intent(in) :: sendtype
  ${type}, intent(out) :: recvbuf
  integer, intent(in) :: recvcount
  integer, intent(in) :: recvtype
  integer, intent(in) :: root
  integer, intent(in) :: comm
  integer, intent(out) :: ierr
  print *, "Open MPI WARNING: You are calling MPI_SCATTERV with incorrect sendcounts and displs arguments!"
  print *, "Your code may crash or produce incorrect results.  ***Your code will fail to compile in future"
  print *, "versions of Open MPI*** because this old/incorrect Fortran subroutine binding will be removed."
  print *, "Please update the type of your sendcounts and displs  parameters to make this warning go away (and have correct code!)."
  call ${procedure}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)
end subroutine ${proc}

subroutine ${proc}_correct(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)
  include "mpif-config.h"
  ${type}, intent(in) :: sendbuf
  integer, dimension(*), intent(in) :: sendcounts
  integer, dimension(*), intent(in) :: displs
  integer, intent(in) :: sendtype
  ${type}, intent(out) :: recvbuf
  integer, intent(in) :: recvcount
  integer, intent(in) :: recvtype
  integer, intent(in) :: root
  integer, intent(in) :: comm
  integer, intent(out) :: ierr
  call ${procedure}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)
end subroutine ${proc}_correct

EOF
}

for rank in $allranks
do
  case "$rank" in  0)  dim=''  ;  esac
  case "$rank" in  1)  dim=', dimension(*)'  ;  esac
  case "$rank" in  2)  dim=', dimension(1,*)'  ;  esac
  case "$rank" in  3)  dim=', dimension(1,1,*)'  ;  esac
  case "$rank" in  4)  dim=', dimension(1,1,1,*)'  ;  esac
  case "$rank" in  5)  dim=', dimension(1,1,1,1,*)'  ;  esac
  case "$rank" in  6)  dim=', dimension(1,1,1,1,1,*)'  ;  esac
  case "$rank" in  7)  dim=', dimension(1,1,1,1,1,1,*)'  ;  esac

  output MPI_Scatterv ${rank} CH "character${dim}"
  output MPI_Scatterv ${rank} L "logical${dim}"
  for kind in $ikinds
  do
    output MPI_Scatterv ${rank} I${kind} "integer*${kind}${dim}"
  done
  for kind in $rkinds
  do
    output MPI_Scatterv ${rank} R${kind} "real*${kind}${dim}"
  done
  for kind in $ckinds
  do
    output MPI_Scatterv ${rank} C${kind} "complex*${kind}${dim}"
  done
done
