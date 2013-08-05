#!/usr/bin/perl -w
# mpi roll installation test.  Usage:
# mpi.t [nodetype [submituser]]
#   where nodetype is one of "Compute", "Dbnode", "Frontend" or "Login"
#   if not specified, the test assumes either Compute or Frontend.
#   submituser is the login id through which jobs will be submitted to the
#   batch queue; defaults to diag.

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

my @COMPILERS = split(/\s+/, 'intel pgi gnu');
my @NETWORKS = split(/\s+/, 'ib');
my @MPIS = split(/\s+/, 'mpich2 mvapich2 openmpi');

my $modulesInstalled = -f '/etc/profile.d/modules.sh';

# mpi-common.xml
foreach my $mpi (@MPIS) {
  foreach my $compiler (@COMPILERS) {

    SKIP: {

      skip "$mpi/$compiler not installed", 5 if ! -d "/opt/$mpi/$compiler";

      foreach my $network (@NETWORKS) {

        `mkdir $TESTFILE`;
        `cp $TESTFILE.c $TESTFILE`;
        my $EXE = "$TESTFILE.$mpi.$compiler.$network.exe";
        my $OUT = "$TESTFILE.$mpi.$compiler.$network.out";

        my $setup = $modulesInstalled ?
          ". /etc/profile.d/modules.sh; module load $compiler ${mpi}_$network" :
          'echo > /dev/null'; # noop

        my $command = "$setup; which mpicc; cd $TESTFILE;mpicc -o $TESTFILE $TESTFILE.c";
        $output = `$command`;
        $output =~ /(\S*mpicc)/;
        my $mpicc = $1 || 'mpicc';
        my $mpirun = $mpicc;
        $mpirun =~ s/mpicc/mpirun/;
        ok(-x "$TESTFILE/$TESTFILE", "Compile with $mpicc");

        SKIP: {

          skip 'No exe', 1 if ! -x "$TESTFILE/$TESTFILE";
          `mv "$TESTFILE/$TESTFILE" "$TESTFILE/$EXE"`;

          $output = `cd $TESTFILE;. /etc/profile.d/modules.sh;module load $compiler ${mpi}_${network}; $mpirun -np $NODECOUNT ./$EXE`;
          like($output, qr/process $LASTNODE of $NODECOUNT/,"Run with $mpirun");
        }

        `rm -fr $TESTFILE`;

        SKIP: {
          skip 'modules not installed', 3 if ! $modulesInstalled;
          my $dir = "/opt/modulefiles/mpi/.$compiler/${mpi}_$network";
          `/bin/ls $dir/[0-9]* 2>&1`;
          ok($? == 0, "$mpi/$compiler/$network module installed");
          `/bin/ls $dir/.version.[0-9]* 2>&1`;
          ok($? == 0, "$mpi/$compiler/$network version module installed");
          ok(-l "$dir/.version",
             "$mpi/$compiler/$network version module link created");
        }

      }

    }

  }
}

`grep -s 'lib\>' /opt/openmpi/*/*/lib/*.la`;
ok($? != 0, 'references to lib changed to lib64');

# mpi-doc.xml
SKIP: {
  skip 'not server', 1 if $appliance ne 'Frontend';
  ok(-d '/var/www/html/roll-documentation/mpi', 'doc installed');
}

