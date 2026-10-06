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

my $target = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";
my $test = $target->CreateProcess ("argument_storage", "");

if ($test->SpawnWaitKill ($target->ProcessStartWaitInterval ()) != 0) {
    print STDERR "ERROR: test returned non-zero\n";
    exit 1;
}

exit 0;
