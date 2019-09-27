
= BRIX11 generate server command

== Collection

taox11

== Usage

  brix11 generate server [options] [NAME]

=== options

  NAME := name to use for server main source code file; default 'server'

  -I, --with-idl=[FILE]            Create includes for IDL generated stub headers from FILE.
                                   Specify filename without extension. Separate with ',' when more than one.
                                   Default: Generate includes for IDL files in working dir
  -i, --interface=NAME             Defines name of interface for which to activate a servant.
                                   Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing module(s).
                                   Default: derived from first IDL file basename (uppercasing first character)
  -S, --with-servant=[FILES]       Create includes for CORBA servant implementation headers.
                                   Specify filenames without extension. Separate with ',' when more than one.
                                   Default: Generate include for each '<idlfile>_impl.h'.
  -s, --servant=NAME               Defines name of servant implementation for which to create a servant.
                                   Use scoped name (i.e. <name>::[<name>::]::<name>) to specify enclosing module(s).
                                   Default: <interface>_impl
      --without-servant            Do NOT generate servant creation and activation (also suppresses implementation headers).
                                   Default: Generate servant creation/activation

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.


== Description

Generates a standard server main source file for a TAOX11 CORBA test/application.

By default the file will contain code:

  - to include an IDL generated servant skeleton header for each IDL file
  - a servant implementation header for each IDL file
  - for basic argument handling
  - for ORB startup
  - for servant creation and activation
  - for IOR file storage
  - for ORB running

The available options allow you to control the IDL files to use, the name and scoping of the interface
as well as the servant implementations to use.

== Example

$ brix11 gen srv

Generates a standard server main file 'server.cpp' using any IDL file in the project directory,
attempting to create and activate a servant implementation with name and implemented interface
derived from the first IDL file.

$ brix11 gen srv -I special_test --servant MySpecial_impl special_server

Generates a server main file 'special_server.cpp' including 'special_testS.h' and
'special_test_impl.h' attempting to create and activate servant implementation 'MySpecial_impl'
implementing interface 'Special_test'.

$ brix11 gen srv -i MyModule::MyInterface -I hello

Generates a server main file 'server.cpp' including 'helloS.h' and 'hello_impl.h' attempting to
create and activate servant implementation 'MyModule::MyInterface_impl' implementing interface
'MyModule::MyInterface'.
