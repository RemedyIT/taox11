#---------------------------------------------------------------------
# @file   run_test.pl
# @author Marijke Hengstmengel
#
# @copyright Copyright (c) Remedy IT Expertise BV
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$type = "";

my $server = PerlACE::TestTarget::create_target (2) || die "Create target 1 failed\n";

sub run_test
{
    my $type = shift(@_);

   $BT = $server->CreateProcess ("basic_test", "-t $type");
    my $basictest = $BT->SpawnWaitKill ($server->ProcessStartWaitInterval());

    if ($basictest != 0) {
        print STDERR "ERROR: basic test for ($type) returned $basictest\n";
        $status = 1;
    }
}

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "Run_Test Perl script for DynAny Test\n\n";
        print "run_test [-t type]\n";
        print "\n";
        print "-t type             -- runs only one type of dynany test\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-t") {
        $type = $ARGV[$i + 1];
        $i++;
    }
}

@types = ("dynany", "dynarray", "dynenum", "dynsequence", "dynstruct",
          "dynunion","dynvalue");


if ($type ne "") {
    run_test ($type);
}
else {
    foreach $type (@types) {
        run_test ($type);
    }
}

exit $status;

