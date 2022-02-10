
= BRIX11 generate client command

== Collection

taox11

== Usage

  brix11 generate client [options] [NAME]

=== options

  NAME := name to use for client main source code file; default 'client'

  -I, --with-idl=[FILE]            Create includes for IDL generated stub headers from FILE.
                                   Specify filename without extension. Separate with ',' when more than one.
                                   Default: Generate includes for IDL files in working dir
  -i, --interface=NAME             Defines name of client interface stub.
                                   Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing module(s).
                                   Default: derived from first IDL file basename (uppercasing first character)
      --without-shutdown           Do NOT generate code to shutdown server.
                                   Default: generate shutdown.

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.


== Description

Generates a standard client main source file for a TAOX11 CORBA test/application.

By default the file will contain code:

  - to include an IDL generated stub header for each IDL file
  - for basic argument handling
  - for ORB startup
  - for servant IOR resolving
  - for servant shutdown

The available options allow you to control the IDL files to use, the name and scoping of the interface
as well as control the generation of the 'shutdown' request.

== Example

$ brix11 gen cli

Generates a standard client main file 'client.cpp' using any IDL file in the project directory,
attempting to resolve a servant IOR to an interface with name derived from the first IDL file and
issuing a standard 'shutdown' request.

$ brix11 gen cli -I special_test --without-shutdown special_client

Generates a client main file 'special_client.cpp' including 'special_testC.h' attempting to
resolve a servant IOR to the interface 'Special_test' without standard 'shutdown'request.

$ brix11 gen cli -i MyModule::MyInterface -I hello

Generates a client main file 'client.cpp' including 'helloC.h' attempting to resolve a servant
IOR to the interface 'MyModule::MyInterface' and issuing a standard 'shutdown' request.
