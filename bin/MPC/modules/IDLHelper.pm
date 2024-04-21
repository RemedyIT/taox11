package IDLHelper;
# -------------------------------------------------------------------
# @file   IDLHelper.pm
# @author Marijke Hengstmengel
#
# @brief  TAOX11 IDLHelper
#
# @copyright Copyright (c) Remedy IT Expertise BV
# ------------------------------------------------------------------
use strict;
use File::Spec;
use CommandHelper;
our @ISA = qw(CommandHelper);

sub get_output {
  my($self, $file, $flags) = @_;
  my @out;

  # handle generated executor files (if any)
  if ($flags =~ /-Gexr?/ && !($flags =~ /-oN/)) {
    my $gendir = '';

    # need to determine the location of the generated executor
    # starter code relative to the gendir directive in the MPC
    # file (MPC flaw).
    #
    # For example:
    # - IDL + MPC + component executor starter code in ./src/sender (-oI .)
    # - gendir = ../../build/sender
    #
    # The result should look something like:
    # ../../build/sender/../../src/sender/xxx_exec.h/cpp

    if ($flags =~ /-oI\s*(\S+)/) {
      my $exec_gendir = File::Spec->rel2abs($1);
      if ($flags =~ /-o\s+(\S+)/) {
        $gendir = $1;
      } elsif ($flags =~ /-o([^I]\S+)/) {
        $gendir = $1;
      } else {
        $gendir = '.';
      }
      $gendir = File::Spec->rel2abs($gendir);
      $gendir = File::Spec->abs2rel($exec_gendir, $gendir);
      if (!($gendir =~ /(\\|\/)$/)) {
        $gendir .= '/';
      }
    }
    $file =~ /^(.*)(\.p?idl)$/;
    push(@out, $gendir.$1."_exec.h");
    push(@out, $gendir.$1."_exec.cpp");
  }
  if ($flags =~ /-Gxhex/ && $flags =~ /-Wb,exec_export_include=(\S*)/) {
    push(@out, $1);
  }
  if ($flags =~ /-Gxhst/) {
    if ($flags =~ /-Wb,stub_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,stub_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /-Gxhsk/) {
    if ($flags =~ /-Wb,skel_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,skel_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /-Gxhsta/) {
    if ($flags =~ /-Wb,amic_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,amic_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /--Gxhimpl/) {
    if ($flags =~ /-Wb,impl_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,impl_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /--Gxhcpr/) {
    if ($flags =~ /-Wb,stub_proxy_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,stub_proxy_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /--Gxhat/) {
    if ($flags =~ /-Wb,anytypecode_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,anytypecode_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /-Gxhsv/ && $flags =~ /-Wb,svnt_export_include=(\S*)/) {
    push(@out, $1);
  }
  if ($flags =~ /-Gxhcn/ && $flags =~ /-Wb,conn_export_include=(\S*)/) {
    push(@out, $1);
  }

  return \@out;
}

sub get_outputexts {
  return ['\\.h']; #this is a regexp pattern, so . gets escaped
}

1;
