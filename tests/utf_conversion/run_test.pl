# @copyright Copyright (c) Remedy IT Expertise BV

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $test = $target->CreateProcess ("utf_conversion");
my $status = $test->SpawnWaitKill ($target->ProcessStartWaitInterval ());
if ($status != 0) {
    print STDERR "ERROR: utf_conversion returned $status\n";
    exit 1;
}
exit 0;
