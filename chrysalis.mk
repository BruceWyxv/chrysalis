# Build Serpent
###############################################################################

# Ensure a sane build environment, error if otherwise

define MESSAGE_ERROR

================================================================================
SERPENT_HOME not defined, cannot build Serpent module.

Ensure the global module makefile checks for this condition before it triggers
my build.
================================================================================
endef

ifeq ($(SERPENT_HOME),)
  $(error $(MESSAGE_ERROR))
endif

###############################################################################

# Run the script to make/update a local copy of the Serpent source code

APPLICATION_DIR			:= $(realpath $(APPLICATION_DIR))

LOCAL_SERPENT_DIR 	:= $(APPLICATION_DIR)/serpent

TRICK_SCRIPT_TO_RUN	:= $(shell $(APPLICATION_DIR)/scripts/update_serpent.sh $(APPLICATION_DIR)/serpent)

###############################################################################

# Define the build environment

NAME			:= serpent
SERPENT_LIB		:= $(LOCAL_SERPENT_DIR)/lib$(NAME)-$(METHOD).la
SERPENT_SRC 		:= $(sort $(wildcard $(LOCAL_SERPENT_DIR)/*.c))
SERPENT_OBJ		:= $(patsubst %.c, %.$(obj-suffix), $(SERPENT_SRC))
SERPENT_DEPS		:= $(patsubst %.$(obj-suffix), %.$(obj-suffix).d, $(SERPENT_OBJ))

SERPENT_CFLAGS	:= $(ADDITIONAL_CPPFLAGS) -w -ansi -ffast-math -O3 -flto
SERPENT_LDFLAGS		:= $(ADDITIONAL_LDFLAGS) -lm -flto

# Add Serpent to this app's dependencies
app_INCLUDES		+= -I$(LOCAL_SERPENT_DIR)
app_LIBS		+= $(SERPENT_LIB)

# Local source directory
MESSAGE_DIRECTORY	:= - The local copy of Serpent is in '$(LOCAL_SERPENT_DIR)'

# MPI
ifeq ($(findstring mpi,$(libmesh_CC)),)
  MESSAGE_MPI		:= - Compiling Serpent without MPI support
else
  MESSAGE_MPI		:= - MPI compiler detected! Compiling Serpent with MPI support
  SERPENT_CFLAGS	+= -DMPI
endif

# OpenMP
ifeq ($(findstring openmp,$(libmesh_CFLAGS)),)
  MESSAGE_OPENMP	:= - Compiling Serpent for single-thread operations
else
  MESSAGE_OPENMP	:= - OpenMP libraries found! Compiling Serpent with OpenMP support
  SERPENT_CFLAGS	+= -DOPEN_MP
endif

# Graphics
ifeq ($(shell ldconfig -p | grep "libgd\."),)
  MESSAGE_GRAPHICS	:= - Compiling Serpent without GD graphics support
  SERPENT_CFLAGS	+= -DNO_GFX_MODE
else
  MESSAGE_GRAPHICS	:= - GD graphics library found! Compiling Serpent with graphics support
  SERPENT_LDFLAGS	+= -lgd
endif

# Debug
ifeq ($(METHOD),dbg)
  MESSAGE_MODE		:= - Debug mode deteted, compiling Serpent with debugging options
  SERPENT_CFLAGS	+= -DDEBUG -g
  SERPENT_LDFLAGS	+= -g
else
  MESSAGE_MODE		:= - Compiling Serpent in optimized mode
endif

###############################################################################

# Create a pretty message that prints out when building Serpent

define MESSAGE_NOTIFICATION
================================================================================
  $(MESSAGE_DIRECTORY)
  $(MESSAGE_MPI)
  $(MESSAGE_OPENMP)
  $(MESSAGE_GRAPHICS)
  $(MESSAGE_MODE)
================================================================================
endef
export MESSAGE_NOTIFICATION

###############################################################################

# Create the rule to build the Serpent library

$(LOCAL_SERPENT_DIR)/%.$(obj-suffix) : $(LOCAL_SERPENT_DIR)/%.c
	@echo "MOOSE Compiling Serpent (in "$(METHOD)" mode) "$<"..."
	@$(libmesh_LIBTOOL) --tag=CC $(LIBTOOLFLAGS) --mode=compile --quiet \
	  $(libmesh_CC) $(libmesh_CPPFLAGS) $(SERPENT_CFLAGS) $(libmesh_CFLAGS) -MMD -MP -MF $@.d -MT $@ -c $< -o $@

$(SERPENT_LIB): pre_install_notifications $(SERPENT_OBJ)
	@echo "Linking Library "$@"..."
	@$(libmesh_LIBTOOL) --tag=CC $(LIBTOOLFLAGS) --mode=link --quiet \
          $(libmesh_CC) $(libmesh_CFLAGS) -o $@ $(SERPENT_OBJ) \
          $(libmesh_LDFLAGS) $(SERPENT_LDFLAGS) $(EXTERNAL_FLAGS) \
          -rpath $(LOCAL_SERPENT_DIR)
	@$(libmesh_LIBTOOL) --mode=install --quiet install -c $(SERPENT_LIB) $(LOCAL_SERPENT_DIR)

pre_install_notifications:
	@echo "$$MESSAGE_NOTIFICATION"

###############################################################################

# Add on the the 'clean' and 'clobber' rules to make sure we handle the local
# Serpent files correctly

clean::
	@rm -f $(SERPENT_LIB)
	@rm -f $(SERPENT_DEPS)
	@rm -f $(SERPENT_OBJ)

clobber::
	@if [ -d "$(LOCAL_SERPENT_DIR)" ]; then\
		echo Removing directory $(LOCAL_SERPENT_DIR); \
		rm -rf $(LOCAL_SERPENT_DIR); \
	fi

###############################################################################
