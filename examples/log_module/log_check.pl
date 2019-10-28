#---------------------------------------------------------------------
# @file   log_check.pl
# @author Marcel Smit
#
# @copyright Copyright (c) Remedy IT Expertise BV
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

$status = 0;

$trace = 0;
$debug = 0;
$info = 0;
$warning = 0;
$error = 0;
$critical = 0;
$panic = 0;
$category = 0;

open FILE, "<output";
while (my $line = <FILE>) {
    $err_line = 0;
    if ($line =~ "MY_LOG_CATEGORY") {
        $category = $category+1;
    }
    if ($line =~ "LP_TRACE") {
        $trace = $trace+1;
    }
    if ($line =~ "LP_DEBUG") {
        $debug = $debug+1;
    }
    if ($line =~ "LP_INFO") {
        $info = $info+1;
    }
    if ($line =~ "LP_WARNING") {
        $warning = $warning+1;
    }
    if ($line =~ "LP_ERROR") {
        $err_line = 1;
        $error = $error+1;
    }
    if ($line =~ "LP_CRITICAL") {
        $critical = $critical+1;
    }
    if ($line =~ "LP_PANIC") {
        $panic = $panic+1;
    }

    if ($err_line == 0) {
       print $line;
    }
}

if ($trace == 1) {
    print "OK: Exactly one trace message found.\n";
}
else {
    print STDERR "ERROR: Unexpected number of trace messages found: expected <1> - found <$trace>\n";
    $status = $status+1;
}

if ($debug == 2) {
    print "OK: Exactly two debug messages found.\n";
}
else {
    print STDERR "ERROR: Unexpected number of debug messages found: expected <2> - found <$debug>\n";
    $status = $status+1;
}

if ($info == 1) {
    print "OK: Exactly one info message found.\n";
}
else {
    print STDERR "ERROR: Unexpected number of info messages found: expected <1> - found <$info>\n";
    $status = $status+1;
}

if ($warning == 2) {
    print "OK: Exactly two warning messages found.\n";
}
else {
    print STDERR "ERROR: Unexpected number of warning messages found: expected <2> - found <$warning>\n";
    $status = $status+1;
}

if ($error == 1) {
    print "OK: Exactly one -err- message found.\n";
}
else {
    print STDERR "ERROR: Unexpected number of error messages found: expected <1> - found <$error>\n";
    $status = $status+1;
}

if ($critical == 1) {
    print "OK: Exactly one critical message found.\n";
}
else {
    print STDERR "ERROR: Unexpected number of critical messages found: expected <1> - found <$critical>\n";
    $status = $status+1;
}

if ($panic == 1) {
    print "OK: Exactly one panic message found.\n";
}
else {
    print STDERR "ERROR: Unexpected number of panic messages found: expected <1> - found <$panic>\n";
    $status = $status+1;
}

if ($category == 9) {
    print "OK: All messages logged the category.\n";
}
else {
    print STDERR "ERROR: Not all messages logged the category : expected <9> - found <$category>\n";
    $status = $status+1;
}

exit $status;
