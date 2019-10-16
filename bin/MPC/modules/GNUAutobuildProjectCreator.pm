package GNUAutobuildProjectCreator;
# -------------------------------------------------------------------
# @file   GNUAutobuildProjectCreator.pm
# @author Martin Corino
#
# @brief  TAOX11 AutobuildCreator
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

use strict;
use File::Basename;

use GNUACEProjectCreator;
use MakeProjectBase;
use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(GNUACEProjectCreator MakeProjectBase ProjectCreator);

sub extractType {
    return 'gnuace';
}

1;
