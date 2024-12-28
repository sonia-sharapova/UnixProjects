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
grep -P --color '\d{2}/\d{2}/\d{4}' "$1" | awk --field-separator="/" '{ if (($3 <= "1999") && ($3 >= "1900")) print $3;}' | sort -n

    