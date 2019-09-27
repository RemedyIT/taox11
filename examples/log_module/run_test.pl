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
use lib ".";
use PerlACE::TestTarget;

my $log_test = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

#Redirect the screen output to the null device.
open (OLDOUT, ">&STDOUT");
open (STDOUT, ">" . output);
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT");

$SV = $log_test->CreateProcess ("test_my_log_module");
$log_test_status = $SV->SpawnWaitKill ($log_test->ProcessStopWaitInterval ());

if ($log_test_status != 0) {
    print STDERR "ERROR: log_test returned $log_test_status\n";
    exit 1;
}

#Redirect the null device output back to the screen
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

$log_test_res = system("perl log_check.pl");
if ($log_test_res != 0) {
    print STDERR "ERROR: log_test_res returned $log_test_res\n";
    exit 1;
}

exit $log_test_status;
