
= BRIX11 generate idl command

== Collection

taox11

== Usage

  brix11 generate idl [options] [NAME]

=== options

  NAME := name to use for IDL file; default 'test'

  -i, --interface=NAME             Defines name to use for interface.
                                   Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing module(s).
                                   Default: derived from IDL file basename with first character uppercased
      --without-shutdown           Do NOT generate a shutdown method.
                                   Default: generate shutdown.

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.


== Description

Generates an IDL file with a single IDL 'interface' definition.

By default the interface will have a single operation defined:

  - a void oneway method with no arguments named 'shutdown'.

The available options allow you to control the name and scoping of the interface as well as
control the generation of the 'shutdown' operation.

== Example

$ brix11 gen idl

Generates a standard IDL file 'test.idl' with an interface definition for an interface named
Test having a standard shutdown operation.

$ brix11 gen id --without-shutdown hello

Generates an IDL file 'hello.idl' with an interface definition for an interface named
Hello without any operation (or other definitions).

$ brix11 gen idl -i MyModule::MyInterface hello

Generates an IDL file named 'hello.idl' with a module definition named MyModule with an
interface definition in it's scope named MyInterface having a standard shutdown operation.
