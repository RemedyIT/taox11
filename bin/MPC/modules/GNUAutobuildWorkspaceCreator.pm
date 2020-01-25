package GNUAutobuildWorkspaceCreator;
# -------------------------------------------------------------------
# @file   GNUAutobuildWorkspaceCreator.pm
# @author Martin Corino
#
# @brief  TAOX11 AutobuildWorkspaceCreator
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

use strict;

use GNUACEWorkspaceCreator;
use GNUAutobuildProjectCreator;
use MakeWorkspaceBase;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(GNUACEWorkspaceCreator MakeWorkspaceBase WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

my($base) = 'GNUmakefile';

# ************************************************************
# Subroutine Section
# ************************************************************
sub extractType {
    return 'gnuace';
}

sub write_project_targets {
  my($self)   = shift;
  my($fh)     = shift;
  my($crlf)   = shift;
  my($target) = shift;
  my($list)   = shift;

  foreach my $project (@$list) {
      my($dname) = $self->mpc_dirname($project);
      my($chdir) = ($dname ne '.');
      my($output_project) = ($chdir ? $self->mpc_basename($project) : $project);

      print $fh "\t\@if (",
            ($chdir ? "cd $dname && " : ''),
            "\$(MAKE) -f ",
            $output_project,
            " $target &> $output_project.log) ; then rc__=0; else rc__=1; fi; cat $dname/$output_project.log ; rm $dname/$output_project.log ; if [ \$\$rc__ -eq 1 ]; then /bin/false; fi $crlf";
  }
}

1;
