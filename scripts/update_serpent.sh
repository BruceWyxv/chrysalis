#!/bin/bash
# This script copies the files from ${SERPENT_HOME} to ./serpent and
# prepares them to be compile compatibly for use within MOOSE

# Acquire/define the directory for the local copy
CURRENT_DIR=$(pwd)
COPY_TO_DIR=${1:-${CURRENT_DIR}/serpent}

# Ensure our two directories are different
if [[ ${COPY_TO_DIR} -ef ${SERPENT_HOME} ]]; then
  printf "Cannot copy when the source and destination directories are the same!\n"
  exit 1
fi

# Ensure the destination directory exists and then copy
mkdir -p ${COPY_TO_DIR}
cd ${COPY_TO_DIR}
printf "Copying source files from %s to %s... \n" ${SERPENT_HOME} ${COPY_TO_DIR}
cp -p ${SERPENT_HOME}/{*.h,*.c} .

# Remove invalid beta files
rm deffcordata.c

printf "Patching .h files... \n"
for file in *.h; do
  # Only patch files that are suspect of being unmodified
  if [[ ! ${file} -nt ${SERPENT_HOME}/${file} ]]; then
    if [[ "${file}" == "header.h" ]]; then
      # Put header.h in an extern "C" block
      sed -i.old '1s;^;#ifdef __cplusplus\nextern "C"{\n#endif\n\n;' header.h
      printf "#ifdef __cplusplus\n}\n#endif\n" >> header.h
    fi

    # Place an include guard on all header files
    sed -i.old '1s;^;#pragma once\n\n;' ${file}
    rm ${file}.old
  fi
done
