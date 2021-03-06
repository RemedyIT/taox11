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

my $target = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

$T = $target->CreateProcess ("client");

$test = $T->SpawnWaitKill ($target->ProcessStartWaitInterval());

$target->GetStderrLog();

if ($test != 0) {
    print STDERR "ERROR: Codec test returned $test\n";
    exit 1;
}

exit 0;
