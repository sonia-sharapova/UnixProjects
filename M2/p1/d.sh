#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

# TODO: Error-Check the command line argument is supplied.
if [[ "$#" -ne 1 ]]
then
    exit 1 
fi

# ONE_LINE_COMMAND
 seq $((`cut -d, -f1 range.txt`+96)) $((`cut -d, -f2 range.txt`+96)) | awk '{printf("%c ",$1)} END{print ""}'