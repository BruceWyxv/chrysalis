#!/bin/bash
# Allows for the building of the code from within the Netbeans IDE

source /etc/profile.d/modules.sh
MODULEPATH=/usr/local/Modules/custom
module load moose

make -j8 ${@}
