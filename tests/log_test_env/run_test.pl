#---------------------------------------------------------------------
# @file   run_test.pl
# @author Marijke Hengstmengel
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

$category = 'TAOX11';

my $log_test = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

#These environment variable's belong to the test scenario, changing these
#values causes errors in the test. So don't change without changing
#log_test.cpp .
$log_test->SetEnv ("X11_LOG_MASK", 'DEBUG');
# these should overwrite X11_LOG_MASK
$log_test->SetEnv ("TAOX11_LOG_MASK", 'INFO|WARNING');
$log_test->SetEnv ("TESTX11_LOG_MASK", 'INFO|WARNING');

$log_test->SetEnv ('X11_VERBOSE', 'TIME');
# these should overwrite X11_VERBOSE
$log_test->SetEnv ('TAOX11_VERBOSE', 'CATEGORY|PRIO');
$log_test->SetEnv ('TESTX11_VERBOSE', 'CATEGORY|PRIO');

# redirect X11 logs to file
$log_test->SetEnv ("X11_LOG_OUTPUT", "FILE=output,,,T");

$SV = $log_test->CreateProcess ("log_test");
$log_test_status = $SV->Spawn ();

if ($log_test_status != 0) {
    print STDERR "ERROR: log_test returned $log_test_status\n";
    exit 1;
}

$log_test_status = $SV->WaitKill ($log_test->ProcessStopWaitInterval());

if ($log_test_status != 0) {
    print STDERR "ERROR: log_test returned $log_test_status\n";
    exit 1;
}

if ($log_test->GetFile ('output.log') == -1) {
    print STDERR "ERROR: cannot retrieve file <output.log>\n";
    exit 1;
}
$log_test_res = system("perl $ENV{'TAOX11_ROOT'}/tao/x11/logger/log_check.pl $category output.log");
if ($log_test_res != 0) {
    print STDERR "ERROR: log_test returned $log_test_res\n";
    exit 1;
}
exit $log_test_status;
