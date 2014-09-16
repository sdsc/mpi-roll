#!/usr/bin/perl -w
# mpi roll installation test.  Usage:
# mpi.t [nodetype]
#   where nodetype is one of "Compute", "Dbnode", "Frontend" or "Login"
#   if not specified, the test assumes either Compute or Frontend.

use Test::More qw(no_plan);

my $appliance = $#ARGV >= 0 ? $ARGV[0] :
                -d '/export/rocks/install' ? 'Frontend' : 'Compute';
my $installedOnAppliancesPattern = '.';
my $output;

my $TESTFILE = 'tmpmpi';
my $NODECOUNT = 4;
my $LASTNODE = $NODECOUNT - 1;

open(OUT, ">$TESTFILE.c");
print OUT <<END;
#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Hello from process %d of %d\\n", rank, size);
  MPI_Finalize();
  return 0;
}
END
close(OUT);

my @COMPILERS = split(/\s+/, 'ROLLCOMPILER');
my @NETWORKS = split(/\s+/, 'ROLLNETWORK');
my @MPIS = split(/\s+/, 'ROLLMPI');

# mpi-common.xml
foreach my $mpi (@MPIS) {
  foreach my $compiler (@COMPILERS) {

    my $compilername = (split('/', $compiler))[0];

    SKIP: {

      skip "$mpi/$compilername not installed", 5
        if ! -d "/opt/$mpi/$compilername";

      foreach my $network (@NETWORKS) {

        my $command = "module load $compiler ${mpi}_$network; which mpicc; mpicc -o $TESTFILE.exe $TESTFILE.c";
        $output = `$command`;
        $output =~ /(\S*mpicc)/;
        my $mpicc = $1 || 'mpicc';
        my $mpirun = $mpicc;
        $mpirun =~ s/mpicc/mpirun/;
        ok(-x "$TESTFILE.exe", "Compile with $mpicc");

        SKIP: {

          skip 'No exe', 1 if ! -x "$TESTFILE.exe";

          open(OUT, ">$TESTFILE.sh");
          print OUT <<END;
#!/bin/bash
module load $compiler ${mpi}_$network
$mpirun -np $NODECOUNT ./$TESTFILE.exe
END
          close(OUT);
          $output = `bash $TESTFILE.sh 2>&1`;
          like($output, qr/process $LASTNODE of $NODECOUNT/,"Run with $mpirun");

        }

        `rm -f $TESTFILE.exe`;

        SKIP: {
          my $dir = "/opt/modulefiles/mpi/.$compilername/${mpi}_$network";
          `/bin/ls $dir/[0-9]* 2>&1`;
          ok($? == 0, "$mpi/$compilername/$network module installed");
          `/bin/ls $dir/.version.[0-9]* 2>&1`;
          ok($? == 0, "$mpi/$compilername/$network version module installed");
          ok(-l "$dir/.version",
             "$mpi/$compilername/$network version module link created");
        }

      }

    }

  }
}

`grep -s 'lib\>' /opt/openmpi/*/*/lib/*.la`;
ok($? != 0, 'references to lib changed to lib64');

`rm -fr $TESTFILE*`;
