#---------------------------------------------------------------------
# @file   run_test.pl
# @author Mark Drijver
#
# @copyright Copyright (c) Remedy IT Expertise BV
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

print "IDL test\n";

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;

my $status =0;
my $server = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

# The location of the ridl utility
my $ridl = "$ENV{X11_BASE_ROOT}/bin/ridlc";

unless (-e $ridl) {
  die "ERROR: Cannot find " . $ridl;
}

opendir(DIRH, ".") or die "Could not open dir. $!";
@files = grep(/\.idl$/,readdir(DIRH));
foreach (sort(@files)){
  print "Testing ", $_, ": ";
  my $input_file = $_;

  #Redirect the screen output to the null device.
  open (OLDOUT, ">&STDOUT");
#  open (STDOUT, ">" . File::Spec->devnull());
  open (STDOUT, ">" . output);
  open (OLDERR, ">&STDERR");
  open (STDERR, ">&STDOUT");

  # Compile the IDL
  system ("$ridl", "$input_file");

  #Redirect the null device output back to the screen
  open (STDOUT, ">&OLDOUT");
  open (STDERR, ">&OLDERR");

  $found = 0;
  open FILE, "<output";
  while (my $line = <FILE>)
  {
    #print $line;
    if ($line =~ "IDL::Parse")
    {
      $found = 1;
      close FILE;
      last;
    }
  }
  #print "\nFound: " . $found;
  if ($found>0) {
    print "parsing failed as expected.\n";
  } else {
    print STDERR "ERROR: Unable to detect expected ridlc error.\n";
    $status = 1;
  }

}

exit $status;
