eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id: run_test.pl 3562 2010-12-08 16:59:01Z twallick $
# -*- perl -*-

use Env (DDS_ROOT);
use lib "$DDS_ROOT/bin";
use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlDDS::Run_Test;
use Sys::Hostname;


my $status = 0;

my $dcpsrepo_ior = "repo.ior";
my $dcpsrepo_port = PerlACE::random_port();

# BuiltIn Topic (bit) support required for ignore_* operations
# simple_tcp.conf contains the instructions to automatically load the SimpleTcp
#  transport.
my $RepoOpts = "-ORBSvcConf simple_tcp.conf -ORBEndpoint iiop://:$dcpsrepo_port";

# simple_tcp.conf contains the instructions to automatically load the SimpleTcp
#  transport.
my $opts = "-ORBSvcConf simple_tcp.conf";

my $DCS_1_Opts = "$opts -n DCS_1 -d files1 -DCPSInfoRepo corbaloc:iiop:localhost:$dcpsrepo_port/DCPSInfoRepo ";
my $DCS_2_Opts = "$opts -n DCS_2 -d files2 -DCPSInfoRepo corbaloc:iiop:localhost:$dcpsrepo_port/DCPSInfoRepo";
my $DCS_3_Opts = "$opts -n DCS_3 -d files3 -f starter.bin -DCPSInfoRepo corbaloc:iiop:localhost:$dcpsrepo_port/DCPSInfoRepo";

my $DCPSREPO = PerlDDS::create_process ("$ENV{DDS_ROOT}/bin/DCPSInfoRepo", $RepoOpts);

my $Node1 = PerlDDS::create_process ("DCS", " $DCS_1_Opts");
my $Node2 = PerlDDS::create_process ("DCS", " $DCS_2_Opts");
my $Node3 = PerlDDS::create_process ("DCS", " $DCS_3_Opts");


sub CleanupOutput {
    unlink $dcpsrepo_ior;
}


sub SpawnWait {
    my $process = shift;
    my $file = shift;
    my $timeout = shift;

    print ">>> " . $process->CommandLine() . "\n";
    $process->Spawn();
    my $ret = PerlACE::waitforfile_timed($file, $timeout);
    if ($ret == -1) {
        print STDERR "ERROR: Cannot find file <$file>\n";
    }
    return $ret;
}




CleanupOutput();


if (SpawnWait($DCPSREPO, $dcpsrepo_ior, 30) != 0) {
    $DCPSREPO->Kill ();
    exit 1;
}

#print STDERR "\n\npress return key to shutdown server\n\n";
#my $userinput =  <STDIN>;


print $Node1->CommandLine() . "\n";
$Node1->Spawn ();
sleep 1;

print $Node2->CommandLine() . "\n";
$Node2->Spawn ();
sleep 1;

print $Node3->CommandLine() . "\n";
$Node3->Spawn ();

$Node1Result = $Node1->WaitKill (60);
if ($Node1Result != 0) {
    print STDERR "ERROR: node 1 returned $Node1Result \n";
    $status = 1;
}

$Node2Result = $Node2->WaitKill (15);
if ($Node2Result != 0) {
    print STDERR "ERROR: node 2 returned $Node2Result \n";
    $status = 1;
}

$Node3Result = $Node3->WaitKill (15);
if ($Node3Result != 0) {
    print STDERR "ERROR: node 3 returned $Node3Result \n";
    $status = 1;
}

$DCPSREPO->Kill();

CleanupOutput();

if ($status == 0) {
  print "test PASSED.\n";
} else {
  print STDERR "test FAILED.\n";
}

exit $status;
