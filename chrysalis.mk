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

LOCAL_SERPENT_DIR 	:= $(CURDIR)/serpent

TRICK_SCRIPT_TO_RUN	:= $(shell ./scripts/update_serpent.sh $(LOCAL_SERPENT_DIR))

###############################################################################

# Define the build environment

NAME			:= serpent
SERPENT_LIB		:= $(LOCAL_SERPENT_DIR)/lib$(NAME)-$(METHOD).la
SERPENT_SRC 		:= $(sort $(wildcard $(LOCAL_SERPENT_DIR)/*.c))
SERPENT_OBJ		:= $(patsubst %.c, %.$(obj-suffix), $(SERPENT_SRC))
SERPENT_DEPS		:= $(patsubst %.$(obj-suffix), %.$(obj-suffix).d, $(SERPENT_OBJ))

ADDITIONAL_CPPFLAGS	+= -w -ansi -ffast-math -O3
ADDITIONAL_LDFLAGS	+= -lm

MESSAGE_DIRECTORY	:= - The local copy of Serpent is in '$(LOCAL_SERPENT_DIR)'
ifeq ($(shell ldconfig -p | grep "libgd\."),)
  MESSAGE_GRAPHICS	:= - Compiling Serpent without GD graphics support
  ADDITIONAL_CPPFLAGS	+= -DNO_GFX_MODE
else
  MESSAGE_GRAPHICS	:= - GD graphics library found! Compiling Serpent with graphics support
  ADDITIONAL_LDFLAGS	+= -lgd
endif

ifeq ($(METHOD),dbg)
  MESSAGE_MODE	:= - Debug mode deteted, compiling Serpent with debugging options
  ADDITIONAL_CPPFLAGS	+= -DDEBUG -g
  ADDITIONAL_LDFLAGS	+= -g
else
  MESSAGE_MODE	:= - Compiling Serpent in optimized mode
endif

app_INCLUDES		+= -I$(LOCAL_SERPENT_DIR)
app_LIBS		+= $(SERPENT_LIB)

###############################################################################

# Create a pretty message that prints out when building Serpent

define MESSAGE_NOTIFICATION
================================================================================
  $(MESSAGE_DIRECTORY)
  $(MESSAGE_GRAPHICS)
  $(MESSAGE_MODE)
================================================================================
endef
export MESSAGE_NOTIFICATION

###############################################################################

# Create the rule to build the Serpent library

$(SERPENT_LIB): pre_install_notifications $(SERPENT_OBJ)
	@echo "Linking Library "$@"..."
	@$(libmesh_LIBTOOL) --tag=CC $(LIBTOOLFLAGS) --mode=link --quiet \
          $(libmesh_CC) $(libmesh_CFLAGS) -o $@ $(SERPENT_OBJ) \
          $(libmesh_LDFLAGS) $(ADDITIONAL_LDFLAGS) $(EXTERNAL_FLAGS) \
          -rpath $(realpath $(LOCAL_SERPENT_DIR))
	@$(libmesh_LIBTOOL) --mode=install --quiet install -c $(SERPENT_LIB) $(realpath $(LOCAL_SERPENT_DIR))

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
		echo Removing directory $(realpath $(LOCAL_SERPENT_DIR)); \
		rm -rf $(LOCAL_SERPENT_DIR); \
	fi

###############################################################################
