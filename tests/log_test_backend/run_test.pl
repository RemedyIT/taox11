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

# redirect TESTX11 logs to a custom backend and stderr
$target->SetEnv ("TESTX11_LOG_OUTPUT", "CUSTOM|STDERR");
$target->SetEnv ("TESTX11_LOG_MASK", "ALL");

$status = 0;

$SV = $target->CreateProcess ("log_test_backend");

$server = $SV->SpawnWaitKill ($target->ProcessStartWaitInterval());

if ($server != 0) {
    print STDERR "ERROR: log_test_backend returned $server\n";
    $status = 1;
}

$target->GetStderrLog();

exit $status;
