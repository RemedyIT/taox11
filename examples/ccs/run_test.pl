#---------------------------------------------------------------------
# @file   run_test.pl
# @author Marcel Smit
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

$TAO_ROOT = "$ENV{'TAO_ROOT'}";

$status = 0;
$startdir = getcwd();

$quiet = 0;

# check for -q flag
if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

$debug_level = 0;
$cdebug_level = 0;
$nsdebug_level = 0;


my $nstest = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$iorfile = "ns.ior";
my $nstest_iorfile = $nstest->LocalFile ($iorfile);

$my_ip = $nstest->IP_Address ();

sub name_server
{
    my $args = "-ORBEndpoint iiop://$my_ip:60003 -ORBdebuglevel $nsdebug_level -o $nstest_iorfile -m 0";
    my $prog = "$TAO_ROOT/orbsvcs/Naming_Service/tao_cosnaming";

    $NS = $nstest->CreateProcess ("$prog", "$args");

    $nstest->DeleteFile($iorfile);

    $NS->Spawn ();

    if ($nstest->WaitForFileTimed ($iorfile,
                                   $nstest->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$nstest_iorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }

    if ($nstest->GetFile ($iorfile) == -1) {
        print STDERR "ERROR: cannot retrieve file <$nstest_iorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
    if ($client->PutFile ($iorfile) == -1) {
        print STDERR "ERROR: cannot set file <$nstest_iorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
    if ($server->PutFile ($iorfile) == -1) {
        print STDERR "ERROR: cannot set file <$nstest_iorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
}


# create naming service, server, client
name_server ();

my $srv_iorfile = $server->LocalFile ($iorfile);
$SV = $server->CreateProcess ("$startdir/server", "-ORBInitRef NameService=file://$srv_iorfile -ORBdebuglevel $debug_level");
my $cli_iorfile = $client->LocalFile ($iorfile);
$CL = $client->CreateProcess ("$startdir/client", "-ORBInitRef NameService=file://$cli_iorfile -ORBdebuglevel $cdebug_level");
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

sleep(3);

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$SV->Kill ();

$NS->Kill ();

$server->DeleteFile($iorfile);
$client->DeleteFile($iorfile);
$nstest->DeleteFile($iorfile);

exit $status;
