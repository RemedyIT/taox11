
= BRIX11 generate mpc command

== Collection

taox11

== Usage

  brix11 generate mpc [options] [NAME]

=== options

  NAME := name to use for project (MPC) file; default 'test'

  -I, --with-idl=[FILE]            Include IDL generation subproject for FILE.
                                   Specify filenames without extension. Separate with ',' when more than one.
                                   Default: Generate IDL generation subproject for IDL files in working dir
      --without-idl                Do NOT generate IDL generation subproject.
                                   Default: Generate IDL generation subproject
  -C, --with-client=[FILES]        Include CORBA client subproject with FILES (do not specify IDL generated file).
                                   Specify filenames without extension. Separate with ',' when more than one.
                                   Default: Generate client subproject with 'client')
      --without-client             Do NOT generate CORBA client subproject.
                                   Default: Generate client subproject
  -S, --with-server=[FILES]        Include CORBA server subproject with FILES (do not specify IDL generated files).
                                   Specify filenames without extension. Separate with ',' when more than one.
                                   By default FILES will be set to 'server' + an '<idl>_impl' for all IDL files.
                                   Default: Generate server subproject.
      --without-server             Do NOT generate CORBA server subproject.
                                   Default: Generate server subproject

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.


== Description

Generate a project definition (MPC) file for a standard TAOX11 CORBA (test) project.

By default the command will generate:
  - an IDL generation subproject for any IDL file in the project directory OR (if there
    does not yet exist any IDL file there) a single IDL file with the same name as the
    project (plus '.idl' extension)
  - a TAOX11 client executable subproject with a source file named 'client.cpp'
  - a TAOX11 server executable subproject with a main source file named 'server.cpp' and
    one servant implementation source file for each IDL file from the IDL subproject
    named '<idlfile>_impl.cpp'

The available options allow you to manipulate the generated content in various ways.

== Example

$ brix11 gen mpc

Generates a standard MPC file 'test.mpc' with standard IDL, client and server subproject.

$ brix11 gen mpc --without-client

Generates a standard MPC file 'test.mpc' with standard IDL and server subproject but without
a client executable subproject.

$ brix11 gen mpc -S myserver hello

Generates an MPC file named 'hello.mpc' with standard IDL and client subprojects and a server
executable subproject with a source file named 'myserver.cpp' and one servant implementation
source file for each IDL file.
