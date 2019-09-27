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

my $target = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

$target->AddLibPath ('.');

$status = 0;

print STDOUT "Reactor test\n\n";

$SV = $target->CreateProcess ("reactor");

$server = $SV->SpawnWaitKill ($target->ProcessStartWaitInterval());

if ($server != 0) {
    print STDERR "ERROR: reactor returned $server\n";
    $status = 1;
}

$target->GetStderrLog();

exit $status;
