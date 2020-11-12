# -*- Makefile -*-

#----------------------------------------------------------------------------
#       Common Makefile rules for all of TAOX11
#----------------------------------------------------------------------------

####
#### Required macros for TAOX11.
####
ifeq ("$(TAOX11_ROOT)", "")
  $(error TAOX11_ROOT must be defined!)
endif

####
#### Macro customization.
####

# Turn on symbol versioning. The scheme that we follow is to allow
# applications dependent on libraries, with same version numbers (major,
# minor and beta) to run, but applications with dependencies on libraries
# with different minor or major or beta versions to fail.
#
ifeq (cmd,$(findstring cmd,$(SHELL)))
TAOX11_MAJOR_VERSION := $(shell awk "/TAOX11_MAJOR_VERSION/ { print $$3}" ${TAOX11_ROOT}/tao/x11/versionx11.h)
TAOX11_MINOR_VERSION := $(shell awk "/TAOX11_MINOR_VERSION/ { print $$3}" ${TAOX11_ROOT}/tao/x11/versionx11.h)
TAOX11_MICRO_VERSION := $(shell awk "/TAOX11_MICRO_VERSION/ { print $$3}" ${TAOX11_ROOT}/tao/x11/versionx11.h)
else
TAOX11_MAJOR_VERSION := $(shell awk '/TAOX11_MAJOR_VERSION/ { print $$3}' ${TAOX11_ROOT}/tao/x11/versionx11.h)
TAOX11_MINOR_VERSION := $(shell awk '/TAOX11_MINOR_VERSION/ { print $$3}' ${TAOX11_ROOT}/tao/x11/versionx11.h)
TAOX11_MICRO_VERSION := $(shell awk '/TAOX11_MICRO_VERSION/ { print $$3}' ${TAOX11_ROOT}/tao/x11/versionx11.h)
endif

GNUACE_PROJECT_VERSION = $(TAOX11_MAJOR_VERSION).$(TAOX11_MINOR_VERSION).$(TAOX11_MICRO_VERSION)
