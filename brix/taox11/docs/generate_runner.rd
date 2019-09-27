
= BRIX11 generate runtest command

== Collection

taox11

== Usage

  brix11 generate runner [options] [NAME]

=== options

  NAME := name to use for script (no extension); default 'run_test'

  -S, --server=NAME                Defines name to use for server executable (no extension).
                                   Default: server
  -C, --client=NAME                Defines name to use for client executable (no extension).
                                   Default: client

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.


== Description

Generate a standard runner script (Perl) for the TAOX11 test framework capable of running a standard
TAOX11 client/server test/application.

== Example

$ brix11 gen run

Generate a standard runner script named 'run_test.pl' in the current directory.

$ brix11 gen run -N my_test

Generate a standard runner script named 'my_test.pl' in the current directory.

$ brix11 gen run -S bin/myserver -C bin/myclient

Generate a standard runner script names 'run_test.pl' in the current directory where the default
server and client executable filenames are have been replaced by 'bin/myserver' and 'bin/myclient'.

