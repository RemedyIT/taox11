# -------------------------------------------------------------------
# @file   run_test.pl
# @author Marcel Smit
#
# @brief  TAOX11 Run test script
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;

my $status =0;
my $server = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";
$SV = $server->CreateProcess ("main");
$test = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());
if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status= 1;
}

exit $status;
