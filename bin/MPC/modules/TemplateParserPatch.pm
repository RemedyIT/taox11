package TemplateParserPatch;
# -------------------------------------------------------------------
# @file   TemplateParserPatch.pm
# @author Martin Corino
#
# @brief  TAOX11 TemplateParser patches
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

use strict;

use TemplateParser;


sub get_template_pattern {
  my ($tp, $patarg) = @_;

  my $patval = $tp->get_value($patarg);
  if (defined $patval) {
    return $patval;
  } else {
    return $patarg;
  }
}

*{TemplateParser::doif_ends_with} = sub {
  my($self, $val) = @_;

  if (defined $val) {
    my($name, $pattern) = $self->split_parameters("@$val");
    if (defined $name && defined $pattern) {
      $pattern = TemplateParserPatch::get_template_pattern ($self, $pattern);
      return ($self->get_value_with_default($name) =~ /$pattern$/);
    }
  }
  return undef;
};

*{TemplateParser::doif_starts_with} = sub {
  my($self, $val) = @_;

  if (defined $val) {
    my($name, $pattern) = $self->split_parameters("@$val");
    if (defined $name && defined $pattern) {
      $pattern = TemplateParserPatch::get_template_pattern ($self, $pattern);
      return ($self->get_value_with_default($name) =~ /^$pattern/);
    }
  }
  return undef;
};

*{TemplateParser::doif_contains} = sub {
  my($self, $val) = @_;

  if (defined $val) {
    my($name, $pattern) = $self->split_parameters("@$val");
    if (defined $name && defined $pattern) {
      $pattern = TemplateParserPatch::get_template_pattern ($self, $pattern);
      return ($self->get_value_with_default($name) =~ /$pattern/);
    }
  }
  return undef;
};
