#---------------------------------------------------------------------
# @file   run_test.pl
# @author Marcel Smit
#
# @copyright Copyright (c) Remedy IT Expertise BV
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$test_not_found = 0;
$test_null_locator = 0;
$test_runtime = 10; # sec
if ($PerlACE::Process::WAIT_DELAY_FACTOR > 0) {
    $test_runtime *= $PerlACE::Process::WAIT_DELAY_FACTOR;
}

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
        $ENV{TAOX11_LOG_MASK}='all';
    }
    elsif ($i eq '-test_exception') {
        $test_not_found = 1;
    }
    elsif ($i eq '-test_null_locator') {
        $test_null_locator = 1;
    }
}

my $server = PerlACE::TestTarget::create_target(2) || die "Create failed\n";
my $client = PerlACE::TestTarget::create_target(3) || die "Create failed\n";

my $TARGETHOSTNAME = $client->HostName ();
my $port = $client->RandomPort ();

my $iorbase = "test.ior";
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level ".
                              "-ORBListenEndpoints iiop://$TARGETHOSTNAME:$port ".
                              "-t $test_not_found -n $test_null_locator -r $test_runtime");
if ($test_null_locator == 1) {
    $test_not_found = 1;
}
$CL = $client->CreateProcess ("client",
                              "-ORBdebuglevel $debug_level ".
                              "-a corbaloc:iiop:$TARGETHOSTNAME:$port/SIMPLE_TEST_KEY ".
                              "-t $test_not_found");
$SV->Spawn ();

sleep(3);

$CL->Spawn ();

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
