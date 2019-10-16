#---------------------------------------------------------------------
# @file   run_test.pl
# @author Marijke Hengstmengel
#
# @copyright Copyright (c) Remedy IT Expertise BV
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";
my $client1 = PerlACE::TestTarget::create_target(3) || die "Create target 3 failed\n";
my $client2 = PerlACE::TestTarget::create_target(4) || die "Create target 4 failed\n";


$server_debug_level = '0';
$client_debug_level = '0';

foreach $i (@ARGV) {
if ($i eq '-debug') {
        $server_debug_level = '10';
        $client_debug_level = '10';
    }
}

$client1_conf = $client1->LocalFile ($conf_file);
$client2_conf = $client2->LocalFile ($conf_file);

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client2_iorfile = $client2->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $server_debug_level -o $server_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot get file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client1->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client1_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client2->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client2_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
$CL1 = $client1->CreateProcess ("client1",
                              "-ORBdebuglevel $client_debug_level"
                              . " -k file://$client1_iorfile ");

$CL2 = $client2->CreateProcess ("client2",
                              "-ORBdebuglevel $client_debug_level"
                              . " -k file://$client2_iorfile ");


$client1_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval());

if ($client1_status != 0) {
    print STDERR "ERROR: client returned $client1_status\n";
    $status = 1;
}

$client2_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval());

if ($client2_status != 0) {
    print STDERR "ERROR: client returned $clien2_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);

exit $status;
