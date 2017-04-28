#!/bin/bash
# This script copies the files from ${SERPENT_HOME} to ./serpent and
# prepares them to be compile compatibly for use within MOOSE

CURRENT_DIR=$(pwd)
FOLDER=$(basename ${CURRENT_DIR})
if [ "${FOLDER}" == "scripts" ]; then
  cd ..
fi
mkdir -p ./serpent

# cd into serpent
cd serpent

printf "Copying source files from %s to %s... " ${SERPENT_HOME} $(pwd)
cp ${SERPENT_HOME}/{*.h,*.c} .
printf "done! \n"

# Remove invalid beta files
rm deffcordata.c

printf "Updating .h files with include guard... "
for file in *.h; do
  sed -i.old '1s;^;#pragma once\n\n;' ${file}
done
printf "done!\n"

printf 'Placing extern "C"{} block on "header.h"... '
  sed -i.old '1s;^;#ifdef __cplusplus\nextern "C"{\n#endif\n\n;' header.h
  printf "#ifdef __cplusplus\n}\n#endif\n" >> header.h
printf "done!\n"
