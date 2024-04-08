#---------------------------------------------------------------------
# @file   run_test.pl
# @author Johnny Willemsen
#
# @copyright Copyright (c) Remedy IT Expertise BV
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $status = 0;
my $debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target(3) || die "Create target 3 failed\n";

my $iorbase = "test1.ior";
my $iorbase2 = "test2.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
my $server_iorfile2 = $server->LocalFile ($iorbase2);
my $client_iorfile2 = $client->LocalFile ($iorbase2);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$server->DeleteFile($iorbase2);
$client->DeleteFile($iorbase2);
$server->SetEnv ("X11_VERBOSE", "prio|thread|process|datetime");
$client->SetEnv ("X11_VERBOSE", "prio|thread|process|datetime");

my $SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -ORBServerId testserver");
my $CL = $client->CreateProcess ("client", "-ORBdebuglevel $debug_level");
my $server_status = $SV->Spawn ();

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
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase2) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile2>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase2) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile2>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

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
$server->DeleteFile($iorbase2);
$client->DeleteFile($iorbase2);

exit $status;
