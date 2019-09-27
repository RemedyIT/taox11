
= BRIX11 run list command

== Collection

taox11

== Usage

  brix11 run list [options] [-- test-options]

=== options

  (any test-options following '--' will be passed on unchecked)

  -l, --list=PATH                  Specifies regression test list to feed auto_run_test.
                                   Default: none
  -r, --root=PATH                  Specifies root directory for running the tests.
                                   Default: <listfile location>
  -s, --sandbox=SANDBOX            Specifies sandbox for running each separate test.
                                   Default: none

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.


== Description

Executes the 'auto_run_tests' script which will run the tests from the specified list
according to the test-options passed.
BRIX11 will automatically pass all test configs to the 'auto_run_tests' script.

== Example

$ brix11 run list -l /taox11/root/bin/taox11_tests.lst -r /taox11/root

Executes the auto_run_tests script with the TAOX11 list.

$ brix11 run list -l /my/location/mylist.lst -s sandbox

Executes the auto_run_tests script with the specified list at the lists location (/my/location)
using the 'sandbox' sandbox command.
