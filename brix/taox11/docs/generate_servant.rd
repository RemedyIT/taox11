
= BRIX11 generate servant command

== Collection

taox11

== Usage

  brix11 generate servant [options]

=== options

  -I, --with-idl=[FILE]            Generate servant implementation(s) from IDL file(s).
                                   Specify filename without extension. Separate with ',' when more than one.
                                   Default: all IDL files in working dir
      --svt-pfx=POSTFIX            Defines postfix to use for generated servant implementation filenames
                                   and classes.
                                   Default: '_impl'
      --shutdown-on=NAME           Restrict shutdown implementations to interface NAME.
                                   Use scoped name (i.e. <name>::[<name>::]::<name>) to specify enclosing module(s).
                                   Separate with ',' when more than one.
                                   Default: all interfaces having shutdown method.
      --without-shutdown           Do not generate any shutdown implementations.
                                   Default: generate implementations.
      --gen-skel                   Also (re)generate servant skeleton (*S.{h,cpp}) files.
                                   Default: do NOT generate skeleton.

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.


== Description

Generates a standard servant implementation header and source file for a TAOX11 CORBA test/application
for each IDL file.

By default the files will contain declarations and code:

  - for servant implementation classes for each interface declared in the IDL file
  - for a void oneway 'shutdown' method implementation on every servant class that
    implements an interface defining a shutdown operation
  - for a member attribute holding an ORB reference used for 'shutdown' implementation

The available options allow you to control the IDL files to use, the naming of implementation files,
additional skeleton generation as well as shutdown implementation.

== Example

$ brix11 gen svt

Generates standard implementation files for each IDL file in the project directory with 'shutdown'
implementations on any implementation class implementing an interface declaring the operation.

$ brix11 gen svt -I special_test --svt-prefix _svt

Generates implementation files 'special_test_svt.{h,cpp}' with implementation class names post fixed
with '_svt' and 'shutdown' implementations on any implementation class implementing an interface
declaring the operation.

$ brix11 gen svt --shutdown-on MyModule::MyInterface_impl -I hello

Generates implementation files 'hello_impl.{h,cpp}' with implementation class names post fixed with
'_impl' and a 'shutdown' implementation only on the implementation class 'MyModule::MyInterface_impl'
if it implements an interface declaring that operation.
