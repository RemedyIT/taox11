
= BRIX11 generate test command

== Collection

taox11

== Usage

  brix11 generate test [options] NAME

=== options

  NAME := Name to use for test (MPC base and folder). If NAME == '.' than
          create project in working dir with default name 'test'.

  -S, --server=NAME                Defines name to use for server executable (no extension).
                                   Default: server
  -C, --client=NAME                Defines name to use for client executable (no extension).
                                   Default: client
  -i, --interface=NAME             Defines name to use for test interface.
                                   Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing
                                   module(s).
                                   Default: MyTest
      --no-folder                  Do NOT create a test folder but create test in working directory.
                                   Default: create test folder under working directory.
      --chdir=PATH                 Change working directory to PATH (will create folder if not existing).
                                   Default: '.'
  -B, --with-build                 Run make or build process after successfully having generated project.
                                   Default: no build

  -f, --force                      Force all tasks to run even if their dependencies do not require them to.
                                   Default: off
  -v, --verbose                    Run with increased verbosity level. Repeat to increase more.
                                   Default: 0

  -h, --help                       Show this help message.

== Description

Generates a CORBA (test) project for TAOX11 according to a format that is similar to the TAOX11
regression tests.
That means that by default a project is generated including:

  - an IDL file ('NAME.idl') with an interface providing a *oneway* *shutdown* method
  - a client main source file ('client.cpp' by default) including basic argument handling, ORB startup,
    IOR resolving and servant shutdown
  - a server main source file ('server.cpp' by default) including basic argument handling, servant creation
    and activation, IOR file output and ORB running
  - a servant implementation header and source file ('NAME_impl.{h,cpp}') )based on the generated IDL file
    including implemented support for shutdown handling
  - an MPC project file ('NAME.mpc') defining an IDL generation subproject, client application subproject and
    server application subproject

The intent is to generate a full, basic, project implementation that can be generated, compiled and
executed without errors.
This base can than be used to extend for your own use.

Various options exist to be able to vary

== Example

$ brix11 gen test hello

Generate project hello in subdirectory ./hello.

$ brix11 gen test --no-folder my_project

Generate project my_project in current directory.

$ brix11 gen test --chdir=mytest --no-folder another_test

Generate project another_test in directory ./mytest.
