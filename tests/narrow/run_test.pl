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

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

exit 0;
