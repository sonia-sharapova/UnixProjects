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
grep -Po '\(\d{3}-\d{3}-\d{4}\)' "$1" | wc -l

    