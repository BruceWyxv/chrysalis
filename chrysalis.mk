# Build Serpent
###############################################################################

# Set up the environment

NAME 			:= serpent
SERPENT_DIR 		:= $(CURDIR)/serpent
SERPENT_LIB		:= $(SERPENT_DIR)/lib$(NAME)-$(METHOD).la
SERPENT_SRC 		:= $(sort $(wildcard $(SERPENT_DIR)/*.c))
SERPENT_OBJ		:= $(patsubst %.c, %.$(obj-suffix), $(SERPENT_SRC))
SERPENT_DEPS		:= $(patsubst %.$(obj-suffix), %.$(obj-suffix).d, $(SERPENT_OBJ))

ADDITIONAL_CPPFLAGS	+= -w -ansi -ffast-math -O3 -DDEBUG -g
ADDITIONAL_LDFLAGS	+= -lm -lgd -g

app_INCLUDES		+= -I$(SERPENT_DIR)
app_LIBS		+= $(SERPENT_LIB)

###############################################################################

# Rule to build the library

$(SERPENT_LIB): $(SERPENT_OBJ)
	@$(libmesh_LIBTOOL) --tag=CC $(LIBTOOLFLAGS) --mode=link --quiet \
          $(libmesh_CC) $(libmesh_CFLAGS) -o $@ $(SERPENT_OBJ) $(libmesh_LDFLAGS) $(ADDITIONAL_LDFLAGS) $(EXTERNAL_FLAGS) -rpath $(realpath $(SERPENT_DIR))
	@$(libmesh_LIBTOOL) --mode=install --quiet install -c $(SERPENT_LIB) $(realpath $(SERPENT_DIR))

###############################################################################

clean::
	@rm -f $(SERPENT_LIB)
	@rm -f $(SERPENT_DEPS)
	@rm -f $(SERPENT_OBJ)

clobber::
	@if [ -d "$(SERPENT_DIR)" ]; then\
		echo Removing directory $(realpath $(SERPENT_DIR)); \
		rm -rf $(SERPENT_DIR); \
	fi

###############################################################################
