#-------------------------------------------------------------------
# @file   run_test.pl
# @author Marcel Smit
#
# @copyright Copyright (c) Remedy IT Expertise BV
#-------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

$TAO_ROOT = "$ENV{'TAO_ROOT'}";

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

# naming service
$NS = 0;
# client
$CL = 0;
#servers
$SV1 = 0;
$SV2 = 0;

my $libpath = "$ENV{TAOX11_ROOT}/../lib";

# init client target
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
$client->AddLibPath ($libpath);

#init server targets
my $server1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
$server1->AddLibPath ($libpath);
my $server2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
$server2->AddLibPath ($libpath);

my $tg_naming = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";


my $ior_nsbase = "ns.ior";
my $ior_nsfile = $tg_naming->LocalFile ($ior_nsbase);
my $client_iorfile = $client->LocalFile ($ior_nsbase);
my $server1_iorfile = $server1->LocalFile ($ior_nsbase);
my $server2_iorfile = $server2->LocalFile ($ior_nsbase);

$tg_naming->DeleteFile ($ior_nsbase);
$client->DeleteFile ($ior_nsbase);
$server1->DeleteFile ($ior_nsbase);
$server2->DeleteFile ($ior_nsbase);

print "Starting Naming Service\n";

$NS = $tg_naming->CreateProcess ("$TAO_ROOT/orbsvcs/Naming_Service/tao_cosnaming", "-m 0 -ORBEndpoint iiop://localhost:60003 -o $ior_nsfile");
$NS->Spawn ();

if ($tg_naming->WaitForFileTimed ($ior_nsbase,
                                  $tg_naming->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($tg_naming->GetFile ($ior_nsbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ior_nsfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($ior_nsbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($server1->PutFile ($ior_nsbase) == -1) {
    print STDERR "ERROR: cannot set file <$server1_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($server2->PutFile ($ior_nsbase) == -1) {
    print STDERR "ERROR: cannot set file <$server2_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}


# Set up NamingService environment
$ENV{"NameServiceIOR"} = "corbaloc:iiop:localhost:60003/NameService";


print "Start servers\n";
# start server 1
$SV1 = $server1->CreateProcess ("server", "-k file://$server1_iorfile -n SERVER_1");
$server1_status = $SV1->Spawn ();

if ($server1_status != 0) {
    print STDERR "ERROR: server 1 returned $server1_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# start server 2
$SV2 = $server2->CreateProcess ("server", "-k file://$server2_iorfile -n SERVER_2");
$server2_status = $SV2->Spawn ();

if ($server2_status != 0) {
    print STDERR "ERROR: Server 2 returned $server2_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

print "Start client\n";
sleep (5);
# start client
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile");
$status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($status != 0) {
    print STDERR "ERROR: client returned $status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$server1_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());

if ($server1_status != 0) {
    print STDERR "ERROR: server1 returned $server1_status\n";
    $status = 1;
}

$server2_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server2_status != 0) {
    print STDERR "ERROR: server1 returned $server2_status\n";
    $status = 1;
}

print "Stopping naming server\n";
$NS->Kill (); $NS->TimedWait (1);

$tg_naming->DeleteFile ($ior_nsbase);
$client->DeleteFile ($ior_nsbase);
$server1->DeleteFile ($ior_nsbase);
$server2->DeleteFile ($ior_nsbase);

exit $status;
