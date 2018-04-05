#!/bin/bash
# This script copies the files from ${SERPENT_HOME} to the argument provided,
# defaulting to ./serpent otherwise, them modifies the headers files for use
# within MOOSE

# Array checker function
array_contains () {
    local array="$1[@]"
    local seeking=$2
    for element in "${!array}"; do
        if [[ $element == $seeking ]]; then
            return 3
        fi
    done
    return 0
}

# Ignored source files
IGNORED=("deffcordata.c")

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
printf "Copying source files from %s to %s... \n" ${SERPENT_HOME} ${COPY_TO_DIR}
cd ${SERPENT_HOME}
for file in {*.h,*.c} ; do
  # Only copy files that are suspect of being modified
  if [[ ${file} -nt ${COPY_TO_DIR}/${file} ]]; then
    array_contains IGNORED ${file}
    if [[ $? -ne 0 ]]; then
      printf "Skipping ignored file %s\n" ${file}
    else
      printf "%s is newer, copying...\n" ${file}
      cp -p ${file} ${COPY_TO_DIR}
    fi
  fi
done

if [[ "${MODIFIED}" == 1 ]]; then
  # Move to the destination directory
  cd ${COPY_TO_DIR}

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
fi
