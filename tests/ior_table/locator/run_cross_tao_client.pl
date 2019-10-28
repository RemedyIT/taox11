#---------------------------------------------------------------------
# @file   run_cross_tao_client.pl
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

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
        $ENV{TAOX11_LOG_MASK}='all';
    }
}

my $server = PerlACE::TestTarget::create_target(2) || die "Create failed\n";
my $client = PerlACE::TestTarget::create_target(3) || die "Create failed\n";

my $TARGETHOSTNAME = $client->HostName ();
my $port = $client->RandomPort ();

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -ORBListenEndpoints iiop://$TARGETHOSTNAME:$port");
$CL = $client->CreateProcess ("tao/client",
                              "-ORBdebuglevel $debug_level ".
                              "-a corbaloc:iiop:$TARGETHOSTNAME:$port/SIMPLE_TEST_KEY");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sleep(3);

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

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
